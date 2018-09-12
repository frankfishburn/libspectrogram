#include <memory>
#include <cstring>
#include <cmath>

#include "stft.h"

STFT::STFT(const InputProps& new_props, const StftConfig& new_config) {
    
    // Copy inputs to internal
    sample_rate_ = new_props.sample_rate;
    num_samples_ = new_props.num_samples;
    padding_mode_ = new_config.padding_mode;
    window_type_ = new_config.window_type;
    window_length_ = new_config.window_length;
    window_overlap_ = new_config.window_overlap;

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

    fftw_free(fourier_spectra_);
    fftw_destroy_plan(fft_plan_);
    
}

void STFT::validate() {

    if (sample_rate_ <= 0.0) {
        fprintf(stderr,"WARNING: Sample rate must be greater than zero. Setting to 1.");
        sample_rate_ = 1.0;
    }

    if (window_length_ < 2) {
        fprintf(stderr,"WARNING: Window length must be greater than 1. Setting to 2.");
        window_length_ = 2;
    }
    
    if (window_overlap_ >= window_length_) {
        fprintf(stderr,"WARNING: Window overlap must be less than window length. Setting to 1 less.");
        window_overlap_ = window_length_ - 1;
    }

}

void STFT::calc_num_windows() {

    const double samples = (double) num_samples_;
    const double length = (double) window_length_;
    const double increment = (double) window_length_ - window_overlap_;

    switch (padding_mode_) {
        case TRUNCATE:
            num_windows_ = (unsigned long) (floor((samples - length) / increment) + 1);
            break;

        case TAIL:
            num_windows_ = (unsigned long) (ceil((samples - length) / increment) + 1);
            break;

        case HEADTAIL:
            num_windows_ = (unsigned long) (ceil((samples - length) / increment) + 1);
            break;

        case HALFWINDOW:
            num_windows_ = (unsigned long) (ceil((samples - ceil(length / 2)) / increment) + 1);
            break;

        default:
            throw "Unknown padding mode";

    }

}

void STFT::calc_num_frequencies() {
    
    if (window_length_ % 2 == 0) {
        num_frequencies_ = window_length_ / 2 + 1;
    } else {
        num_frequencies_ = (window_length_ + 1) / 2;
    }

}

// Allocate the internal buffer to hold segmented data / Fourier spectra, and FFTW plan
void STFT::init_fft() {
        
    // Allocate
    fourier_spectra_ = (double*) fftw_malloc(sizeof (double) * num_windows_ * window_length_);

    // Create FFT plan
    const int wid[] = {(int) window_length_};
    unsigned int flags = FFTW_MEASURE | FFTW_PRESERVE_INPUT;
    const fftw_r2r_kind fft_kind = FFTW_R2HC;
    fft_plan_ = fftw_plan_many_r2r(1, wid, (int) num_windows_,\
                                    fourier_spectra_, NULL, 1, (int) window_length_,\
                                    fourier_spectra_, NULL, 1, (int) window_length_, &fft_kind, flags);

    // Zero-out buffer
    memset(fourier_spectra_, 0, sizeof (double) * num_windows_ * window_length_);
    
}

void STFT::init_window_coefs() {
    
    window_coefs_.resize(window_length_);

    unsigned long i;
    double x, alpha, beta, alpha0, alpha1, alpha2, alpha3;
    const double mult = (2.0 * M_PI) / (window_length_ - 1);

    switch (window_type_) {
        case RECTANGULAR:
            for (i = 0; i < window_length_; i++) {
                window_coefs_[i] = 1.0;
            }
            break;

        case TRIANGULAR:
            x = (window_length_ - 1.0) / 2.0;
            for (i = 0; i < ceil(window_length_/2.0); i++) {
                if (window_length_ % 2 == 0) {
                    window_coefs_[i] = (2*(i+1.0)-1)/window_length_;
                } else {
                    window_coefs_[i] = 2*(i+1.0)/(window_length_+1.0);
                }
                window_coefs_[window_length_-i-1] = window_coefs_[i];
                
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
            beta = 21.0 / 46.0;
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
                window_coefs_[i] = alpha0 - alpha1 * cos(i * mult) + alpha2 * cos(2.0 * i * mult) - alpha3 * cos(3.0 * i * mult);
            }
            break;

        case BLACKMAN_NUTTALL:
            alpha0 = 0.3635819;
            alpha1 = 0.4891775;
            alpha2 = 0.1365995;
            alpha3 = 0.0106411;
            for (i = 0; i < window_length_; i++) {
                window_coefs_[i] = alpha0 - alpha1 * cos(i * mult) + alpha2 * cos(2.0 * i * mult) - alpha3 * cos(3.0 * i * mult);
            }
            break;

        case BLACKMAN_HARRIS:
            alpha0 = 0.35875;
            alpha1 = 0.48829;
            alpha2 = 0.14128;
            alpha3 = 0.01168;
            for (i = 0; i < window_length_; i++) {
                window_coefs_[i] = alpha0 - alpha1 * cos(i * mult) + alpha2 * cos(2.0 * i * mult) - alpha3 * cos(3.0 * i * mult);
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

    time_.resize( num_windows_ );
    
    const double time_increment = (window_length_ - window_overlap_) / sample_rate_;
    const double time_offset = (window_length_-1) / (2.0 * sample_rate_);

    for (unsigned long window = 0; window < num_windows_; window++) {
        
        time_[window] = window * time_increment + time_offset;
        
    }

}

// Create output frequency vector
void STFT::init_frequency() {

    frequency_.resize( num_frequencies_ );
    
    const double freq_resolution = sample_rate_/window_length_;

    for (unsigned long freq_index = 0; freq_index < num_frequencies_; freq_index++) {

        frequency_[freq_index] = freq_resolution * freq_index;
        
    }

}

// Perform segmentation and windowing of input data, then calculate FFTs
void STFT::compute(double *signal) {

    // Check input
    if (num_windows_ < 1) {
        status_ = 1;
        return;
    }

    unsigned long input_offset = 0;
    unsigned long window_increment = window_length_ - window_overlap_;

    // Apply segmentation and windowing
    for (unsigned long window = 0; window < num_windows_; window++) {

        for (unsigned long sample = 0; sample < window_length_; sample++) {

            fourier_spectra_[window * window_length_ + sample] = window_coefs_[sample] * signal[window * window_increment + sample + input_offset];

        }
    }

    // Compute Fourier spectra in-place
    fftw_execute(fft_plan_);

}

std::vector<double> STFT::power() {

    std::vector<double> pwr;
    pwr.resize(num_windows_ * num_frequencies_);
    
    unsigned long row_in, row_out;
    double real, imag;
    
    
    for (unsigned long window_index=0; window_index<num_windows_; window_index++) {
        
        row_in = window_index * window_length_;
        row_out = window_index * num_frequencies_;
    
        // Special case for freq=0 because FFTW doesn't give a complex value since its always zero
        real = fourier_spectra_[row_in];
        pwr[row_out] = real*real * scale_factor_;
        
        // Normal frequencies P=(i^2 + j^2) * 2*scale
        for (unsigned long frequency_index=1; frequency_index<num_frequencies_-1; frequency_index++) {
            
            real = fourier_spectra_[row_in + frequency_index];
            imag = fourier_spectra_[row_in + (window_length_-frequency_index)];
            
            pwr[row_out + frequency_index] = (real*real + imag*imag) * 2*scale_factor_;
            
        }
        
        // Special case for Nyquist
        real = fourier_spectra_[row_in + num_frequencies_ - 1];
        if (num_frequencies_ % 2 == 0) {
            pwr[row_out + num_frequencies_ - 1] = real*real * scale_factor_;
        } else {
            pwr[row_out + num_frequencies_ - 1] = real*real * 2*scale_factor_;
        }
        
    }
    
    return pwr;
}