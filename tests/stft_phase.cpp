/*
 * This file tests the phase calculation
 */

#include "stft_common.h"
#include <algorithm>


TEST_F(Fixture_18x6x3_10Hz, Phase) {

    std::vector<double> phase = { 0,   -0.1136,   -2.6167,    0,
                                  0,   -2.0363,    1.7501,    0,
                                  0,   -3.0426,    0.6256,    0,
                                  0,    2.3869,   -1.0981,    0,
                                  0,   -0.7533,   -2.9982,    0 };
    
    EXPECT_LT(MaxError(mystft->get_phase_vector<double>(),phase), .0001);    

}


TEST_F(Fixture_18x6x3_10Hz_pow2, Phase) {

    std::vector<double> phase = {0,   -0.7565,   -0.1360,   -0.3521,   -1.2602,   -2.2674,    2.9402,    1.7012,    0,
                                 0,   -0.7459,   -1.5064,   -2.3119,    3.0893,    2.1065,    0.9750,   -0.4811,    0,
                                 0,   -1.1614,   -2.3040,    2.8814,    1.8536,    0.9150,    0.0905,   -0.5418,    0,
                                 0,   -1.3072,   -2.7969,    1.8532,    0.4471,   -0.7339,   -1.7907,   -2.6837,    0,
                                 0,   -0.1093,   -0.3824,   -0.9830,   -1.7825,   -2.6826,    2.6275,    1.5098,    0};
        
    EXPECT_LT(MaxError(mystft->get_phase_vector<double>(),phase), .0001);
    
}
