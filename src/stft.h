#ifndef STFT_H
#define STFT_H

#include <fftw3.h>
#include <vector>

#include "spectrogram.h"

class STFT {
public:

    // Setup
    STFT(const InputProps& input, const StftConfig& config);
    STFT(const STFT& orig);
    virtual ~STFT();

    // Input accessors
    unsigned long num_samples() const { return num_samples_; };
    double sample_rate() const { return sample_rate_; };
    PaddingMode padding_mode() const {return padding_mode_; };
    WindowType window_type() const {return window_type_; };
    unsigned long window_length() const {return window_length_; };
    unsigned long window_overlap() const {return window_overlap_; };
    
    // Derived accessors
    unsigned long num_windows() const { return num_windows_; };
    unsigned long num_frequencies() const { return num_frequencies_; };
    std::vector<double> window_coefs() const{ return window_coefs_; };
        
    // Computation
    void compute(double*);

    // Outputs
    std::vector<double> time() const { return time_; };
    std::vector<double> frequency() const { return frequency_; };
    std::vector<double> power();
    std::vector<double> phase();

    void get_time(double *out_ptr);
    void get_freq(double *out_ptr);
    void get_power(double *out_ptr);
    void get_phase(double *out_ptr);
    
    
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

    
    int status_ = 0;

    // User-supplied input/transform parameters
    //StftConfig config_;
    //InputProps props_;
    double sample_rate_;
    unsigned long num_samples_;
    PaddingMode padding_mode_;
    WindowType window_type_;
    unsigned long window_length_;
    unsigned long window_overlap_;

    // Derived parameters
    unsigned long num_windows_;
    unsigned long num_frequencies_;
    std::vector<double> window_coefs_;
    double scale_factor_;
    
    // Outputs
    std::vector<double> time_;
    std::vector<double> frequency_;

    // FFT-related
    fftw_plan fft_plan_;
    double* fourier_spectra_;

};

#endif /* STFT_H */

