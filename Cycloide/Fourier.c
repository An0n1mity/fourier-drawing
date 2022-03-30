#include "Fourier.h"

struct Circle_s* createCircle(int p_index, SHAPE_Point p_position, struct Complex_s p_coeff)
{
	struct Circle_s* circle = (struct Circle_s*) malloc(sizeof(struct Circle_s));
	circle->m_index = p_index;
	circle->m_position = p_position;
	circle->m_coeff = p_coeff;
	circle->m_nextCircle = NULL;
	return circle;
}

void addCircleList(struct Circle_s** p_list, struct Circle_s* p_toAdd)
{
	p_toAdd->m_nextCircle = (*p_list);
	(*p_list) = p_toAdd;
}

SHAPE_Point getPositionFromCircles(struct Circle_s* p_circleList, double p_time)
{
	SHAPE_Point position = { 0 };
	struct Complex_s result = { 0 };

	while (p_circleList)
	{
		p_circleList->m_coeff = getCoeff(p_circleList->m_index, 
			(struct Complex_s) { 2.0 * ((double)rand() / RAND_MAX), 2.0 * ((double)rand() / RAND_MAX)}
		/*getPositionFromTime(p_position, p_time)*/);

		//getPositionFromTime is more getBezierFunction with on a given time

		result = addComplex(result, multiplyComplex(p_circleList->m_coeff, getExponentialComplex(createComplex(0, (double)(p_circleList->m_index * 2) * PI * p_time))));
		p_circleList = p_circleList->m_nextCircle;
	}
	return (SHAPE_Point) { result.m_real, result.m_imaginary }; 
}


struct Complex_s getCoeff(int index, struct Complex_s p_position)
{
	struct Complex_s result = { 0 };
	for (double i = 0; i <= 1; i += 0.02)
	{
		result = addComplex(result, multiplyComplex(multiplyComplex(p_position, getExponentialComplex(createComplex(0, (-1) * index * 2 * PI))), createComplex(i, 0)));
	}
	return result;
}