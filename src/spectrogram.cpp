#include "spectrogram.h"
#include "stft.h"
#include <stdlib.h> 

// Create
SpectrogramTransform* spectrogram_create(InputProps *props, StftConfig *config) {

    return reinterpret_cast<SpectrogramTransform*>( new STFT( *props , *config ) );
    
}

// Execute
void spectrogram_execute(SpectrogramTransform* transform, double* input) {
    
    STFT *mystft = reinterpret_cast<STFT*>(transform);
    mystft->compute(input);
    
}


// Get output parameters
size_t spectrogram_get_timelen(SpectrogramTransform* transform) {
    
    STFT *mystft = reinterpret_cast<STFT*>(transform);
    return mystft->num_windows();
    
}

size_t spectrogram_get_freqlen(SpectrogramTransform* transform) {
    
    STFT *mystft = reinterpret_cast<STFT*>(transform);
    return mystft->num_frequencies();
    
}


// Get outputs
void spectrogram_get_time(SpectrogramTransform* transform, double* time) {
    
    STFT *mystft = reinterpret_cast<STFT*>(transform);
    mystft->get_time(time);
    
}

void spectrogram_get_freq(SpectrogramTransform* transform, double* freq) {
    
    STFT *mystft = reinterpret_cast<STFT*>(transform);
    mystft->get_freq(freq);
    
}
void spectrogram_get_power(SpectrogramTransform* transform, double* power) {
    
    STFT *mystft = reinterpret_cast<STFT*>(transform);
    mystft->get_power(power);
    
}

void spectrogram_get_phase(SpectrogramTransform* transform, double* phase) {
    
    STFT *mystft = reinterpret_cast<STFT*>(transform);
    mystft->get_phase(phase);
    
}

// Destroy
void spectrogram_destroy(SpectrogramTransform* transform) {

    STFT *mystft = reinterpret_cast<STFT*>(transform);
    delete mystft;
    
}
