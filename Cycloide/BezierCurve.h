#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H

#include "shapes.h"
#include <stdio.h>


/**
 * @brief create two functions from four points 
 * \param[in] p_P0 the first point 
 * \param[in] p_P1 the second point 
 * \param[in] p_P2 the third point 
 * \param[in] p_P3 the fourth point 
 * \return matrix containing function of x and function of y by this order : 1, x, x^2, x^3
 */
double** getBezierFunction(ShapePoint p_P0, ShapePoint p_P1, ShapePoint p_P2, ShapePoint p_P3);

/**
 * @brief free the bezierFunction
 * \param[in] p_bezierFunction the function
 */
void freeBezierFunction(double** p_bezierFunction);

/**
 * @brief get the point from a bezier function depending on time
 * \param[in] p_bezierFunction the bezier function
 * \param[in] p_time at which time I want to get the point
 * \return the point depending of the time
 */
ShapePoint getBezierPoint(double** p_bezierFunction, double p_time);

/**
 * @brief get a point from a list of bezier function from a certain time
 * \param[in] p_bezierFunction the list of bezier function
 * \param[in] p_nbBezier the number of bezier curves
 * \param[in] p_time at which time I want to get the position
 * \return the point the assembled functions at a certain time
 */
ShapePoint getBezierPointFromList(double*** p_bezierFunction, int p_nbBezier, double p_time);

#endif