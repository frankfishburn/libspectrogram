add_executable(simple simple.c)
target_include_directories(simple PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(simple fftw3 spectrogram)