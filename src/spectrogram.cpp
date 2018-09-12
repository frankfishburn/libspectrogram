#include "spectrogram.h"
#include "stft.h"
#include <stdlib.h> 

// Create
SpectrogramTransform* spectrogram_create(InputProps *props, StftConfig *config) {

    return reinterpret_cast<SpectrogramTransform*>( new STFT( *props , *config ) );
    
}

// Execute
SpectrogramData spectrogram_execute(SpectrogramTransform* transform, double* input) {
    
    STFT *mystft = reinterpret_cast<STFT*>(transform);
    mystft->compute(input);
    
    SpectrogramData data;
    data.time_length = mystft->num_windows();
    data.freq_length = mystft->num_frequencies();
    data.time = mystft->time().data();
    data.freq = mystft->frequency().data();
    data.power = mystft->power().data();
    data.phase = NULL;
    
    return data;
    
}

// Destroy
void spectrogram_destroy(SpectrogramTransform* transform) {

    STFT *mystft = reinterpret_cast<STFT*>(transform);
    delete mystft;
    
}

// Free the output data
void spectrogram_free(SpectrogramData* data) {
    
    if (data->time != NULL) {
        free(data->time);
        data->time = NULL;
    }
    
    if (data->freq!=NULL) {
        free(data->freq);
        data->freq = NULL;
    }
    
    if (data->power!=NULL) {
        free(data->power);
        data->power = NULL;
    }
    
    if (data->phase!=NULL) {
        free(data->phase);
        data->phase = NULL;
    }
    
}
