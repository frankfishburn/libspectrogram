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
