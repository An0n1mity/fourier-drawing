#include "Fourier.h"

struct Circle_s createCircle(int p_index, SHAPE_Point p_position, struct Complex_s p_coeff)
{
	struct Circle_s circle;
	circle.m_index = p_index;
	circle.m_position = p_position;
	circle.m_coeff = p_coeff;
	circle.m_nextCircle = NULL;
	return circle;
}

void addCircleList(struct Circle_s** p_list, struct Circle_s* p_toAdd)
{
	p_toAdd->m_nextCircle = (*p_list);
	(*p_list) = p_toAdd;
}

SHAPE_Point getPositionFromCircles(struct Circle_s* p_circleList, double p_time)
{
	SHAPE_Point position = {0};
	struct Complex_s complex = { 0 };
	while (p_circleList)
	{
		complex = addComplex(complex, multiplyComplex(p_circleList->m_coeff, getExponentialComplex(createComplex(0, p_circleList->m_index * 2 * PI * p_time))));
		p_circleList->m_nextCircle;
	}
	return (SHAPE_Point) { complex.m_real, complex.m_imaginary };
}


struct Complex_s getCoeff(int index, struct Complex_s p_position)
{
	struct Complex_s complex = { 0 };
	for (double i = 0; i <= 1; i += 0.02)
	{
		complex = addComplex(complex, multiplyComplex(multiplyComplex(p_position, getExponentialComplex(createComplex(0, (-1) * index * 2 * PI))), createComplex(i, 0)));
	}
}