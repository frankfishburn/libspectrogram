# Build object library
add_library(spectrogram_object OBJECT spectrogram.cpp stft.cpp)
set_property(TARGET spectrogram_object PROPERTY POSITION_INDEPENDENT_CODE 1)
target_include_directories(spectrogram_object PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(spectrogram_object PRIVATE "${PROJECT_SOURCE_DIR}/lib")
target_link_libraries(spectrogram_object ${FFTW_LIBS})
target_link_libraries(spectrogram_object ${FFTWF_LIBS})

# Build shared and static libraries
add_library(spectrogram_shared SHARED $<TARGET_OBJECTS:spectrogram_object>)
add_library(spectrogram_static STATIC $<TARGET_OBJECTS:spectrogram_object>)
set_target_properties(spectrogram_shared PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(spectrogram_static PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(spectrogram_shared PROPERTIES OUTPUT_NAME spectrogram)
set_target_properties(spectrogram_static PROPERTIES OUTPUT_NAME spectrogram)
