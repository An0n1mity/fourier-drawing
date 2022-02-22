#ifndef COMPLEX_H
#define COMPLEX_H
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

struct Complex_s
{
	double m_real;
	double m_imaginary;
};

struct ComplexList_s
{
	struct Complex_s* m_complex;
	struct ComplexList_s* m_nextComplexList;
};

struct Complex_s* createComplex(double p_real, double p_imaginary);

struct ComplexList_s* createComplexList(struct Complex_s* p_headComplex);

void addObjectComplexList(struct ComplexList_s** p_complexList, struct Complex_s* p_complex);

void destroyComplex(struct Complex_s* p_complex);

void destroyComplexList(struct ComplexList_s* p_complexList);

inline double getRealPart(struct Complex_s* p_complex)
{
	return p_complex->m_real;
}

inline double getImaginaryPart(struct Complex_s* p_complex)
{
	return p_complex->m_imaginary;
}

inline struct Complex_s* addComplex(struct Complex_s* p_first, struct Complex_s* p_second)
{
	return createComplex(p_first->m_real + p_second->m_real, p_first->m_imaginary + p_second->m_imaginary);
}

struct Complex_s* addComplexList(struct ComplexList_s* p_complexList);

inline struct Complex_s* subComplex(struct Complex_s* p_first, struct Complex_s* p_second)
{
	return createComplex(p_first->m_real - p_second->m_real, p_first->m_imaginary - p_second->m_imaginary);
}

struct Complex_s* subComplexList(struct ComplexList_s* p_complexList);

inline struct Complex_s* multiplyComplex(struct Complex_s* p_first, struct Complex_s* p_second)
{
	return createComplex((p_first->m_real * p_second->m_real) - (p_first->m_imaginary * p_second->m_imaginary),
		(p_first->m_real * p_second->m_imaginary) + (p_second->m_real * p_first->m_imaginary));
}

struct Complex_s* multiplyComplexList(struct ComplexList_s* p_complexList);

struct Complex_s* divideComplex(struct Complex_s* p_first, struct Complex_s* p_second);

struct Complex_s* divideComplexList(struct ComplexList_s* p_complexList);

inline struct Complex_s* getExponentialComplex(struct Complex_s* p_exponent)
{
	return createComplex(cos(p_exponent->m_real), sin(p_exponent->m_imaginary));
}

inline void printComplex(struct Complex_s* p_complex, int p_precision)
{
	return printf("%.*lf + %.*lfi\n", p_precision, p_complex->m_real, p_precision, p_complex->m_imaginary);
}
#endif // COMPLEX_H