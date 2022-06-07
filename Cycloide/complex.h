#ifndef COMPLEX_H
#define COMPLEX_H
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

extern double g_timeScale;	///< scale to draw more or less fast
extern int g_nbCircles;		///< count of nb of circles

struct Complex_s
{
	double m_real;		///< real value
	double m_imaginary; ///< imaginary value
};
  
struct ComplexList_s
{
	struct Complex_s* m_complex;			///< currentComplex
	struct ComplexList_s* m_nextComplexList;///< next Complex in the list
};

/**
 * @brief create a complex number
 * \param[in] p_real real value
 * \param[in] p_imaginary imaginary value
 * \return complex element
 */
__inline struct Complex_s createComplex(double p_real, double p_imaginary)
{
	return (struct Complex_s) {p_real, p_imaginary};
}


/**
 * @brief create a complex number list
 * \param[in] p_headComplex new complex list Head
 * \return complex list
 */
struct ComplexList_s* createComplexList(struct Complex_s* p_headComplex);

/**
 * @brief add an object to a complexList
 * \param[in/out] p_complexList the list where it should be inserted
 * \param[in] p_complex the number to insert
 */
void addObjectComplexList(struct ComplexList_s** p_complexList, struct Complex_s* p_complex);

/**
 * @brief destroy a complex list
 * \param[in] p_complexList the list
 */
void destroyComplexList(struct ComplexList_s* p_complexList);

/**
 * @brief add two complex numbers
 * \param[in] p_first the first complex number
 * \param[in] p_second the second complex number
 * \return complex result
 */
__inline struct Complex_s addComplex(struct Complex_s p_first, struct Complex_s p_second)
{
	return createComplex(p_first.m_real + p_second.m_real, p_first.m_imaginary + p_second.m_imaginary);
}

/**
 * @brief add together a list of complex numbers
 * \param[in] p_complexList the list of complex numbers
 * \return complex result
 */
struct Complex_s addComplexList(struct ComplexList_s* p_complexList);

/**
 * @brief sub a complex from another complex
 * \param[in] p_first the substracted element
 * \param[in] p_second the substracted value
 * \return complex result
 */
__inline struct Complex_s subComplex(struct Complex_s p_first, struct Complex_s p_second)
{
	return createComplex(p_first.m_real - p_second.m_real, p_first.m_imaginary - p_second.m_imaginary);
}

/**
 * @brief sub a list of complex number from a complex number
 * \param[in] p_complexList
 * \return complex result
 */
struct Complex_s subComplexList(struct ComplexList_s* p_complexList);

/**
 * @brief multiply two complex numbers
 * \param[in] p_first the first complex
 * \param[in] p_second the second complex
 * \return complex result of product
 */
__inline struct Complex_s multiplyComplex(struct Complex_s p_first, struct Complex_s p_second)
{
	return createComplex((p_first.m_real * p_second.m_real) - (p_first.m_imaginary * p_second.m_imaginary),
		(p_first.m_real * p_second.m_imaginary) + (p_second.m_real * p_first.m_imaginary));
}

/**
 * @brief multiply together a list of complex numbers
 * \param[in] p_complexList
 * \return complex result
 */
struct Complex_s multiplyComplexList(struct ComplexList_s* p_complexList);

/**
 * @brief divide a complex number by another
 * \param[in] p_first the divided complex
 * \param[in] p_second the value to divide
 * \return complex result
 */
struct Complex_s divideComplex(struct Complex_s p_first, struct Complex_s p_second);

/**
 * @brief divide a list of complex number from the top
 * \param[in] p_complexList	the list of complex numbers
 * \return complex result
 */
struct Complex_s divideComplexList(struct ComplexList_s* p_complexList);

/**
 * @brief get the value of exp(a+ib)
 * \param[in] p_exponent the exponential function
 * \return complex result
 */
__inline struct Complex_s getExponentialComplex(struct Complex_s p_exponent)
{
	return createComplex(expf(p_exponent.m_real) * cosf(p_exponent.m_imaginary), expf(p_exponent.m_real) * sinf(p_exponent.m_imaginary));
}

/**
 * @brief print the value of a complex number
 * \param[in] p_complex the complex number
 * \param[in] p_precision the number of decimals after the comma
 */
__inline void printComplex(struct Complex_s p_complex, int p_precision)
{
	printf("%.*lf + %.*lfi\n", p_precision, p_complex.m_real, p_precision, p_complex.m_imaginary);
}

/**
 * @brief print the value of a list of complex
 * \param[in] p_complexList the list of complex numbers
 * \param[in] p_precision the number of decimals after the comma
 */
void printComplexList(struct ComplexList_s* p_complexList, int p_precision);

#endif // COMPLEX_H