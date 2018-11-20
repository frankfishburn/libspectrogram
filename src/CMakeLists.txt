set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
set(BUILD_SHARED ON CACHE BOOL "Build the shared library")
set(BUILD_STATIC ON CACHE BOOL "Build the static library")

# Build shared library
if(BUILD_SHARED)
add_library(spectrogram_shared SHARED spectrogram.cpp stft.cpp)
target_include_directories(spectrogram_shared PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(spectrogram_shared PUBLIC "${FFTW_INCLUDE_FIR}")
target_link_libraries(spectrogram_shared ${FFTW_LIBS})
target_link_libraries(spectrogram_shared ${FFTWF_LIBS})
set_target_properties(spectrogram_shared PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(spectrogram_shared PROPERTIES OUTPUT_NAME spectrogram)
install(TARGETS spectrogram_shared DESTINATION lib)
endif()

# Build static library
if(BUILD_STATIC)
add_library(spectrogram_static STATIC spectrogram.cpp stft.cpp)
set_property(TARGET spectrogram_static PROPERTY POSITION_INDEPENDENT_CODE 1)
target_include_directories(spectrogram_static PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(spectrogram_static PUBLIC "${FFTW_INCLUDE_DIR}")
target_link_libraries(spectrogram_static ${FFTW_LIBS})
target_link_libraries(spectrogram_static ${FFTWF_LIBS})
set_target_properties(spectrogram_static PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(spectrogram_static PROPERTIES OUTPUT_NAME spectrogram)
install(TARGETS spectrogram_static DESTINATION lib)
endif()

# Set install paths
install(FILES "${PROJECT_SOURCE_DIR}/include/spectrogram.h" DESTINATION include)
