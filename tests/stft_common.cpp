#include "stft_common.h"

double MaxError(std::vector<double> v1, std::vector<double> v2){
    
    if (v1.size() != v2.size())
        return std::numeric_limits<double>::infinity();
    
    double error = 0;
    for (int i=0; i<(int)v1.size(); i++)
        error = std::max( error , std::abs(v1[i]-v2[i]) );
    
    return error;
}

void Fixture_4x2x1_1Hz::SetUp(){
    
    props.sample_rate = 1;
    props.num_samples = 4;
    props.data_size = sizeof(double);

    config.padding_mode = TRUNCATE;
    config.window_type = RECTANGULAR;
    config.window_length = 2;
    config.window_overlap = 1;

    mystft = new STFT(props, config);
    
}

void Fixture_4x2x1_1Hz::TearDown() {

    delete mystft;

}

void Fixture_7x3x2_1Hz::SetUp(){
    
    props.sample_rate = 1;   
    props.num_samples = 7;
    props.data_size = sizeof(double);

    config.padding_mode = TRUNCATE;
    config.window_type = RECTANGULAR;
    config.window_length = 3;
    config.window_overlap = 2;

    mystft = new STFT(props, config);
    
}

void Fixture_7x3x2_1Hz::TearDown() {

    delete mystft;

}

void Fixture_10x4x1_piHz::SetUp(){
    
    props.sample_rate = M_PI;   
    props.num_samples = 10;
    props.data_size = sizeof(double);

    config.padding_mode = TRUNCATE;
    config.window_type = RECTANGULAR;
    config.window_length = 4;
    config.window_overlap = 1;

    mystft = new STFT(props, config);
    
}

void Fixture_10x4x1_piHz::TearDown() {

    delete mystft;

}

void Fixture_18x6x3_10Hz::SetUp(){
    
    props.sample_rate = 10;
    props.num_samples = 18;
    props.data_size = sizeof(double);

    config.padding_mode = TRUNCATE;
    config.window_type = HAMMING;
    config.window_length = 6;
    config.window_overlap = 3;
    
    mystft = new STFT(props, config);
    
    // Noisy 2.5 Hz signal
    std::vector<double> signal = {0.655098, 1.162612, 0.118998, -0.501636, 0.959744, 1.340386, 
                                  0.585268, -0.776188, 0.751267, 1.255095, 0.505957, -0.300923, 
                                  0.890903, 1.959291, 0.547216, -0.861376, 0.149294, 1.257508};
    
    mystft->compute(signal.data());
    
}

void Fixture_18x6x3_10Hz::TearDown(){
    
    delete mystft;
    
}

void Fixture_16::SetUp(){
    
    props.sample_rate = 1;
    props.num_samples = 16;
    props.data_size = sizeof(double);

    config.padding_mode = TRUNCATE;
    config.window_length = 16;
    config.window_overlap = 1;

}

void Fixture_17::SetUp(){
    
    props.sample_rate = 1;
    props.num_samples = 17;
    props.data_size = sizeof(double);

    config.padding_mode = TRUNCATE;
    config.window_length = 17;
    config.window_overlap = 1;

}
