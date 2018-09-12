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

// Define the output data structure
typedef struct {
    
    unsigned long time_length;
    unsigned long freq_length;
    double *time;
    double *freq;
    double *power;
    double *phase;

} SpectrogramData;

// Define the C interface
struct SpectrogramTransform;
typedef struct SpectrogramTransform SpectrogramTransform;

// Contructor
SpectrogramTransform* spectrogram_create(InputProps*, StftConfig*);

// Computation
SpectrogramData spectrogram_execute(SpectrogramTransform* program, double* input);

// Destructor
void spectrogram_destroy(SpectrogramTransform*);

// Free
void spectrogram_free(SpectrogramData*);


#ifdef __cplusplus
}
#endif

#endif /* SPECTROGRAM_H */

