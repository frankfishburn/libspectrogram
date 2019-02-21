#include "cases.h"
#include <cmath>

void STFT_Test_1::SetUp() {
    props.sample_rate = 1;
    props.num_samples = 4;
    props.data_size   = sizeof(double);
    props.stride      = 1;

    config.padding_mode     = TRUNCATE;
    config.window_type      = RECTANGULAR;
    config.window_length    = 2;
    config.window_overlap   = 1;
    config.transform_length = 2;

    input = std::vector<double>{1.0, 2.0, 3.0, 4.0};

    time  = std::vector<double>{0.5, 1.5, 2.5};
    freq  = std::vector<double>{0.0, 0.5};
    power = std::vector<double>{4.5, 0.5, 12.5, 0.5, 24.5, 0.5};
    phase = std::vector<double>{0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

    stft = spectrogram_create(&props, &config);
    spectrogram_execute(stft, input.data());
}

void STFT_Test_2::SetUp() {
    props.sample_rate = 1;
    props.num_samples = 7;
    props.data_size   = sizeof(double);
    props.stride      = 1;

    config.padding_mode     = TRUNCATE;
    config.window_type      = RECTANGULAR;
    config.window_length    = 3;
    config.window_overlap   = 2;
    config.transform_length = 3;

    input = std::vector<double>{1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0};

    time  = std::vector<double>{1.0, 2.0, 3.0, 4.0, 5.0};
    freq  = std::vector<double>{0, 1.0 / 3.0};
    power = std::vector<double>{12.0, 2.0, 27.0, 2.0, 48.0, 2.0, 75.0, 2.0, 108.0, 2.0};
    phase = std::vector<double>{0.0, 2.618, 0.0, 2.618, 0.0, 2.618, 0.0, 2.618, 0.0, 2.618};

    stft = spectrogram_create(&props, &config);
    spectrogram_execute(stft, input.data());
}

void STFT_Test_3::SetUp() {
    props.sample_rate = M_PI;
    props.num_samples = 10;
    props.data_size   = sizeof(double);
    props.stride      = 1;

    config.padding_mode     = TRUNCATE;
    config.window_type      = RECTANGULAR;
    config.window_length    = 4;
    config.window_overlap   = 1;
    config.transform_length = 4;

    input = std::vector<double>{1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};

    time  = std::vector<double>{0.47746, 1.4324, 2.3873};
    freq  = std::vector<double>{0, M_PI / 4.0, M_PI / 2.0};
    power = std::vector<double>{7.9577, 1.2732, 0.3183, 38.5155, 1.2732, 0.3183, 91.9916, 1.2732, 0.3183};
    phase = std::vector<double>{0, 2.3562, 0, 0, 2.3562, 0, 0, 2.3562, 0};

    stft = spectrogram_create(&props, &config);
    spectrogram_execute(stft, input.data());
}

void STFT_Test_4::SetUp() {
    props.sample_rate = 10;
    props.num_samples = 18;
    props.data_size   = sizeof(double);
    props.stride      = 1;

    config.padding_mode     = TRUNCATE;
    config.window_type      = HAMMING;
    config.window_length    = 6;
    config.window_overlap   = 3;
    config.transform_length = 6;

    // Noisy 2.5 Hz signal
    input = std::vector<double>{0.655098, 1.162612,  0.118998, -0.501636, 0.959744, 1.340386,
                                0.585268, -0.776188, 0.751267, 1.255095,  0.505957, -0.300923,
                                0.890903, 1.959291,  0.547216, -0.861376, 0.149294, 1.257508};

    time  = std::vector<double>{0.2500, 0.5500, 0.8500, 1.1500, 1.4500};
    freq  = std::vector<double>{0, 1.6667, 3.3333, 5.0000};
    power = std::vector<double>{0.022942, 0.031642, 0.117803, 0.009019, 0.171276, 0.322919, 0.114042,
                                0.000697, 0.152259, 0.294051, 0.128774, 0.000883, 0.141962, 0.131140,
                                0.140239, 0.009721, 0.027941, 0.199963, 0.210372, 0.013773};
    phase = std::vector<double>{0,      -0.1136, -2.6167, 0,      0,       -2.0363, 1.7501, 0,       0,       -3.0426,
                                0.6256, 0,       0,       2.3869, -1.0981, 0,       0,      -0.7533, -2.9982, 0};

    stft = spectrogram_create(&props, &config);
    spectrogram_execute(stft, input.data());
}

void STFT_Test_5::SetUp() {
    props.sample_rate = 10;
    props.num_samples = 18;
    props.data_size   = sizeof(double);
    props.stride      = 2;

    config.padding_mode     = TRUNCATE;
    config.window_type      = HAMMING;
    config.window_length    = 6;
    config.window_overlap   = 3;
    config.transform_length = 6;

    // Noisy 2.5 Hz signal
    input = std::vector<double>{0.0, 0.655098,  0.0, 1.162612,  0.0, 0.118998,  0.0, -0.501636, 0.0, 0.959744,
                                0.0, 1.340386,  0.0, 0.585268,  0.0, -0.776188, 0.0, 0.751267,  0.0, 1.255095,
                                0.0, 0.505957,  0.0, -0.300923, 0.0, 0.890903,  0.0, 1.959291,  0.0, 0.547216,
                                0.0, -0.861376, 0.0, 0.149294,  0.0, 1.257508};

    time  = std::vector<double>{0.2500, 0.5500, 0.8500, 1.1500, 1.4500};
    freq  = std::vector<double>{0, 1.6667, 3.3333, 5.0000};
    power = std::vector<double>{0.022942, 0.031642, 0.117803, 0.009019, 0.171276, 0.322919, 0.114042,
                                0.000697, 0.152259, 0.294051, 0.128774, 0.000883, 0.141962, 0.131140,
                                0.140239, 0.009721, 0.027941, 0.199963, 0.210372, 0.013773};
    phase = std::vector<double>{0,      -0.1136, -2.6167, 0,      0,       -2.0363, 1.7501, 0,       0,       -3.0426,
                                0.6256, 0,       0,       2.3869, -1.0981, 0,       0,      -0.7533, -2.9982, 0};

    stft = spectrogram_create(&props, &config);
    spectrogram_execute(stft, input.data() + 1);
}

void STFT_Test_6::SetUp() {
    props.sample_rate = 10;
    props.num_samples = 18;
    props.data_size   = sizeof(double);
    props.stride      = 1;

    config.padding_mode     = TRUNCATE;
    config.window_type      = HAMMING;
    config.window_length    = 6;
    config.window_overlap   = 3;
    config.transform_length = 16;

    // Noisy 2.5 Hz signal
    input = std::vector<double>{0.655098, 1.162612,  0.118998, -0.501636, 0.959744, 1.340386,
                                0.585268, -0.776188, 0.751267, 1.255095,  0.505957, -0.300923,
                                0.890903, 1.959291,  0.547216, -0.861376, 0.149294, 1.257508};

    time  = std::vector<double>{0.2500, 0.5500, 0.8500, 1.1500, 1.4500};
    freq  = std::vector<double>{0, 0.6250, 1.2500, 1.8750, 2.5000, 3.1250, 3.7500, 4.3750, 5.0000};
    power = std::vector<double>{0.022942, 0.022594, 0.005996, 0.053877, 0.119506, 0.128965, 0.084413, 0.036440,
                                0.009019, 0.171276, 0.340821, 0.334704, 0.312032, 0.249483, 0.148732, 0.055753,
                                0.010328, 0.000697, 0.152259, 0.304614, 0.302260, 0.285912, 0.238230, 0.158233,
                                0.074325, 0.019199, 0.000883, 0.141962, 0.230381, 0.147340, 0.136594, 0.168549,
                                0.157374, 0.096510, 0.039734, 0.009721, 0.027941, 0.067262, 0.128601, 0.236184,
                                0.295900, 0.244919, 0.136515, 0.054366, 0.013773};
    phase = std::vector<double>{0, -0.7565, -0.1360, -0.3521, -1.2602, -2.2674, 2.9402,  1.7012,  0,
                                0, -0.7459, -1.5064, -2.3119, 3.0893,  2.1065,  0.9750,  -0.4811, 0,
                                0, -1.1614, -2.3040, 2.8814,  1.8536,  0.9150,  0.0905,  -0.5418, 0,
                                0, -1.3072, -2.7969, 1.8532,  0.4471,  -0.7339, -1.7907, -2.6837, 0,
                                0, -0.1093, -0.3824, -0.9830, -1.7825, -2.6826, 2.6275,  1.5098,  0};

    stft = spectrogram_create(&props, &config);
    spectrogram_execute(stft, input.data());
}
