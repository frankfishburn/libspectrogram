find_package(Matlab COMPONENTS MX_LIBRARY MEX_COMPILER)

if(Matlab_FOUND)

    matlab_add_mex(
        NAME matlab
        MODULE SRC libspectrogram.cpp 
        OUTPUT_NAME libspectrogram 
        LINK_TO spectrogram_static ${FFTW_LIBS} ${FFTWF_LIBS})

    target_include_directories(matlab PUBLIC "${PROJECT_SOURCE_DIR}/include")

endif()
