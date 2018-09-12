#include <algorithm>
#include "stft_common.h"

TEST_F(Fixture_16, WindowTriangular) {

    config.window_type = TRIANGULAR;
    mystft = new STFT(props, config);
    
    std::vector<double> coefs = { 0.062500, 0.187500, 0.312500, 0.437500, 0.562500, 0.687500, 0.812500, 0.937500, 0.937500, 0.812500, 0.687500, 0.562500, 0.437500, 0.312500, 0.187500, 0.062500 };
    
    EXPECT_LT(MaxError(mystft->window_coefs(), coefs), .000001);
    
    delete(mystft);

}

TEST_F(Fixture_16, WindowHamming) {

    config.window_type = HAMMING;
    mystft = new STFT(props, config);
    
    std::vector<double> coefs = { 0.086957, 0.126425, 0.238006, 0.402405, 0.591198, 0.771739, 0.912812, 0.990024, 0.990024, 0.912812, 0.771739, 0.591198, 0.402405, 0.238006, 0.126425, 0.086957 };
    
    EXPECT_LT(MaxError(mystft->window_coefs(), coefs), .000001);
    
    delete(mystft);

}

TEST_F(Fixture_17, WindowTriangular) {

    config.window_type = TRIANGULAR;
    mystft = new STFT(props, config);
    
    std::vector<double> coefs = { 0.111111, 0.222222, 0.333333, 0.444444, 0.555556, 0.666667, 0.777778, 0.888889, 1.000000, 0.888889, 0.777778, 0.666667, 0.555556, 0.444444, 0.333333, 0.222222, 0.111111 };
    
    EXPECT_LT(MaxError(mystft->window_coefs(), coefs), .000001);
    
    delete(mystft);

}

TEST_F(Fixture_17, WindowHamming) {

    config.window_type = HAMMING;
    mystft = new STFT(props, config);
    
    std::vector<double> coefs = { 0.086957, 0.121707, 0.220669, 0.368775, 0.543478, 0.718182, 0.866288, 0.965249, 1.000000, 0.965249, 0.866288, 0.718182, 0.543478, 0.368775, 0.220669, 0.121707, 0.086957 };
    
    EXPECT_LT(MaxError(mystft->window_coefs(), coefs), .000001);
    
    delete(mystft);

}