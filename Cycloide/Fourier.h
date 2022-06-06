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
	double m_amplitude;
	struct Circle_s* m_nextCircle;
};

struct Circle_s* createCircle(int p_index, struct Complex_s p_coeff);

void updateCirclePosition(struct Circle_s* p_circle, SHAPE_Point p_position);

inline void updateCirclePosition(struct Circle_s* p_circle, SHAPE_Point p_position)
{
	p_circle->m_position.x = p_position.x;
	p_circle->m_position.y = p_position.y;
}

void addCircleList(struct Circle_s** p_list, struct Circle_s* p_toAdd);

SHAPE_Point getPositionFromCircles(struct Circle_s* p_circleList, double*** p_bezierList, int p_nbBezier, double p_time);

struct Complex_s getCircleCoeff(int index, double*** p_bezierList, int p_nbBezier);

void drawCircles(SDL_Renderer* renderer, struct Circle_s* p_circleList);

inline struct Complex_s convertPointToComplex(SHAPE_Point p_point)
{
	return createComplex(p_point.x, p_point.y);
}
#endif // FOURIER_H