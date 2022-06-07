#include "audio.h"

int paStreamCallback(const void* input, void* output,
                            unsigned long samplesPerFrame,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void* userData)
{
    paData* data = (paData*)userData;
    struct FFT_s* frequencies_array = data->frequencies_array;
    float *out = (float*)output;

    static unsigned long n = 0;
    for(unsigned long i = 0; i < samplesPerFrame; i++, n++){
      float v = sin ( 2 * frequencies_array[data->idx].frequency * M_PI * n /1024.0);
       *out++ = v;  /* left */
       *out++ = v;  /* right */
   }

    return paContinue;
}