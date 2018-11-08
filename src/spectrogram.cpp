#include "spectrogram.h"
#include "stft.h"
#include <stdlib.h> 

// Create
SpectrogramTransform* spectrogram_create(SpectrogramInput *props, SpectrogramConfig *config) {

    return reinterpret_cast<SpectrogramTransform*>( new STFT( *props , *config ) );
    
}

// Execute
void spectrogram_execute(SpectrogramTransform* transform, void* input) {
    
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
void spectrogram_get_time(SpectrogramTransform* transform, void* time) {
    
    STFT *mystft = reinterpret_cast<STFT*>(transform);
    
    if (mystft->data_size()==sizeof(float)) {
        mystft->get_time<float>(time);
    
    } else if (mystft->data_size()==sizeof(double)) {
        mystft->get_time<double>(time);
    }
}

void spectrogram_get_freq(SpectrogramTransform* transform, void* freq) {
    
    STFT *mystft = reinterpret_cast<STFT*>(transform);
    
    if (mystft->data_size()==sizeof(float)) {
        mystft->get_freq<float>(freq);
        
    } else if (mystft->data_size()==sizeof(double)) {
        mystft->get_freq<double>(freq);
    }
    
    
}

void spectrogram_get_power(SpectrogramTransform* transform, void* power) {
    
    STFT *mystft = reinterpret_cast<STFT*>(transform);
    
    if (mystft->data_size()==sizeof(float)) {
        mystft->get_power<float>(power);
        
    } else if (mystft->data_size()==sizeof(double)) {
        mystft->get_power<double>(power);
    }
}

void spectrogram_get_phase(SpectrogramTransform* transform, void* phase) {
    
    STFT *mystft = reinterpret_cast<STFT*>(transform);
    
    if (mystft->data_size()==sizeof(float)) {
        mystft->get_phase<float>(phase);
        
    } else if (mystft->data_size()==sizeof(double)) {
        mystft->get_phase<double>(phase);
    }
    
}

void spectrogram_get_power_periodogram(SpectrogramTransform* transform, void* power) {
    
    STFT *mystft = reinterpret_cast<STFT*>(transform);
    
    if (mystft->data_size()==sizeof(float)) {
        mystft->get_power_periodogram<float>(power);
        
    } else if (mystft->data_size()==sizeof(double)) {
        mystft->get_power_periodogram<double>(power);
    }
}

void spectrogram_get_phase_periodogram(SpectrogramTransform* transform, void* phase) {
    
    STFT *mystft = reinterpret_cast<STFT*>(transform);
    
    if (mystft->data_size()==sizeof(float)) {
        mystft->get_phase_periodogram<float>(phase);
        
    } else if (mystft->data_size()==sizeof(double)) {
        mystft->get_phase_periodogram<double>(phase);
    }
    
}

// Destroy
void spectrogram_destroy(SpectrogramTransform* transform) {

    STFT *mystft = reinterpret_cast<STFT*>(transform);
    delete mystft;
    
}
