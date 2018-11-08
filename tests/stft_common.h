/*
 * Contains common utility functions and fixtures
 */

#ifndef STFT_COMMON_H
#define STFT_COMMON_H

#include <gtest/gtest.h>
#include <cmath>
#include "spectrogram.h"
#include "stft.h"

double MaxError(std::vector<double> v1, std::vector<double> v2);

class Fixture_4x2x1_1Hz : public ::testing::Test {
        
protected:
    
    STFT* mystft;
    SpectrogramInput props;
    SpectrogramConfig config;

    virtual void SetUp();
    virtual void TearDown();
    
};

class Fixture_7x3x2_1Hz : public ::testing::Test {
        
protected:
    
    STFT* mystft;
    SpectrogramInput props;
    SpectrogramConfig config;

    virtual void SetUp();
    virtual void TearDown();
    
};

class Fixture_10x4x1_piHz : public ::testing::Test {
        
protected:
    
    STFT* mystft;
    SpectrogramInput props;
    SpectrogramConfig config;

    virtual void SetUp();
    virtual void TearDown();
    
};

class Fixture_18x6x3_10Hz : public ::testing::Test {
        
protected:
    
    STFT* mystft;
    SpectrogramInput props;
    SpectrogramConfig config;

    virtual void SetUp();
    virtual void TearDown();
};


class Fixture_18x6x3_10Hz_interleaved : public ::testing::Test {
        
protected:
    
    STFT* mystft;
    SpectrogramInput props;
    SpectrogramConfig config;

    virtual void SetUp();
    virtual void TearDown();
};

class Fixture_16 : public ::testing::Test {
        
protected:
    
    STFT* mystft;
    SpectrogramInput props;
    SpectrogramConfig config;

    virtual void SetUp();
    
};

class Fixture_17 : public ::testing::Test {
        
protected:
    
    STFT* mystft;
    SpectrogramInput props;
    SpectrogramConfig config;

    virtual void SetUp();
    
};

#endif /* STFT_COMMON_H */

