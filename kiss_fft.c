//
// Created by biist on 23/04/2022.
//
#include "kiss_fft.h"

/*  facbuf is populated by p1,m1,p2,m2, ...
    where
    p[i] * m[i] = m[i-1]
    m0 = n                  */
static
void kf_factor(int n, int *facbuf) {
    int p = 4;
    double floor_sqrt;
    floor_sqrt = floor(sqrt((double) n));

    /*factor out powers of 4, powers of 2, then any remaining primes */
    do {
        while (n % p) {
            switch (p) {
                case 4:
                    p = 2;
                    break;
                case 2:
                    p = 3;
                    break;
                default:
                    p += 2;
                    break;
            }
            if (p > floor_sqrt)
                p = n;          /* no more factors, skip to end */
        }
        n /= p;
        *facbuf++ = p;
        *facbuf++ = n;
    } while (n > 1);
}




/*
 *
 * User-callable function to allocate all necessary storage space for the fft.
 *
 * The return value is a contiguous block of memory, allocated with malloc.  As such,
 * It can be freed with free(), rather than a kiss_fft-specific function.
 * */
kiss_fft_cfg kiss_fft_alloc(int nfft, int inverse_fft, void *mem, size_t *lenmem) {
    kiss_fft_cfg st = NULL;
    size_t memneeded = sizeof(struct kiss_fft_state)
                       + sizeof(kiss_fft_cpx) * (nfft - 1); /* twiddle factors*/

    if (lenmem == NULL) {
        st = (kiss_fft_cfg) KISS_FFT_MALLOC(memneeded);
    } else {
        if (mem != NULL && *lenmem >= memneeded)
            st = (kiss_fft_cfg) mem;
        *lenmem = memneeded;
    }
    if (st) {
        int i;
        st->nfft = nfft;
        st->inverse = inverse_fft;

        for (i = 0; i < nfft; ++i) {
            double phase = -2.0 * M_PI * ((double)i) / ((double)nfft);
            if (st->inverse)
                phase *= -1;
            kf_cexp(st->twiddles + i, phase);
        }

        kf_factor(nfft, st->factors);
    }
    return st;
}


void kiss_fft_stride(kiss_fft_cfg st, const kiss_fft_cpx *fin, kiss_fft_cpx *fout, int in_stride) {
    int j;
    if (fin == fout) {
        //NOTE: this is not really an in-place FFT algorithm.
        //It just performs an out-of-place FFT into a temp buffer
        kiss_fft_cpx *tmpbuf = (kiss_fft_cpx *) malloc(sizeof(kiss_fft_cpx) * st->nfft);
        kf_work(tmpbuf, fin, 1, in_stride, st->factors, st);
        memcpy(fout, tmpbuf, sizeof(kiss_fft_cpx) * st->nfft);
        free(tmpbuf);
    } else {
        kf_work(fout, fin, 1, in_stride, st->factors, st);
    }
    if (st->inverse) {
        for (j = 0; j < st->nfft; j++) {
            fout[j].r /= st->nfft;
            fout[j].i /= st->nfft;
        }
    }
}


void kiss_fft(kiss_fft_cfg cfg, const kiss_fft_cpx *fin, kiss_fft_cpx *fout) {
    kiss_fft_stride(cfg, fin, fout, 1);
}


int kiss_fft_next_fast_size(int n) {
    while (1) {
        int m = n;
        while ((m % 2) == 0) m /= 2;
        while ((m % 3) == 0) m /= 3;
        while ((m % 5) == 0) m /= 5;
        if (m <= 1)
            break; /* n is completely factorable by twos, threes, and fives */
        n++;
    }
    return n;
}