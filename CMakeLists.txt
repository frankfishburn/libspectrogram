cmake_minimum_required (VERSION 3.12.0)
project(spectrogram VERSION 0.01 DESCRIPTION "Library for computing time-frequency representations of time series data.")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -Werror -pedantic")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

enable_testing()

# Find FFTW
find_path(FFTW_INCLUDE_DIR fftw3.h)
find_library(FFTW_LIBS REQUIRED NAMES fftw3)
find_library(FFTWF_LIBS REQUIRED NAMES fftw3f)
include_directories(${FFTW_INCLUDE_DIR})
message(STATUS "FFTW include directory: ${FFTW_INCLUDE_DIR}")
message(STATUS "FFTW float library: ${FFTWF_LIBS}")
message(STATUS "FFTW double library: ${FFTW_LIBS}")

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(examples)
add_subdirectory(docs)
add_subdirectory(bindings)
