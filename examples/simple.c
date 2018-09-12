#include <stdio.h>
#include <stdlib.h>

#include "spectrogram.h"

int main(int argc, char** argv) {
    
    // Create input signal
    double signal[18] = {0.655098, 1.162612, 0.118998, -0.501636, 0.959744, 1.340386, 
                    0.585268, -0.776188, 0.751267, 1.255095, 0.505957, -0.300923, 
                    0.890903, 1.959291, 0.547216, -0.861376, 0.149294, 1.257508};
    
    // Specify the input signal properties
    InputProps props;
    props.num_samples = 18;
    props.sample_rate = 10;
    
    // Specify the configuration for the transform
    StftConfig config;
    config.padding_mode = TRUNCATE;
    config.window_type = HAMMING;
    config.window_length = 6;
    config.window_overlap = 3;
    
    // Create the program
    SpectrogramTransform *mySTFT = spectrogram_create( &props, &config );
    
    // Perform the STFT
    SpectrogramData result = spectrogram_execute( mySTFT , signal );
    
    // Print the result
    for (int f=result.freq_length-1; f>-1; f--) {
        printf("\n%6f Hz:  ",result.freq[f]);
        for (int t=0; t<result.time_length; t++) {
        
            printf("%6f   ",result.power[t*result.freq_length+f]);
        }
        printf("\n");
    }
    
    // Destroy the transform
    spectrogram_destroy( mySTFT );
    
    return (EXIT_SUCCESS);
}

