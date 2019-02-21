#include <stdio.h>
#include <stdlib.h>

#include "spectrogram.h"

int main(int argc, char** argv) {
    // Create input signal
    double signal[18] = {0.655098, 1.162612, 0.118998,  -0.501636, 0.959744, 1.340386, 0.585268,  -0.776188, 0.751267,
                         1.255095, 0.505957, -0.300923, 0.890903,  1.959291, 0.547216, -0.861376, 0.149294,  1.257508};

    // Specify the input signal properties
    SpectrogramInput props;
    props.num_samples = 18;
    props.sample_rate = 10;
    props.data_size   = sizeof(double);
    props.stride      = 1;

    // Specify the configuration for the transform
    SpectrogramConfig config;
    config.padding_mode     = TRUNCATE;
    config.window_type      = HAMMING;
    config.window_length    = 6;
    config.window_overlap   = 3;
    config.transform_length = 16;

    // Create the program
    SpectrogramTransform* mySTFT = spectrogram_create(&props, &config);

    // Get output parameters
    unsigned long time_len = spectrogram_get_timelen(mySTFT);
    unsigned long freq_len = spectrogram_get_freqlen(mySTFT);

    // Allocate outputs
    double* time  = malloc(sizeof(double) * time_len);
    double* freq  = malloc(sizeof(double) * freq_len);
    double* power = malloc(sizeof(double) * time_len * freq_len);
    double* phase = malloc(sizeof(double) * time_len * freq_len);

    // Perform the STFT
    spectrogram_execute(mySTFT, (void*)signal);

    // Get the outputs
    spectrogram_get_time(mySTFT, (void*)time);
    spectrogram_get_freq(mySTFT, (void*)freq);
    spectrogram_get_power(mySTFT, (void*)power);
    spectrogram_get_phase(mySTFT, (void*)phase);

    // Destroy the transform
    spectrogram_destroy(mySTFT);

    // Print the result
    printf("Power:");
    for (int f = freq_len - 1; f > -1; f--) {
        printf("\n%6f Hz:  ", freq[f]);
        for (int t = 0; t < time_len; t++) {
            printf("%6f   ", power[t * freq_len + f]);
        }
        printf("\n");
    }

    printf("\nPhase:");
    for (int f = freq_len - 1; f > -1; f--) {
        printf("\n%6f Hz:  ", freq[f]);
        for (int t = 0; t < time_len; t++) {
            printf("%+6f   ", phase[t * freq_len + f]);
        }
        printf("\n");
    }

    free(time);
    free(freq);
    free(power);
    free(phase);

    return (EXIT_SUCCESS);
}
