#include "spectrogram.h"
#include <stdlib.h>
#include "stft.h"

#if defined _WIN32 || defined __CYGWIN__
#ifdef BUILDING_DLL
#ifdef __GNUC__
#define DLL_PUBLIC __attribute__((dllexport))
#else
#define DLL_PUBLIC __declspec(dllexport)  // Note: actually gcc seems to also supports this syntax.
#endif
#else
#ifdef __GNUC__
#define DLL_PUBLIC __attribute__((dllimport))
#else
#define DLL_PUBLIC __declspec(dllimport)  // Note: actually gcc seems to also supports this syntax.
#endif
#endif
#define DLL_LOCAL
#else
#if __GNUC__ >= 4
#define DLL_PUBLIC __attribute__((visibility("default")))
#define DLL_LOCAL __attribute__((visibility("hidden")))
#else
#define DLL_PUBLIC
#define DLL_LOCAL
#endif
#endif

// Create
DLL_PUBLIC SpectrogramTransform* spectrogram_create(SpectrogramInput* props, SpectrogramConfig* config) {
    return reinterpret_cast<SpectrogramTransform*>(new STFT(*props, *config));
}

// Execute
DLL_PUBLIC void spectrogram_execute(SpectrogramTransform* transform, void* input) {
    STFT* mystft = reinterpret_cast<STFT*>(transform);
    mystft->compute(input);
}

// Get output parameters
DLL_PUBLIC size_t spectrogram_get_timelen(SpectrogramTransform* transform) {
    STFT* mystft = reinterpret_cast<STFT*>(transform);
    return mystft->num_windows();
}

DLL_PUBLIC size_t spectrogram_get_freqlen(SpectrogramTransform* transform) {
    STFT* mystft = reinterpret_cast<STFT*>(transform);
    return mystft->num_frequencies();
}

// Get outputs
DLL_PUBLIC void spectrogram_get_time(SpectrogramTransform* transform, void* time) {
    STFT* mystft = reinterpret_cast<STFT*>(transform);

    if (mystft->data_size() == sizeof(float)) {
        mystft->get_time<float>(time);

    } else if (mystft->data_size() == sizeof(double)) {
        mystft->get_time<double>(time);
    }
}

DLL_PUBLIC void spectrogram_get_freq(SpectrogramTransform* transform, void* freq) {
    STFT* mystft = reinterpret_cast<STFT*>(transform);

    if (mystft->data_size() == sizeof(float)) {
        mystft->get_freq<float>(freq);

    } else if (mystft->data_size() == sizeof(double)) {
        mystft->get_freq<double>(freq);
    }
}

DLL_PUBLIC void spectrogram_get_power(SpectrogramTransform* transform, void* power) {
    STFT* mystft = reinterpret_cast<STFT*>(transform);

    if (mystft->data_size() == sizeof(float)) {
        mystft->get_power<float>(power);

    } else if (mystft->data_size() == sizeof(double)) {
        mystft->get_power<double>(power);
    }
}

DLL_PUBLIC void spectrogram_get_phase(SpectrogramTransform* transform, void* phase) {
    STFT* mystft = reinterpret_cast<STFT*>(transform);

    if (mystft->data_size() == sizeof(float)) {
        mystft->get_phase<float>(phase);

    } else if (mystft->data_size() == sizeof(double)) {
        mystft->get_phase<double>(phase);
    }
}

DLL_PUBLIC void spectrogram_get_power_periodogram(SpectrogramTransform* transform, void* power) {
    STFT* mystft = reinterpret_cast<STFT*>(transform);

    if (mystft->data_size() == sizeof(float)) {
        mystft->get_power_periodogram<float>(power);

    } else if (mystft->data_size() == sizeof(double)) {
        mystft->get_power_periodogram<double>(power);
    }
}

DLL_PUBLIC void spectrogram_get_phase_periodogram(SpectrogramTransform* transform, void* phase) {
    STFT* mystft = reinterpret_cast<STFT*>(transform);

    if (mystft->data_size() == sizeof(float)) {
        mystft->get_phase_periodogram<float>(phase);

    } else if (mystft->data_size() == sizeof(double)) {
        mystft->get_phase_periodogram<double>(phase);
    }
}

// Destroy
DLL_PUBLIC void spectrogram_destroy(SpectrogramTransform* transform) {
    STFT* mystft = reinterpret_cast<STFT*>(transform);
    delete mystft;
}
