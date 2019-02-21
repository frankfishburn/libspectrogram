#include <cmath>
#include <cstring>
#include <memory>

#include "stft.h"

STFT::STFT(const SpectrogramInput& new_props, const SpectrogramConfig& new_config) {
    // Copy inputs to internal
    sample_rate_      = new_props.sample_rate;
    num_samples_      = new_props.num_samples;
    data_size_        = new_props.data_size;
    stride_           = new_props.stride;
    padding_mode_     = new_config.padding_mode;
    window_type_      = new_config.window_type;
    window_length_    = new_config.window_length;
    window_overlap_   = new_config.window_overlap;
    transform_length_ = new_config.transform_length;

    // Validate inputs
    validate();

    // Initialize derived parameters
    calc_num_windows();
    calc_num_frequencies();

    // Allocate spectra buffer and create FFT plan
    init_fft();

    // Initialize
    init_window_coefs();
    init_time();
    init_frequency();
}

STFT::~STFT() {
    if (isFloat()) {
        fftwf_free(fourier_spectra_);
        fftwf_destroy_plan(fftwf_plan_);
        fftwf_cleanup();

    } else if (isDouble()) {
        fftw_free(fourier_spectra_);
        fftw_destroy_plan(fftw_plan_);
        fftw_cleanup();
    }
}

void STFT::validate() {
    if (sample_rate_ <= 0.0) {
        fprintf(stderr, "WARNING: Sample rate must be greater than zero. Setting to 1.");
        sample_rate_ = 1.0;
    }

    if (window_length_ < 2) {
        fprintf(stderr, "WARNING: Window length must be greater than 1. Setting to 2.");
        window_length_ = 2;
    }

    if (window_overlap_ >= window_length_) {
        fprintf(stderr, "WARNING: Window overlap must be less than window length. Setting to 1 less.");
        window_overlap_ = window_length_ - 1;
    }

    if (transform_length_ < window_length_) {
        fprintf(
            stderr,
            "WARNING: Transform length must be greater than or equal to the window length. Setting to window length.");
        transform_length_ = window_length_;
    }

    if (!isFloat() & !isDouble()) {
        fprintf(stderr, "WARNING: data_size is not float or double.");
    }

    if (stride_ < 1) {
        fprintf(stderr, "WARNING: Stride cannot be less than 1. Setting to 1.");
    }
}

void STFT::calc_num_windows() {
    const double samples     = (double)num_samples_;
    const double length      = (double)window_length_;
    const double increment   = (double)window_length_ - window_overlap_;
    const double num_windows = (samples - length) / increment + 1.0;

    switch (padding_mode_) {
        case TRUNCATE:
            num_windows_ = (unsigned long)floor(num_windows);
            break;

        case PAD:
            num_windows_ = (unsigned long)ceil(num_windows);
            break;

        default:
            throw "Unknown padding mode";
    }
}

void STFT::calc_num_frequencies() {
    if (transform_length_ % 2 == 0) {
        num_frequencies_ = transform_length_ / 2 + 1;
    } else {
        num_frequencies_ = (transform_length_ + 1) / 2;
    }
}

// Allocate the internal buffer to hold segmented data / Fourier spectra, and FFTW plan
void STFT::init_fft() {
    // Set FFT parameters
    const int           wid[]    = {(int)transform_length_};
    unsigned int        flags    = FFTW_MEASURE | FFTW_PRESERVE_INPUT;
    const fftw_r2r_kind fft_kind = FFTW_R2HC;

    if (isFloat()) {
        // Allocate
        fourier_spectra_ = fftwf_malloc(sizeof(float) * num_windows_ * transform_length_);

        // Create FFT plan
        fftwf_plan_ = fftwf_plan_many_r2r(1, wid, (int)num_windows_, (float*)fourier_spectra_, NULL, 1,
                                          (int)transform_length_, (float*)fourier_spectra_, NULL, 1,
                                          (int)transform_length_, &fft_kind, flags);

    } else if (isDouble()) {
        // Allocate
        fourier_spectra_ = fftw_malloc(sizeof(double) * num_windows_ * transform_length_);

        // Create FFT plan
        fftw_plan_ = fftw_plan_many_r2r(1, wid, (int)num_windows_, (double*)fourier_spectra_, NULL, 1,
                                        (int)transform_length_, (double*)fourier_spectra_, NULL, 1,
                                        (int)transform_length_, &fft_kind, flags);
    }
}

void STFT::init_window_coefs() {
    window_coefs_.resize(window_length_);

    unsigned long i;
    double        x, alpha, beta, alpha0, alpha1, alpha2, alpha3;
    const double  mult = (2.0 * M_PI) / (window_length_ - 1);

    switch (window_type_) {
        case RECTANGULAR:
            for (i = 0; i < window_length_; i++) {
                window_coefs_[i] = 1.0;
            }
            break;

        case TRIANGULAR:
            for (i = 0; i < ceil(window_length_ / 2.0); i++) {
                if (window_length_ % 2 == 0) {
                    window_coefs_[i] = (2 * (i + 1.0) - 1) / window_length_;
                } else {
                    window_coefs_[i] = 2 * (i + 1.0) / (window_length_ + 1.0);
                }
                window_coefs_[window_length_ - i - 1] = window_coefs_[i];
            }
            break;

        case WELCH:
            x = (window_length_ - 1) / 2.0;
            for (i = 0; i < window_length_; i++) {
                window_coefs_[i] = 1 - ((i - x) / x) * ((i - x) / x);
            }
            break;

        case HANN:
            for (i = 0; i < window_length_; i++) {
                window_coefs_[i] = 0.5 - 0.5 * cos(i * mult);
            }
            break;

        case HAMMING:
            alpha = 25.0 / 46.0;
            beta  = 21.0 / 46.0;
            for (i = 0; i < window_length_; i++) {
                window_coefs_[i] = alpha - beta * cos(i * mult);
            }
            break;

        case BLACKMAN:
            alpha0 = 7938.0 / 18608.0;
            alpha1 = 9240.0 / 18608.0;
            alpha2 = 1430.0 / 18608.0;
            for (i = 0; i < window_length_; i++) {
                window_coefs_[i] = alpha0 - alpha1 * cos(i * mult) + alpha2 * cos(2.0 * i * mult);
            }
            break;

        case NUTTALL:
            alpha0 = 0.355768;
            alpha1 = 0.487396;
            alpha2 = 0.144232;
            alpha3 = 0.012604;
            for (i = 0; i < window_length_; i++) {
                window_coefs_[i] = alpha0 - alpha1 * cos(i * mult) + alpha2 * cos(2.0 * i * mult) -
                                   alpha3 * cos(3.0 * i * mult);
            }
            break;

        case BLACKMAN_NUTTALL:
            alpha0 = 0.3635819;
            alpha1 = 0.4891775;
            alpha2 = 0.1365995;
            alpha3 = 0.0106411;
            for (i = 0; i < window_length_; i++) {
                window_coefs_[i] = alpha0 - alpha1 * cos(i * mult) + alpha2 * cos(2.0 * i * mult) -
                                   alpha3 * cos(3.0 * i * mult);
            }
            break;

        case BLACKMAN_HARRIS:
            alpha0 = 0.35875;
            alpha1 = 0.48829;
            alpha2 = 0.14128;
            alpha3 = 0.01168;
            for (i = 0; i < window_length_; i++) {
                window_coefs_[i] = alpha0 - alpha1 * cos(i * mult) + alpha2 * cos(2.0 * i * mult) -
                                   alpha3 * cos(3.0 * i * mult);
            }
            break;

        default:
            throw "Unknown window";
            break;
    }

    // Compute the scaling factor from the window coefficients
    double coef_sq = 0.0;
    for (i = 0; i < window_length_; i++)
        coef_sq += window_coefs_[i] * window_coefs_[i];

    scale_factor_ = 1.0 / (sample_rate_ * coef_sq);
}

// Create output time vector
void STFT::init_time() {
    time_.resize(num_windows_);

    const double time_increment = (window_length_ - window_overlap_) / sample_rate_;
    const double time_offset    = (window_length_ - 1) / (2.0 * sample_rate_);

    for (unsigned long window = 0; window < num_windows_; window++) {
        time_[window] = window * time_increment + time_offset;
    }
}

// Create output frequency vector
void STFT::init_frequency() {
    frequency_.resize(num_frequencies_);

    const double freq_resolution = sample_rate_ / transform_length_;

    for (unsigned long freq_index = 0; freq_index < num_frequencies_; freq_index++) {
        frequency_[freq_index] = freq_resolution * freq_index;
    }
}

// Perform segmentation and windowing of input data, then calculate FFTs
void STFT::compute(void* vsignal) {
    // Check input
    if (num_windows_ < 1) {
        return;
    }

    unsigned long window_increment = window_length_ - window_overlap_;
    unsigned long input_index;

    if (isFloat()) {
        float* signal          = (float*)vsignal;
        float* fourier_spectra = (float*)fourier_spectra_;

        // Zero-out buffer
        memset(fourier_spectra_, 0, sizeof(float) * num_windows_ * transform_length_);

        // Apply segmentation and windowing
        for (unsigned long window = 0; window < num_windows_; window++) {
            for (unsigned long sample = 0; sample < window_length_; sample++) {
                input_index                                          = window * window_increment + sample;
                fourier_spectra[window * transform_length_ + sample] = window_coefs_[sample] *
                                                                       signal[stride_ * input_index];
            }
        }

        // Compute Fourier spectra in-place
        fftwf_execute(fftwf_plan_);

    } else if (isDouble()) {
        double* signal          = (double*)vsignal;
        double* fourier_spectra = (double*)fourier_spectra_;

        // Zero-out buffer
        memset(fourier_spectra_, 0, sizeof(double) * num_windows_ * transform_length_);

        // Apply segmentation and windowing
        for (unsigned long window = 0; window < num_windows_; window++) {
            for (unsigned long sample = 0; sample < window_length_; sample++) {
                input_index                                          = window * window_increment + sample;
                fourier_spectra[window * transform_length_ + sample] = window_coefs_[sample] *
                                                                       signal[stride_ * input_index];
            }
        }

        // Compute Fourier spectra in-place
        fftw_execute(fftw_plan_);
    }
}

template <typename T>
void STFT::get_time(void* vout_ptr) {
    T* out_ptr = (T*)vout_ptr;
    for (unsigned long window_index = 0; window_index < num_windows_; window_index++)
        out_ptr[window_index] = time_[window_index];
}
template void STFT::get_time<float>(void*);
template void STFT::get_time<double>(void*);

template <typename T>
void STFT::get_freq(void* vout_ptr) {
    T* out_ptr = (T*)vout_ptr;
    for (unsigned long frequency_index = 0; frequency_index < num_frequencies_; frequency_index++)
        out_ptr[frequency_index] = frequency_[frequency_index];
}
template void STFT::get_freq<float>(void*);
template void STFT::get_freq<double>(void*);

template <typename T>
void STFT::get_power(void* vout_ptr) {
    unsigned long row_in, row_out, frequency_index;
    T             real, imag;
    T*            out_ptr         = (T*)vout_ptr;
    T*            fourier_spectra = (T*)fourier_spectra_;

    for (unsigned long window_index = 0; window_index < num_windows_; window_index++) {
        row_in  = window_index * transform_length_;
        row_out = window_index * num_frequencies_;

        // Special case for freq=0 because FFTW doesn't give a complex value since its always zero
        real             = fourier_spectra[row_in];
        out_ptr[row_out] = real * real * scale_factor_;

        // Normal frequencies P=(i^2 + j^2) * 2*scale
        for (frequency_index = 1; frequency_index < num_frequencies_ - 1; frequency_index++) {
            real = fourier_spectra[row_in + frequency_index];
            imag = fourier_spectra[row_in + (transform_length_ - frequency_index)];

            out_ptr[row_out + frequency_index] = (real * real + imag * imag) * 2 * scale_factor_;
        }

        // Special case for Nyquist
        real = fourier_spectra[row_in + frequency_index];
        if (transform_length_ % 2 == 0) {
            out_ptr[row_out + frequency_index] = real * real * scale_factor_;
        } else {
            imag                               = fourier_spectra[row_in + (transform_length_ - frequency_index)];
            out_ptr[row_out + frequency_index] = (real * real + imag * imag) * 2 * scale_factor_;
        }
    }
}
template void STFT::get_power<float>(void*);
template void STFT::get_power<double>(void*);

template <typename T>
void STFT::get_phase(void* vout_ptr) {
    unsigned long row_in, row_out, frequency_index;
    T             real, imag;
    T*            out_ptr         = (T*)vout_ptr;
    T*            fourier_spectra = (T*)fourier_spectra_;

    for (unsigned long window_index = 0; window_index < num_windows_; window_index++) {
        row_in  = window_index * transform_length_;
        row_out = window_index * num_frequencies_;

        // Special case for freq=0 because FFTW doesn't give a complex value since its always zero
        out_ptr[row_out] = 0;

        // Normal frequencies P=(i^2 + j^2) * 2*scale
        for (frequency_index = 1; frequency_index < num_frequencies_ - 1; frequency_index++) {
            real = fourier_spectra[row_in + frequency_index];
            imag = fourier_spectra[row_in + (transform_length_ - frequency_index)];

            out_ptr[row_out + frequency_index] = atan2(imag, real);
        }

        // Special case for Nyquist
        if (transform_length_ % 2 == 0) {
            out_ptr[row_out + frequency_index] = 0;
        } else {
            real                               = fourier_spectra[row_in + frequency_index];
            imag                               = fourier_spectra[row_in + (transform_length_ - frequency_index)];
            out_ptr[row_out + frequency_index] = atan2(imag, real);
        }
    }
}
template void STFT::get_phase<float>(void*);
template void STFT::get_phase<double>(void*);

template <typename T>
std::vector<T> STFT::get_power_vector() {
    std::vector<T> pwr;
    pwr.resize(num_windows_ * num_frequencies_);
    get_power<T>((void*)pwr.data());
    return pwr;
}
template std::vector<float>  STFT::get_power_vector();
template std::vector<double> STFT::get_power_vector();

template <typename T>
std::vector<T> STFT::get_phase_vector() {
    std::vector<T> phs;
    phs.resize(num_windows_ * num_frequencies_);
    get_phase<T>((void*)phs.data());
    return phs;
}
template std::vector<float>  STFT::get_phase_vector();
template std::vector<double> STFT::get_phase_vector();

template <typename T>
void STFT::get_power_periodogram(void* vout_ptr) {
    unsigned long row_in;
    T             real, imag;
    T*            out_ptr         = (T*)vout_ptr;
    T*            fourier_spectra = (T*)fourier_spectra_;
    memset(out_ptr, 0, num_frequencies_ * data_size_);

    for (unsigned long window_index = 0; window_index < num_windows_; window_index++) {
        row_in = window_index * transform_length_;

        // Special case for freq=0 because FFTW doesn't give a complex value since its always zero
        real = fourier_spectra[row_in];
        out_ptr[0] += real * real * scale_factor_;

        // Normal frequencies P=(i^2 + j^2) * 2*scale
        for (unsigned long frequency_index = 1; frequency_index < num_frequencies_ - 1; frequency_index++) {
            real = fourier_spectra[row_in + frequency_index];
            imag = fourier_spectra[row_in + (transform_length_ - frequency_index)];

            out_ptr[frequency_index] += (real * real + imag * imag) * 2 * scale_factor_;
        }

        // Special case for Nyquist
        real = fourier_spectra[row_in + num_frequencies_ - 1];
        if (num_frequencies_ % 2 == 0) {
            out_ptr[num_frequencies_ - 1] += real * real * scale_factor_;
        } else {
            out_ptr[num_frequencies_ - 1] += real * real * 2 * scale_factor_;
        }
    }
}
template void STFT::get_power_periodogram<float>(void*);
template void STFT::get_power_periodogram<double>(void*);

template <typename T>
void STFT::get_phase_periodogram(void* vout_ptr) {
    unsigned long row_in;
    T             real, imag;
    T*            out_ptr         = (T*)vout_ptr;
    T*            fourier_spectra = (T*)fourier_spectra_;
    memset(out_ptr, 0, num_frequencies_ * data_size_);

    for (unsigned long window_index = 0; window_index < num_windows_; window_index++) {
        row_in = window_index * transform_length_;

        // Normal frequencies P=(i^2 + j^2) * 2*scale
        for (unsigned long frequency_index = 1; frequency_index < num_frequencies_ - 1; frequency_index++) {
            real = fourier_spectra[row_in + frequency_index];
            imag = fourier_spectra[row_in + (transform_length_ - frequency_index)];

            out_ptr[frequency_index] = atan2(imag, real);
        }
    }
}
template void STFT::get_phase_periodogram<float>(void*);
template void STFT::get_phase_periodogram<double>(void*);
