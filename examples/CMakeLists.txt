set(BUILD_EXAMPLES ON CACHE BOOL "Build example applications")

if(BUILD_EXAMPLES)
add_executable(simple simple.c)
target_include_directories(simple PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(simple spectrogram_shared)
target_link_libraries(simple ${FFTW_LIBS})
target_link_libraries(simple ${FFTWF_LIBS})
endif()
