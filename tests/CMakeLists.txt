add_executable(stft_tests stft_common.cpp stft_constructor.cpp stft_window.cpp stft_power.cpp stft_phase.cpp)
target_include_directories(stft_tests PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(stft_tests PRIVATE "${PROJECT_SOURCE_DIR}/src")
target_link_libraries(stft_tests gtest_main)
target_link_libraries(stft_tests ${FFTW_LIBS})
target_link_libraries(stft_tests spectrogram)
add_test(NAME stft_tests COMMAND stft_tests)

