//
// Created by biist on 02/05/2022.
//

#ifndef CYCLOIDE_FFT_H
#define CYCLOIDE_FFT_H

#include "shapes.h"

struct FFT_s{
    double amplitude;
    double phase;
    size_t frequency;
};

/*
 * Return the coefficent of the DFT from abstract shapes stack
 */
kiss_fft_cpx* GetComplexArrayFromPoints(ShapePoint* points, size_t nbpoints);
struct FFT_s* GetFFTOfComplexArray(kiss_fft_cpx* complex_array, size_t nbcomplex);

#endif //CYCLOIDE_FFT_H
