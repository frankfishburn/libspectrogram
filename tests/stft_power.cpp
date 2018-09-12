/*
 * This file tests the windowing functions (Hamming, Blackman, etc) for even and
 * odd size windows.
 */

#include "stft_common.h"
#include <algorithm>


TEST_F(Fixture_18x6x3_10Hz, Power) {

    std::vector<double> power = {0.022942, 0.031642, 0.117803, 0.009019, 
                                 0.171276, 0.322919, 0.114042, 0.000697, 
                                 0.152259, 0.294051, 0.128774, 0.000883, 
                                 0.141962, 0.131140, 0.140239, 0.009721, 
                                 0.027941, 0.199963, 0.210372, 0.013773};
    
    EXPECT_LT(MaxError(mystft->power(),power), .0001);    

}
