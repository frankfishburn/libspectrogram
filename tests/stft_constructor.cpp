#include "stft_common.h"

// Tests for the 4x2x1_1Hz configuration
TEST_F(Fixture_4x2x1_1Hz, DerivedParameters) {
    
    EXPECT_EQ(mystft->num_windows(), 3);
    EXPECT_EQ(mystft->num_frequencies(), 2);
    
    std::vector<double> coefs = {1.0, 1.0};
    EXPECT_EQ(mystft->window_coefs(), coefs);
    
}

TEST_F(Fixture_4x2x1_1Hz, Time) {
    
    std::vector<double> time = {0.5, 1.5, 2.5};
    EXPECT_EQ(mystft->time(), time);
    
}

TEST_F(Fixture_4x2x1_1Hz, Frequency) {
    
    std::vector<double> frequency = {0, 0.5};
    EXPECT_EQ(mystft->frequency(), frequency);
    
}

// Tests for the 7x3x2_1Hz configuration
TEST_F(Fixture_7x3x2_1Hz, DerivedParameters) {
    
    EXPECT_EQ(mystft->num_windows(), 5);
    EXPECT_EQ(mystft->num_frequencies(), 2);
    
    std::vector<double> coefs = {1.0, 1.0, 1.0};
    EXPECT_EQ(mystft->window_coefs(), coefs);
    
}

TEST_F(Fixture_7x3x2_1Hz, Time) {
    
    std::vector<double> time = {1.0, 2.0, 3.0, 4.0, 5.0};
    EXPECT_EQ(mystft->time(), time);
    
}

TEST_F(Fixture_7x3x2_1Hz, Frequency) {
    
    std::vector<double> frequency = {0, 1.0/3.0};
    EXPECT_EQ(mystft->frequency(), frequency);
    
}

// Tests for the 10x4x1_piHz configuration
TEST_F(Fixture_10x4x1_piHz, DerivedParameters) {
    
    EXPECT_EQ(mystft->num_windows(), 3);
    EXPECT_EQ(mystft->num_frequencies(), 3);
    
    std::vector<double> coefs = {1.0, 1.0, 1.0, 1.0};
    EXPECT_EQ(mystft->window_coefs(), coefs);
    
}

TEST_F(Fixture_10x4x1_piHz, Time) {
    
    std::vector<double> time = {0.47746, 1.4324, 2.3873};
    EXPECT_LT(MaxError(mystft->time(), time), .0001);
    
}

TEST_F(Fixture_10x4x1_piHz, Frequency) {
    
    std::vector<double> frequency = {0, M_PI/4.0, M_PI/2.0};
    EXPECT_EQ(mystft->frequency(), frequency);
    
}

