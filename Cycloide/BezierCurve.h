#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H

#include "shapes.h"
#include <stdio.h>

double** getBezierFunction(SHAPE_Point p_P0, SHAPE_Point p_P1, SHAPE_Point p_P2, SHAPE_Point p_P3);

void freeBezierFunction(double** p_bezierFunction);

SHAPE_Point getBezierPoint(double** p_bezierFunction, double p_position);
#endif