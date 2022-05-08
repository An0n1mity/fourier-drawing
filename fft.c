//
// Created by biist on 02/05/2022.
//
#include "fft.h"

kiss_fft_cpx* GetComplexArrayFromPoints(ShapePoint* points, size_t nbpoints)
{
    kiss_fft_cpx* complex_array = malloc(sizeof(kiss_fft_cpx) * nbpoints);
    ShapePoint* point = points;

    double scale_x = 800.f/1920.f;
    double scale_y = 800.f/1080.f;

    for (int i = 0; i < nbpoints; ++i, point = point->np) {
        complex_array[i].r = point->x;//* scale_x;
        complex_array[i].i = point->y;// * scale_x;
    }

    return complex_array;
}

int CompareAmplitude(const void* A, const void* B)
{
    int amplitude_a = ((struct FFT_s*)A)->amplitude;
    int amplitude_b = ((struct FFT_s*)B)->amplitude;

    return amplitude_b - amplitude_a;
}

struct FFT_s* GetFFTOfComplexArray(kiss_fft_cpx* complex_array, size_t nbcomplex)
{

    struct FFT_s* fft = malloc(sizeof(struct FFT_s) * nbcomplex);
    kiss_fft_cfg cfg = kiss_fft_alloc( nbcomplex, 1 , NULL, NULL);
    kiss_fft_cpx* complex_fft_array = malloc(sizeof(kiss_fft_cpx)*nbcomplex);

    kiss_fft(cfg, complex_array, complex_fft_array);
    free(cfg);

    for (int i = 0; i < nbcomplex; ++i) {
        fft[i].amplitude = sqrtf(powf(complex_fft_array[i].r, 2) + powf(complex_fft_array[i].i, 2));
        fft[i].phase = atan2(complex_fft_array[i].i, complex_fft_array[i].r);
        fft[i].frequency = i;
    }

    qsort(fft, nbcomplex, sizeof(struct FFT_s), CompareAmplitude);
    free(complex_array);
    free(complex_fft_array);

    return fft;
}
