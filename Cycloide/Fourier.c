#include "Fourier.h"

struct Circle_s* createCircle(int p_index, struct Complex_s p_coeff)
{
	struct Circle_s* circle = (struct Circle_s*) malloc(sizeof(struct Circle_s));
	if (!circle)
		return NULL;

	circle->m_index = p_index;
	circle->m_position = (SHAPE_Point) { 0.0, 0.0 };
	circle->m_coeff = p_coeff;
	circle->m_amplitude = sqrtf((p_coeff.m_real * p_coeff.m_real) + (p_coeff.m_imaginary * p_coeff.m_imaginary));
	circle->m_nextCircle = NULL;
	return circle;
}

void addCircleList(struct Circle_s** p_circleList, struct Circle_s* p_toAdd)
{
	if (!p_circleList || !(*p_circleList) || !p_toAdd)
		return;
	struct Circle_s* temp = (*p_circleList);
	while (temp->m_nextCircle)
		temp = temp->m_nextCircle;

	temp->m_nextCircle = p_toAdd;
}

SHAPE_Point getPositionFromCircles(struct Circle_s* p_circleList, double*** p_bezierList, int p_nbBezier, double p_time)
{
	if (!p_circleList)
		return (SHAPE_Point) { 0, 0 };

	struct Complex_s result = { 0 };
	while (p_circleList)
	{
		updateCirclePosition(p_circleList, (SHAPE_Point) { result.m_real, result.m_imaginary });
		result = addComplex(result, multiplyComplex(p_circleList->m_coeff, getExponentialComplex(createComplex(0, (double)(p_circleList->m_index * 2) * PI * p_time))));
		p_circleList = p_circleList->m_nextCircle;
	}
	return (SHAPE_Point) { result.m_real, result.m_imaginary};
}


struct Complex_s getCircleCoeff(int index, double*** p_bezierList, int p_nbBezier)
{
	struct Complex_s result = {0, 0};
	for (double i = 0; i < 1; i += 0.01)
	{
		// result = result + p_bezierList(t) * e^-indexPIit 
		result = addComplex(result, multiplyComplex(convertPointToComplex(getBezierPointFromList(p_bezierList, p_nbBezier, i)), getExponentialComplex(createComplex(0,  - 2 * index * PI * i))));
	}

	//result = result * dt
	result.m_real *= 0.01;
	result.m_imaginary *= 0.01;
	return result;
}

void drawCircles(SDL_Renderer* renderer, struct Circle_s* p_circleList)
{
	while (p_circleList)
	{
		double radius = p_circleList->m_amplitude;
		radius *= radius < 0 ? -1 : 1;
		const double diameter = radius * 2;

		double x = (radius - 1);
		double y = 0;
		double tx = 1;
		double ty = 1;
		double error = (tx - diameter);
		SHAPE_Point position = p_circleList->m_position;
		while (x >= y)
		{
			//  Each of the following renders an octant of the circle
			SDL_RenderDrawPoint(renderer, position.x + x, position.y - y);
			SDL_RenderDrawPoint(renderer, position.x + x, position.y + y);
			SDL_RenderDrawPoint(renderer, position.x - x, position.y - y);
			SDL_RenderDrawPoint(renderer, position.x - x, position.y + y);
			SDL_RenderDrawPoint(renderer, position.x + y, position.y - x);
			SDL_RenderDrawPoint(renderer, position.x + y, position.y + x);
			SDL_RenderDrawPoint(renderer, position.x - y, position.y - x);
			SDL_RenderDrawPoint(renderer, position.x - y, position.y + x);

			if (error <= 0)
			{
				++y;
				error += ty;
				ty += 2;
			}

			if (error > 0)
			{
				--x;
				tx += 2;
				error += (tx - diameter);
			}
		}
		p_circleList = p_circleList->m_nextCircle;
	}
}

void freeLastCircles(struct Circle_s** p_circleList)
{
	if (!p_circleList)
		return;

	struct Circle_s** cursor = p_circleList;
	while ((*cursor)->m_nextCircle && (*cursor)->m_nextCircle->m_nextCircle)
		cursor = &(*cursor)->m_nextCircle;

	free((*cursor)->m_nextCircle);
	(*cursor)->m_nextCircle = NULL;
	free((*cursor));
	(*cursor) = NULL;
}

void addLastCircles(struct Circle_s** p_circleList, int p_index, double*** p_bezierList, int p_nbBezier)
{
	addCircleList(p_circleList, createCircle(p_index, getCircleCoeff(p_index, p_bezierList, p_nbBezier)));
	addCircleList(p_circleList, createCircle(-p_index, getCircleCoeff(-p_index, p_bezierList, p_nbBezier)));
}