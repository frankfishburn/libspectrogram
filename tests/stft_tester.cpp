#include "stft_tester.h"

double MaxError(std::vector<double> v1, std::vector<double> v2) {
    if (v1.size() != v2.size())
        return std::numeric_limits<double>::infinity();

    double error = 0;
    for (int i = 0; i < (int)v1.size(); i++)
        error = std::max(error, std::abs(v1[i] - v2[i]));

    return error;
}

void STFT_Tester::TearDown() {
    spectrogram_destroy(stft);
}

void STFT_Tester::TestTime() {
    EXPECT_EQ(spectrogram_get_timelen(stft), time.size());

    std::vector<double> time_observed;
    time_observed.resize(time.size());
    spectrogram_get_time(stft, time_observed.data());

    EXPECT_LT(MaxError(time, time_observed), .0001);
}

void STFT_Tester::TestFrequency() {
    EXPECT_EQ(spectrogram_get_freqlen(stft), freq.size());

    std::vector<double> freq_observed;
    freq_observed.resize(freq.size());
    spectrogram_get_freq(stft, freq_observed.data());

    EXPECT_LT(MaxError(freq, freq_observed), .0001);
}

void STFT_Tester::TestPower() {
    EXPECT_EQ(spectrogram_get_timelen(stft) * spectrogram_get_freqlen(stft), power.size());

    std::vector<double> power_observed;
    power_observed.resize(power.size());
    spectrogram_get_power(stft, power_observed.data());

    EXPECT_LT(MaxError(power, power_observed), .0001);
}

void STFT_Tester::TestPhase() {
    EXPECT_EQ(spectrogram_get_timelen(stft) * spectrogram_get_freqlen(stft), phase.size());

    std::vector<double> phase_observed;
    phase_observed.resize(phase.size());
    spectrogram_get_phase(stft, phase_observed.data());

    EXPECT_LT(MaxError(phase, phase_observed), .0001);
}