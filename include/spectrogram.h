#ifndef SPECTROGRAM_H
#define SPECTROGRAM_H

#ifdef __cplusplus
extern "C" {
#endif
    
typedef enum {
    TRUNCATE,
    HEAD,
    TAIL,
    HEADTAIL,
    HALFWINDOW
} PaddingMode;

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

// Define the properties of the input data
typedef struct {
    
    double sample_rate;
    unsigned long num_samples;
    
} InputProps;

// Define the parameters of the transform
typedef struct {
    
    PaddingMode padding_mode;
    WindowType window_type;
    unsigned long window_length;
    unsigned long window_overlap;

} StftConfig;

// Define the C interface
struct SpectrogramTransform;
typedef struct SpectrogramTransform SpectrogramTransform;

// Contructor
SpectrogramTransform* spectrogram_create(InputProps*, StftConfig*);

// Computation
void spectrogram_execute(SpectrogramTransform* program, double* input);

// Get data-agnostic outputs
unsigned long spectrogram_get_timelen(SpectrogramTransform* program);
unsigned long spectrogram_get_freqlen(SpectrogramTransform* program);

// Get data-specific outputs
void spectrogram_get_time(SpectrogramTransform* program, double* time);
void spectrogram_get_freq(SpectrogramTransform* program, double* freq);
void spectrogram_get_power(SpectrogramTransform* program, double* power);
void spectrogram_get_phase(SpectrogramTransform* program, double* phase);

// Destructor
void spectrogram_destroy(SpectrogramTransform*);

#ifdef __cplusplus
}
#endif

#endif /* SPECTROGRAM_H */

