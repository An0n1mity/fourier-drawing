#include "complex.h"

struct Complex_s* createComplex(double p_real, double p_imaginary)
{
	struct Complex_s* complex = (struct Complex_s*)calloc(1, sizeof(struct Complex_s));
	if (!complex)
	{
		printf("Couldn't create complex in createComplex()\n");
		return NULL;
	}
	complex->m_real = p_real;
	complex->m_imaginary = p_imaginary;
	return complex;
}

struct ComplexList_s* createComplexList(struct Complex_s* p_headComplex)
{
	struct ComplexList_s* complexList = (struct Complex_s*)calloc(1, sizeof(struct Complex_s));
	if (!complexList)
	{
		printf("Couldn't create complexList in createComplexList()\n");
		return NULL;
	}
	complexList->m_complex = p_headComplex;
	return complexList;
}

void addObjectComplexList(struct ComplexList_s** p_complexList, struct Complex_s* p_complex)
{
	if (!p_complexList || !(*p_complexList))
	{
		printf("Cannot add a NULL to a ComplexList in addObjectComplexList\n");
		return;
	}
	(*p_complexList)->m_nextComplex = p_complex;
	return;
}

void destroyComplex(struct Complex_s* p_complex)
{
	free(p_complex);
}

void destroyComplexList(struct ComplexList_s* p_complexList)
{
	if (p_complexList->m_complex)
		destroyComplexList(p_complexList->m_nextComplex);
	else
		destroyComplex(p_complexList->m_complex);
}

struct Complex_s* addComplexList(struct ComplexList_s* p_complexList)
{
	struct Complex_s* complex = createComplex(0, 0);
	if (p_complexList->m_nextComplex)
	complex = addComplex(p_complexList->m_complex, addComplexList(p_complexList->m_nextComplex));
	return complex;
}

struct Complex_s* subComplexList(struct ComplexList_s* p_complexList)
{
	struct Complex_s* sumComplex;
	if (p_complexList->m_nextComplex)
		sumComplex = addComplexList(p_complexList->m_nextComplex);
	return subComplex(p_complexList->m_complex, sumComplex);
}

struct Complex_s* multiplyComplexList(struct ComplexList_s* p_complexList)
{
	struct Complex_s* complex = createComplex(1, 1);
	if (p_complexList->m_nextComplex)
		complex = multiplyComplex(p_complexList->m_complex, multiplyComplexList(p_complexList->m_nextComplex));
	return complex;
}

struct Complex_s* divideComplex(struct Complex_s* p_first, struct Complex_s* p_second)
{
	struct Complex_s* modifiedSecond = createComplex(p_second->m_real, -1 * p_second->m_imaginary);
	struct Complex_s* numerator = multiplyComplex(p_first, modifiedSecond);
	double denominator = p_second->m_real * p_second->m_real - (1 * p_second->m_imaginary * p_second->m_imaginary);
	destroyComplex(modifiedSecond);

	if (denominator == 0)
		return numerator;
	numerator->m_real /= denominator;
	numerator->m_imaginary /= denominator;
	return numerator;
}

struct Complex_s* divideComplexList(struct ComplexList_s* p_complexList)
{
	struct Complex_s* productComplex = createComplex(1, 1);
	if (p_complexList->m_nextComplex)
		productComplex = multiplyComplex(p_complexList->m_complex, multiplyComplexList(p_complexList->m_nextComplex));
	return divideComplex(p_complexList->m_complex, productComplex);
}