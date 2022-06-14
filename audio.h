#ifndef AUDIO_H
#define AUDIO_H

#include <portaudio.h>
#include "fft.h"
#define FRAME_SIZE    1024
#define SAMPLE_RATE  (44100)
#define FRAMES_PER_BUFFER (512)
#define NUM_SECONDS     (5)
#define NUM_CHANNELS    (2)


/* Select sample format. */
#define PA_SAMPLE_TYPE  paFloat32
typedef float SAMPLE;
#define SAMPLE_SILENCE  (0.0f)
#define PRINTF_S_FORMAT "%.8f"


typedef struct {
    struct FFT_s* frequencies_array;
    size_t idx;
} paData;

int paStreamCallback(const void* input, void* output,
                            unsigned long samplesPerFrame,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void* userData);


#endif
