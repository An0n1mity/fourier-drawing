#ifndef FOURIER_H
#define FOURIER_H

#include "BezierCurve.h"
#include "complex.h"
#include "SDL2/SDL.h"

#define PI 3.14159265358979323846

struct Circle_s
{
	int m_index;
	SHAPE_Point m_position;
	struct Complex_s m_coeff;
	struct Circle_s* m_nextCircle;
};

struct Test_s
{
	double m_first;
	double m_second;
};

struct Circle_s* createCircle(int p_index, SHAPE_Point p_position, struct Complex_s p_coeff);

void addCircleList(struct Circle_s** p_list, struct Circle_s* p_toAdd);

SHAPE_Point getPositionFromCircles(struct Circle_s* p_circleList, double*** p_bezierList, int p_nbBezier, double p_time);

struct Complex_s getCircleCoeff(int index, double*** p_bezierList, int p_nbBezier);

void drawCircle(SDL_Renderer* renderer, struct Circle_s p_circle);

#endif // FOURIER_H