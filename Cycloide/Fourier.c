#include "Fourier.h"

struct Circle_s* createCircle(int p_index, SHAPE_Point p_position, struct Complex_s p_coeff)
{
	struct Circle_s* circle = (struct Circle_s*) malloc(sizeof(struct Circle_s));
	if (!circle)
		return NULL;

	circle->m_index = p_index;
	circle->m_position = p_position;
	circle->m_coeff = p_coeff;
	circle->m_nextCircle = NULL;
	return circle;
}

void addCircleList(struct Circle_s** p_list, struct Circle_s* p_toAdd)
{
	if (!p_list || !(*p_list) || !p_toAdd)
		return;

	p_toAdd->m_nextCircle = (*p_list);
	(*p_list) = p_toAdd;
}

SHAPE_Point getPositionFromCircles(struct Circle_s* p_circleList, double*** p_bezierList, int p_nbBezier, double p_time)
{
	if (!p_circleList)
		return (SHAPE_Point) { 0, 0 };

	struct Complex_s result = { 0 };

	/*while (p_circleList)
	{
		result = addComplex(result, multiplyComplex(p_circleList->m_coeff, getExponentialComplex(createComplex(0, (double)(p_circleList->m_index * 2) * PI * p_time))));
		p_circleList = p_circleList->m_nextCircle;
	}
	*/
	//result = multiplyComplex(createComplex(1, 1), getExponentialComplex(createComplex(0, 2 * PI * p_time)));// Créer bien un cercle 
	result = multiplyComplex(p_circleList->m_coeff, getExponentialComplex(createComplex(0, (double)(p_circleList->m_index * 2) * PI * p_time))); //Créer aussi un cercle
	p_circleList = p_circleList->m_nextCircle;
	result = addComplex(result, multiplyComplex(p_circleList->m_coeff, getExponentialComplex(createComplex(0, (double)(p_circleList->m_index * 2) * PI * p_time))));
	return (SHAPE_Point) { result.m_real + 512, result.m_imaginary + 360 };
}


struct Complex_s getCircleCoeff(int index, double*** p_bezierList, int p_nbBezier)
{
	struct Complex_s result = {0};
		
	SHAPE_Point f_0 = getBezierPoint(p_bezierList[0], 0);
	SHAPE_Point f_half = getBezierPoint(p_bezierList[p_nbBezier / 2], 0.5);
	SHAPE_Point f_1 = getBezierPoint(p_bezierList[p_nbBezier - 1], 1);
	struct Complex_s exponent_half = getExponentialComplex(createComplex(0, -1 * (double)index * PI));
	struct Complex_s exponent_1 = getExponentialComplex(createComplex(0, -2 * (double)index * PI));

	result.m_real = 0.166667 * (f_0.x + (4.0 * f_half.x * exponent_half.m_real) + (f_1.x * exponent_1.m_real));
	result.m_imaginary = 0.166667 * (f_0.y + (4.0 * f_half.y * exponent_half.m_imaginary) + (f_1.y * exponent_1.m_imaginary));
	return result;
	
}

void drawCircle(SDL_Renderer* renderer, struct Circle_s p_circle)
{
	double radius = p_circle.m_coeff.m_real;
	radius *= radius < 0 ? -1 : 1;
	const double diameter = radius * 2;

	double x = (radius - 1);
	double y = 0;
	double tx = 1;
	double ty = 1;
	double error = (tx - diameter);
	SHAPE_Point position = p_circle.m_position;
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
}