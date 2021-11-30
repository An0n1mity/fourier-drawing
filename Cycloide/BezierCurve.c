#include "BezierCurve.h"

double** getBezierFunction(SHAPE_Point p_P0, SHAPE_Point p_P1, SHAPE_Point p_P2, SHAPE_Point p_P3)
{
// B(t) = (1-t)^3P0 + 3t(1-t)�P1 + 3t�(1-t)P2 + t^3P3
// B(t) = P0 + (-3P0 + 3P1 )t + (3P0 - 6P1 + 3P2)t^2 + (-P0 + 3P1 - 3P2 + P3) t^3

double** cubicFunction = (double**) calloc(2, sizeof(double*)); // 1, x, x�, x^3
if (!cubicFunction)
{
	printf("Can't create cubicFunction in getBezierFunction\n");
	return NULL;
}

cubicFunction[0] = (double*) calloc(4, sizeof(double)); // Bx(t)
cubicFunction[1] = (double*) calloc(4, sizeof(double)); // By(t)

if (!cubicFunction[0] || !cubicFunction[1])
{
	printf("Can't create cubicFunctionX or Y in getBezierFunction\n");
	free(cubicFunction[0]);
	free(cubicFunction);
	return NULL;
}

cubicFunction[0][0] = p_P0.x;												cubicFunction[1][0] = p_P0.y;
cubicFunction[0][1] = 0.0 - (3 * p_P0.x) + (3 * p_P1.x);					cubicFunction[1][1] = 0.0 - (3 * p_P0.y) + (3 * p_P1.y);
cubicFunction[0][2] = (3 * p_P0.x) - (6 * p_P1.x) + (3 * p_P2.x);			cubicFunction[1][2] = (3 * p_P0.y) - (6 * p_P1.y) + (3 * p_P2.y);
cubicFunction[0][3] = 0.0 - p_P0.x + (3 * p_P1.x) - (3 * p_P2.x) + p_P3.x;	cubicFunction[1][3] = 0.0 - p_P0.y + (3 * p_P1.y) - (3 * p_P2.y) + p_P3.y;

return cubicFunction;
}

void freeBezierFunction(double** p_bezierFunction)
{
	free(p_bezierFunction[1]);
	free(p_bezierFunction[0]);
	free(p_bezierFunction);
}

SHAPE_Point getBezierPoint(double** p_bezierFunction, double t)
{
	SHAPE_Point point = { 0,0 };
	double tSquared = t * t, tCubed = tSquared * t;
	point.x = p_bezierFunction[0][0] +
		(p_bezierFunction[0][1] * t) +
		(p_bezierFunction[0][2] * tSquared) +
		(p_bezierFunction[0][3] * tCubed);

	point.y = p_bezierFunction[1][0] +
		(p_bezierFunction[1][1] * t) +
		(p_bezierFunction[1][2] * tSquared) +
		(p_bezierFunction[1][3] * tCubed);
	return point;
}