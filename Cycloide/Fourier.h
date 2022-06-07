#ifndef FOURIER_H
#define FOURIER_H

#include "BezierCurve.h"
#include "complex.h"
#include "SDL2/SDL.h"

#define PI 3.14159265358979323846

struct Circle_s
{
	int m_index;					///< index of the circle
	SHAPE_Point m_position;			///< position of the circle
	struct Complex_s m_coeff;		///< complex coeff corresponding to the initial angle and the amplitude
	double m_amplitude;				///< radius of the circle
	struct Circle_s* m_nextCircle;	///< pointer to the next circle
};

/**
 * @brief create a circle for Fourier
 * \param[in] p_index the index of the circle
 * \param[in] p_coeff the angle and the size of the circle
 * \return a circle element
 */
struct Circle_s* createCircle(int p_index, struct Complex_s p_coeff);

/**
 * @brief change the position of the given circle
 * \param[in] p_circle the circle
 * \param[in] p_position the new position of the circle
 */

__inline void updateCirclePosition(struct Circle_s* p_circle, SHAPE_Point p_position)
{
	p_circle->m_position.x = p_position.x;
	p_circle->m_position.y = p_position.y;
}

/**
 * @brief link a circle to an other
 * \param[in] p_list the list of circles
 * \param[in] p_toAdd the circle to link
 */
void addCircleList(struct Circle_s** p_list, struct Circle_s* p_toAdd);

/**
 * @brief apply fourier to get the position led by the circles
 * \param[in] p_circleList the list of circles
 * \param[in] p_bezierList the list of bezier functions
 * \param[in] p_nbBezier the number of bezier functions	
 * \param[in] p_time the time of the next circle position
 * \return the new position led by the circles
 */
SHAPE_Point getPositionFromCircles(struct Circle_s* p_circleList, double*** p_bezierList, int p_nbBezier, double p_time);

/**
 * @brief calculate the coeff of the circle depending of the index and the path
 * \param[in] p_index the index of the circle
 * \param[in] p_bezierList the list representing the path of the circles
 * \param[in] p_nbBezier the number of functions inside the path
 * \return complex value of the coeff of the circle
 */
struct Complex_s getCircleCoeff(int index, double*** p_bezierList, int p_nbBezier);

/**
 * @brief draw the circles of the circle list
 * \param[in] p_renderer the place to draw the circles
 * \param[in] p_circleList the list of circles
 */
void drawCircles(SDL_Renderer* renderer, struct Circle_s* p_circleList);

/**
 * @brief convert a SHAPE_Point to a complex number
 * \param[in] p_point the SHAPE_Point to convert
 * \return complex number representing the position
 */
__inline struct Complex_s convertPointToComplex(SHAPE_Point p_point)
{
	return createComplex(p_point.x, p_point.y);
}

/**
 * @brief free the two last circles when decreasing precision
 * \param[in] p_circleList the list of circles
 */
void freeLastCircles(struct Circle_s** p_circleList);

/**
 * @brief add two circles at the end when increasing the precision
 * \param[in] p_circleList the list of circles
 * \param[in] p_index the index of the new circles
 * \param[in] p_bezierList the list of function representing the path
 * \param[in] p_nbBezier the number of functions inside the path
 */
void addLastCircles(struct Circle_s** p_circleList, int p_index, double*** p_bezierList, int p_nbBezier);

/**
 * @brief setup the fourier transform 
 * \param[in/out] p_bezierList list that will be filled with functions from given points inside
 * \return complex number representing the position
 */
struct Circle_s* initFourier(double*** p_bezierList);

/**
 * @brief free all the created circles
 * \param[in] p_circleList list of the circles
 */
void freeCircles(struct Circle_s* p_circleList);

#endif // FOURIER_H