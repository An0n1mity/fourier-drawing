#ifndef AUDIO_H
#define AUDIO_H

#include <portaudio.h>
#include "fft.h"
#define FRAME_SIZE 1024
#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 512
#define NUM_CHANNELS 2

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