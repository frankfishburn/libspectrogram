set(BUILD_MATLAB OFF CACHE BOOL "Build matlab binding")

if(BUILD_MATLAB)

    set(CMAKE_FIND_LIBRARY_SUFFIXES ".so;.a")
    find_package(Matlab COMPONENTS MX_LIBRARY MEX_COMPILER)

    if(NOT Matlab_FOUND)
        message(FATAL_ERROR "MATLAB is needed to build the bindings.")
    endif()

    set(MATLAB_INSTALL_PATH "$ENV{HOME}/Documents/MATLAB/libspectrogram" CACHE FILEPATH "Location to place Matlab bindings")

    matlab_add_mex(
        NAME spectrogram_mex
        SHARED SRC libspectrogram.cpp 
        OUTPUT_NAME libspectrogram_mex 
        LINK_TO spectrogram_static ${FFTW_LIBS} ${FFTWF_LIBS})

    set_target_properties(spectrogram_mex PROPERTIES COMPILE_FLAGS "-fvisibility=default")
    target_include_directories(spectrogram_mex PUBLIC "${PROJECT_SOURCE_DIR}/include")
    target_include_directories(spectrogram_mex PUBLIC "${FFTW_INCLUDE_DIR}")

    install(TARGETS spectrogram_mex DESTINATION "${MATLAB_INSTALL_PATH}")
    install(FILES "${PROJECT_SOURCE_DIR}/bindings/matlab/libspectrogram.m" DESTINATION "${MATLAB_INSTALL_PATH}")
    install(FILES "${PROJECT_SOURCE_DIR}/bindings/matlab/functionSignatures.json" DESTINATION "${MATLAB_INSTALL_PATH}")

endif()
