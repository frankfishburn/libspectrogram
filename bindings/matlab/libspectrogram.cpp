#include <string.h>
#include "mex.h"
#include "matrix.h"
#include "spectrogram.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

    // Check number of outputs
    if (nlhs>1) {
        printf("There should only be one output");
        return;
    }

    // Create empty output struct
    const char* fnames[4];
    fnames[0] = "time";
    fnames[1] = "freq";
    fnames[2] = "power";
    fnames[3] = "phase";
    plhs[0] = mxCreateStructMatrix(1, 1, 4, fnames);

    // Check input data type
    if ( !mxIsDouble(prhs[0]) ) {
        printf("Data should be double type.");
        return;
    }

    // Check input data dimensions
    const int rank = mxGetNumberOfDimensions( prhs[0] );
    const unsigned long *dims = mxGetDimensions( prhs[0] );
    
    unsigned long numel = 1;
    unsigned long max_dim = 0;
    for (int i=0; i<rank; i++) {
        numel *= dims[i];
        if (dims[i]>max_dim)
            max_dim = dims[i];
    }
    
    if (numel!=max_dim) {
        printf("Should only be one non-singleton dimension.\n");
        return;
    }
    
    double *signal = mxGetPr(prhs[0]);

    // Specify the input signal properties
    InputProps props;
    props.num_samples = numel;
    props.sample_rate = 0;

    // Specify the configuration for the transform
    StftConfig config;
    config.padding_mode   = TRUNCATE;
    config.window_type    = HAMMING;
    config.window_length  = 0;
    config.window_overlap = 0;

    // Parse input parameters
    bool isset_sample_rate = false;
    bool isset_window_length = false;
    bool isset_window_overlap = false;
    
    for (int i=1; i<nrhs; i+=2) {

        if (!mxIsChar(prhs[i])) {
            printf("Input %i should be a parameter name",i+1);
            return;
        }
        
        size_t buflen = mxGetN(prhs[i]) + 1;
        char key[buflen];
        int status = mxGetString(prhs[i], key, (mwSize) buflen);
        
        if (status!=0)
            continue;
        
        double value = mxGetScalar(prhs[i+1]);
        
        printf("%s = %f\n",key,value); fflush(stdout);
        
        if (strcmp(key,"sample_rate")==0) {
                props.sample_rate = value;
                isset_sample_rate = true;
        }

        if (strcmp(key,"window_length")==0) {
                config.window_length = value;
                isset_window_length = true;
        }
        
        if (strcmp(key,"window_overlap")==0) {
                config.window_overlap = value;
                isset_window_overlap = true;
        }
        
    }

    // Check that all required fields were set    
    if (!isset_sample_rate) {
        printf("Sample rate is unset");
        return;
    }
        
    if (!isset_window_length) {
        printf("Window length is unset");
        return;
    }
        
    if (!isset_window_overlap) {
        printf("Window overlap is unset");
        return;
    }
    
    // Create the program
    SpectrogramTransform *mySTFT = spectrogram_create( &props, &config );

    // Get output parameters
    unsigned long time_len = spectrogram_get_timelen( mySTFT );
    unsigned long freq_len = spectrogram_get_freqlen( mySTFT );
    
    // Allocate outputs
    mxArray *mxTime = mxCreateNumericMatrix( 1, time_len, mxDOUBLE_CLASS, mxREAL );
    mxArray *mxFreq = mxCreateNumericMatrix( 1, freq_len, mxDOUBLE_CLASS, mxREAL );
    mxArray *mxPower = mxCreateNumericMatrix( freq_len, time_len, mxDOUBLE_CLASS, mxREAL );
    mxArray *mxPhase = mxCreateNumericMatrix( freq_len, time_len, mxDOUBLE_CLASS, mxREAL );
    
    // Add fields to output structure
    mxSetField( plhs[0], 0, "time", mxTime );
    mxSetField( plhs[0], 0, "freq", mxFreq );
    mxSetField( plhs[0], 0, "power", mxPower );
    mxSetField( plhs[0], 0, "phase", mxPhase );
    
    // Perform the STFT
    spectrogram_execute( mySTFT , signal );

    // Get the outputs
    spectrogram_get_time( mySTFT , mxGetPr(mxTime) );
    spectrogram_get_freq( mySTFT , mxGetPr(mxFreq) );
    spectrogram_get_power( mySTFT , mxGetPr(mxPower) );
    spectrogram_get_phase( mySTFT , mxGetPr(mxPhase) );

}
