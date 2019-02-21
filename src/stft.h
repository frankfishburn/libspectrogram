#ifndef STFT_H
#define STFT_H

#include <fftw3.h>
#include <vector>

#include "spectrogram.h"

class STFT {
   public:
    // Setup
    STFT(const SpectrogramInput& input, const SpectrogramConfig& config);
    STFT(const STFT& orig);
    virtual ~STFT();

    // Input accessors
    unsigned long num_samples() const { return num_samples_; };
    double        sample_rate() const { return sample_rate_; };
    int           data_size() const { return data_size_; };
    PaddingMode   padding_mode() const { return padding_mode_; };
    WindowType    window_type() const { return window_type_; };
    unsigned long window_length() const { return window_length_; };
    unsigned long window_overlap() const { return window_overlap_; };

    // Derived accessors
    unsigned long       num_windows() const { return num_windows_; };
    unsigned long       num_frequencies() const { return num_frequencies_; };
    std::vector<double> window_coefs() const { return window_coefs_; };

    // Computation
    void compute(void*);

    // Outputs
    template <typename T>
    std::vector<T> get_time_vector() const {
        return time_;
    }
    template <typename T>
    std::vector<T> get_frequency_vector() const {
        return frequency_;
    }
    template <typename T>
    std::vector<T> get_power_vector();
    template <typename T>
    std::vector<T> get_phase_vector();

    template <typename T>
    void get_time(void* out_ptr);
    template <typename T>
    void get_freq(void* out_ptr);
    template <typename T>
    void get_power(void* out_ptr);
    template <typename T>
    void get_phase(void* out_ptr);
    template <typename T>
    void get_power_periodogram(void* out_ptr);
    template <typename T>
    void get_phase_periodogram(void* out_ptr);

   private:
    // Input validation
    void validate();

    // Calculate derived parameters
    void calc_num_windows();
    void calc_num_frequencies();

    // Initialize
    void init_window_coefs();
    void init_fft();
    void init_time();
    void init_frequency();

    // User-supplied input parameters
    double        sample_rate_;
    unsigned long num_samples_;
    int           data_size_;
    int           stride_;

    // User-supplied transform parameters
    PaddingMode   padding_mode_;
    WindowType    window_type_;
    unsigned long window_length_;
    unsigned long window_overlap_;
    unsigned long transform_length_;

    // Derived parameters
    unsigned long       num_windows_;
    unsigned long       num_frequencies_;
    std::vector<double> window_coefs_;
    double              scale_factor_;
    bool                isFloat() const { return (data_size_ == sizeof(float)); }
    bool                isDouble() const { return (data_size_ == sizeof(double)); }

    // Outputs
    std::vector<double> time_;
    std::vector<double> frequency_;

    // FFT-related
    fftw_plan  fftw_plan_;
    fftwf_plan fftwf_plan_;
    void*      fourier_spectra_;
};

#endif /* STFT_H */
