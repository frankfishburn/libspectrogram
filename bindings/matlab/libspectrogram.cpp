#include <string.h>
#include "matrix.h"
#include "mex.h"
#include "spectrogram.h"

void mexFunction(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
    // Check number of outputs
    if (nlhs != 1) {
        printf("There should be one output\n");
        mexErrMsgIdAndTxt("libspectrogram:input:badnumberout", "There should be one output.");
    }

    // Check number of inputs
    if (nrhs != 2) {
        mexErrMsgIdAndTxt("libspectrogram:input:badnumberin",
                          "There should be two inputs: input matrix and configuration struct");
    }

    // Check input data dimensions
    const int            rank    = mxGetNumberOfDimensions(prhs[0]);
    const unsigned long* dims    = mxGetDimensions(prhs[0]);
    unsigned long        numel   = 1;
    unsigned long        max_dim = 0;
    for (int i = 0; i < rank; i++) {
        numel *= dims[i];
        if (dims[i] > max_dim)
            max_dim = dims[i];
    }

    if (numel != max_dim) {
        printf("Should only be one non-singleton dimension.\n");
        return;
    }

    // Check/prepare input parameters
    SpectrogramInput props;
    props.num_samples = numel;
    props.stride      = 1;

    void* signal = NULL;
    if (mxIsSingle(prhs[0])) {
        props.data_size = sizeof(float);
        signal          = (void*)mxGetSingles(prhs[0]);
    } else if (mxIsDouble(prhs[0])) {
        props.data_size = sizeof(double);
        signal          = (void*)mxGetDoubles(prhs[0]);
    } else {
        mexErrMsgIdAndTxt("libspectrogram:input:nonfloat", "Data should be single or double type.");
    }

    const mxArray* field;
    field = mxGetField(prhs[1], 0, "sample_rate");
    if (!(mxIsDouble(field) & mxIsScalar(field))) {
        mexErrMsgIdAndTxt("libspectrogram:input:badsamplerate", "Sample rate should be double-precision");
    }
    props.sample_rate = mxGetScalar(field);

    // Check/prepare configuration
    int               buflen, status;
    char*             buffer;
    SpectrogramConfig config;

    // Configuration: Padding Mode
    field  = mxGetField(prhs[1], 0, "padding_mode");
    buflen = mxGetN(field) + 1;
    buffer = new char[buflen];
    status = mxGetString(field, buffer, buflen);
    if (status != 0) {
        mexErrMsgIdAndTxt("libspectrogram:input:badpadding", "Invalid padding mode");
    }
    if (strcmp(buffer, "TRUNCATE") == 0) {
        config.padding_mode = TRUNCATE;
    } else if (strcmp(buffer, "PAD") == 0) {
        config.padding_mode = PAD;
    } else {
        mexErrMsgIdAndTxt("libspectrogram:input:badpadding", "Invalid padding mode");
    }
    delete buffer;

    // Configuration: Window Type
    field  = mxGetField(prhs[1], 0, "window_type");
    buflen = mxGetN(field) + 1;
    buffer = new char[buflen];
    status = mxGetString(field, buffer, buflen);
    if (status != 0) {
        mexErrMsgIdAndTxt("libspectrogram:input:badwindowtype", "Invalid window type");
    }
    if (strcmp(buffer, "RECTANGULAR") == 0) {
        config.window_type = RECTANGULAR;
    } else if (strcmp(buffer, "TRIANGULAR") == 0) {
        config.window_type = TRIANGULAR;
    } else if (strcmp(buffer, "BARTLETT") == 0) {
        config.window_type = BARTLETT;
    } else if (strcmp(buffer, "HANN") == 0) {
        config.window_type = HANN;
    } else if (strcmp(buffer, "WELCH") == 0) {
        config.window_type = WELCH;
    } else if (strcmp(buffer, "HAMMING") == 0) {
        config.window_type = HAMMING;
    } else if (strcmp(buffer, "BLACKMAN") == 0) {
        config.window_type = BLACKMAN;
    } else if (strcmp(buffer, "NUTTALL") == 0) {
        config.window_type = NUTTALL;
    } else if (strcmp(buffer, "BLACKMAN_NUTTALL") == 0) {
        config.window_type = BLACKMAN_NUTTALL;
    } else if (strcmp(buffer, "BLACKMAN_HARRIS") == 0) {
        config.window_type = BLACKMAN_HARRIS;
    } else {
        mexErrMsgIdAndTxt("libspectrogram:input:badwindowtype", "Invalid window type");
    }
    delete buffer;

    // Configuration: Window Length
    field = mxGetField(prhs[1], 0, "window_length");
    if (!(mxIsDouble(field) & mxIsScalar(field))) {
        mexErrMsgIdAndTxt("libspectrogram:input:badwindowlength", "Invalid window length");
    }
    config.window_length = (unsigned long)mxGetScalar(field);

    // Configuration: Window Overlap
    field = mxGetField(prhs[1], 0, "window_overlap");
    if (!(mxIsDouble(field) & mxIsScalar(field))) {
        mexErrMsgIdAndTxt("libspectrogram:input:badwindowoverlap", "Invalid window overlap");
    }
    config.window_overlap = (unsigned long)mxGetScalar(field);

    // Configuration: Window Overlap
    field = mxGetField(prhs[1], 0, "transform_length");
    if (!(mxIsDouble(field) & mxIsScalar(field))) {
        mexErrMsgIdAndTxt("libspectrogram:input:badwindowoverlap", "Invalid transform length");
    }
    config.transform_length = (unsigned long)mxGetScalar(field);

    // Create the program
    SpectrogramTransform* mySTFT = spectrogram_create(&props, &config);

    // Get output parameters
    unsigned long time_len = spectrogram_get_timelen(mySTFT);
    unsigned long freq_len = spectrogram_get_freqlen(mySTFT);

    // Create empty output struct
    const char* fnames[4];
    fnames[0] = "time";
    fnames[1] = "freq";
    fnames[2] = "power";
    fnames[3] = "phase";
    plhs[0]   = mxCreateStructMatrix(1, 1, 4, fnames);

    // Allocate outputs
    mxArray* mxTime  = mxCreateNumericMatrix(1, time_len, mxDOUBLE_CLASS, mxREAL);
    mxArray* mxFreq  = mxCreateNumericMatrix(1, freq_len, mxDOUBLE_CLASS, mxREAL);
    mxArray* mxPower = mxCreateNumericMatrix(freq_len, time_len, mxDOUBLE_CLASS, mxREAL);
    mxArray* mxPhase = mxCreateNumericMatrix(freq_len, time_len, mxDOUBLE_CLASS, mxREAL);

    // Add fields to output structure
    mxSetField(plhs[0], 0, "time", mxTime);
    mxSetField(plhs[0], 0, "freq", mxFreq);
    mxSetField(plhs[0], 0, "power", mxPower);
    mxSetField(plhs[0], 0, "phase", mxPhase);

    // Perform the STFT
    spectrogram_execute(mySTFT, signal);

    // Get the outputs
    spectrogram_get_time(mySTFT, mxGetPr(mxTime));
    spectrogram_get_freq(mySTFT, mxGetPr(mxFreq));
    spectrogram_get_power(mySTFT, mxGetPr(mxPower));
    spectrogram_get_phase(mySTFT, mxGetPr(mxPhase));
}
