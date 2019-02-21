#ifndef STFT_TESTER_H
#define STFT_TESTER_H

#include <gtest/gtest.h>
#include <vector>
#include "spectrogram.h"

double MaxError(std::vector<double> v1, std::vector<double> v2);

class STFT_Tester : public ::testing::Test {
   protected:
    SpectrogramInput      props;
    SpectrogramConfig     config;
    SpectrogramTransform* stft;
    std::vector<double>   input;
    std::vector<double>   time;
    std::vector<double>   freq;
    std::vector<double>   power;
    std::vector<double>   phase;
    void                  TearDown();
    void                  TestTime();
    void                  TestFrequency();
    void                  TestPower();
    void                  TestPhase();
};

#endif /* STFT_TESTER_H */
