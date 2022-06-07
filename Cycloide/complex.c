#include "complex.h"

struct ComplexList_s* createComplexList(struct Complex_s* p_headComplex)
{
	struct ComplexList_s* complexList = (struct ComplexList_s*)calloc(1, sizeof(struct ComplexList_s));
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
	if ((*p_complexList)->m_nextComplexList)
		addObjectComplexList(&(*p_complexList)->m_nextComplexList, p_complex);
	else
		(*p_complexList)->m_nextComplexList = createComplexList(p_complex);
}
void destroyComplexList(struct ComplexList_s* p_complexList)
{
	if (!p_complexList)
		return;
	if (p_complexList->m_nextComplexList)
		destroyComplexList(p_complexList->m_nextComplexList);
	p_complexList->m_complex = NULL;
	free(p_complexList);
	p_complexList = NULL;
}

struct Complex_s addComplexList(struct ComplexList_s* p_complexList)
{
	if (!p_complexList)
	{
		printf("ERROR in addComplexList : p_complexList is NULL\n");
		return createComplex(0, 0);
	}
	if (!p_complexList->m_nextComplexList)
		return (*p_complexList->m_complex);
	struct ComplexList_s* index = p_complexList->m_nextComplexList;
	double real = p_complexList->m_complex->m_real;
	double imaginary = p_complexList->m_complex->m_imaginary;
	while (index)
	{
		real += index->m_complex->m_real;
		imaginary += index->m_complex->m_imaginary;
		index = index->m_nextComplexList;
	}
	return createComplex(real, imaginary);
}

struct Complex_s subComplexList(struct ComplexList_s* p_complexList)
{
	if (!p_complexList)
	{
		printf("ERROR in subComplexList : p_complexList is NULL\n");
		return createComplex(0, 0);
	}

	if (!p_complexList->m_nextComplexList)
		return (*p_complexList->m_complex);

	struct ComplexList_s* index = p_complexList->m_nextComplexList;
	double real = p_complexList->m_complex->m_real;
	double imaginary = p_complexList->m_complex->m_imaginary;
	while (index)
	{
		real -= index->m_complex->m_real;
		imaginary -= index->m_complex->m_imaginary;
		index = index->m_nextComplexList;
	}
	return createComplex(real, imaginary);
}

struct Complex_s multiplyComplexList(struct ComplexList_s* p_complexList)
{
	if (!p_complexList)
	{
		printf("ERROR in multiplyComplexList : p_complexList is NULL\n");
		return createComplex(0, 0);
	}

	if (!p_complexList->m_nextComplexList)
		return (*p_complexList->m_complex);

	double real = p_complexList->m_complex->m_real;
	double imaginary = p_complexList->m_complex->m_imaginary;
	double temp = real;
	struct ComplexList_s* index = p_complexList->m_nextComplexList;

	while (index)
	{
		real = (real * index->m_complex->m_real) - (imaginary * index->m_complex->m_imaginary);
		imaginary = (temp * index->m_complex->m_imaginary) + (index->m_complex->m_real * imaginary);
		temp = real;
		index = index->m_nextComplexList;
	}
	return createComplex(real, imaginary);
}

struct Complex_s divideComplex(struct Complex_s p_first, struct Complex_s p_second)
{
	double denominator = (p_second.m_real * p_second.m_real) + (p_second.m_imaginary * p_second.m_imaginary);
	if (denominator == 0)
	{
		printf("How did you divide by zero ? in divideComplex()\n");
		return createComplex(0, 0);
	}
	double real = ((p_first.m_real * p_second.m_real) + (p_first.m_imaginary * p_second.m_imaginary)) / denominator;
	double imaginary = ((p_first.m_imaginary*p_second.m_real) - (p_first.m_real*p_second.m_imaginary)) / denominator;
	return createComplex(real, imaginary);
}

struct Complex_s divideComplexList(struct ComplexList_s* p_complexList)
{
	if (!p_complexList)
	{
		printf("ERROR in divideComplexList : p_complexList is NULL\n");
		return createComplex(0, 0);
	}
	if (!p_complexList->m_nextComplexList)
		return (*p_complexList->m_complex);
	struct Complex_s denominator = multiplyComplexList(p_complexList->m_nextComplexList);
	struct Complex_s result = divideComplex((*p_complexList->m_complex), denominator);
	return result;
}

void printComplexList(struct ComplexList_s* p_complexList, int p_precision)
{
	if (!p_complexList)
	{
		printf("ERROR in printComplexList() : p_complexList is NULL\n");
		return;
	}
	struct ComplexList_s* index = p_complexList;
	while (index)
	{
		printComplex((*index->m_complex), p_precision);
		index = index->m_nextComplexList;
	}
}