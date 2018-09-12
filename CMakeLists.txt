cmake_minimum_required (VERSION 3.11)
project(spectrogram VERSION 0.01 DESCRIPTION "Library for computing time-frequency representations of time series data.")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_subdirectory(src)


