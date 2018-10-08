#ifndef SPECTROGRAM_H
#define SPECTROGRAM_H

#ifdef __cplusplus
extern "C" {
#endif
/** \mainpage libspectrogram
 *
 * \section about_sec About
 *
 * libspectrogram is a C++ library for computing spectrograms and periodograms.
 *
**/ 

/**
 * @brief Specifies the type of padding to apply to the signal before segmentation and windowing.
**/   
typedef enum {
    TRUNCATE,   /**< Round down to the nearest whole segment */
    PAD         /**< Zero-pad the ending of the signal to the next whole segment */
} PaddingMode;

/**
 * @brief Specifies the windowing function to use on each segment
**/
typedef enum {
    RECTANGULAR,
    TRIANGULAR,
    BARTLETT,
    HANN,
    WELCH,
    HAMMING,
    BLACKMAN,
    NUTTALL,
    BLACKMAN_NUTTALL,
    BLACKMAN_HARRIS
} WindowType;

/**
 * @brief Specifies the properties of the input signal (sample rate, number of samples, bytes per sample)
**/
typedef struct {
    
    double sample_rate;        /**< The acquisition sample rate of the signal */
    unsigned long num_samples; /**< The number of samples in the signal */
    int data_size;             /**< The size of each sample in bytes */
    int stride;                /**< Indicates the number of values to skip between each consecutive sample (1 for contiguous data) */
    
} InputProps;

/**
 * @brief Specifies the properties of the STFT (padding mode, window function, window length, window overlap)
**/
typedef struct {
    
    PaddingMode padding_mode;      /**< The method for zero-padding the input signal */
    WindowType window_type;        /**< The windowing function to use on each segment */
    unsigned long window_length;   /**< The length in samples of each segment */
    unsigned long window_overlap;  /**< The number of samples of overlap between consecutive segments */

} StftConfig;

/**
 * @brief The opaque pointer for the transform
**/
struct SpectrogramTransform;
typedef struct SpectrogramTransform SpectrogramTransform;

/**
 * @brief The STFT contructor
 * @param[in] InputProps* A pointer to the properties of the input signal
 * @param[in] StftConfig* A pointer to the configuration of the desired STFT
 * @returns The opaque pointer to the transform object
**/
SpectrogramTransform* spectrogram_create(InputProps*, StftConfig*);

/**
 * @brief Compute the STFT on an input signal
 * @param[in] SpectrogramTransform* The opaque pointer to the transform object
 * @param[in] void* The input signal
**/
void spectrogram_execute(SpectrogramTransform* program, void* input);

/**
 * @brief Get the number of time points (i.e. number of windows)
 * @param[in] SpectrogramTransform* The opaque pointer to the transform object
 * @returns the number of time points
**/
unsigned long spectrogram_get_timelen(SpectrogramTransform* program);

/**
 * @brief Get the number of frequencies
 * @param[in] SpectrogramTransform* The opaque pointer to the transform object
 * @returns the number of frequencies
**/
unsigned long spectrogram_get_freqlen(SpectrogramTransform* program);

/**
 * @brief Get the time vector
 * @param[in] SpectrogramTransform* The opaque pointer to the transform object
 * @param[out] void* Array of time points (in seconds) at the center of each segment/window
**/
void spectrogram_get_time(SpectrogramTransform* program, void* time);

/**
 * @brief Get the frequency vector
 * @param[in] SpectrogramTransform* The opaque pointer to the transform object
 * @param[out] void* Array of frequencies (in Hz)
**/
void spectrogram_get_freq(SpectrogramTransform* program, void* freq);

/**
 * @brief Get the STFT power
 * @param[in] SpectrogramTransform* The opaque pointer to the transform object
 * @param[out] void* Array of spectral power at each time and frequency
**/
void spectrogram_get_power(SpectrogramTransform* program, void* power);

/**
 * @brief Get the STFT phase
 * @param[in] SpectrogramTransform* The opaque pointer to the transform object
 * @param[out] void* Array of phase angle at each time and frequency
**/
void spectrogram_get_phase(SpectrogramTransform* program, void* phase);

/**
 * @brief Get the STFT power periodogram
 * @param[in] SpectrogramTransform* The opaque pointer to the transform object
 * @param[out] void* Array of spectral power at each frequency
**/
void spectrogram_get_power_periodogram(SpectrogramTransform* program, void* power);

/**
 * @brief Get the STFT phase periodogram
 * @param[in] SpectrogramTransform* The opaque pointer to the transform object
 * @param[out] void* Array of phase angle at each frequency
**/
void spectrogram_get_phase_periodogram(SpectrogramTransform* program, void* phase);

/**
 * @brief The STFT destructor
 * @param[in] SpectrogramTransform* The opaque pointer to the transform object
**/
void spectrogram_destroy(SpectrogramTransform*);

#ifdef __cplusplus
}
#endif

#endif /* SPECTROGRAM_H */

