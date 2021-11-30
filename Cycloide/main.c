#include "shapes.h"
#include "svgparser.h"
#include "BezierCurve.h"

int main()
{
	xmlDocPtr svgfile = PARSER_LoadSVG("../Cycloide/test.xml");
	xmlNodeShape* shapes = PARSER_GetShapesFromSVG(svgfile);

	SHAPE_Point test[4];
	test[0].x = 0; test[0].y = 0;
	test[1].x = 0.2; test[1].y = 0.4;
	test[2].x = 0.8; test[2].y = 0.4;
	test[3].x = 1; test[3].y = 1;
	int** func = getBezierFunction(test[0], test[1], test[2], test[3]);

	test[0] = getBezierPoint(func, 0.25);
	test[1] = getBezierPoint(func, 0.5);
	test[2] = getBezierPoint(func, 0.75);
	test[3] = getBezierPoint(func, 1);
	printf("en 1/4 : (%lf;%lf)\n en 1/2 : (%lf;%lf)\n en 3/4 : (%lf;%lf)\n en 1 : (%lf;%lf)\n", test[0].x, test[0].y, test[1].x,test[1].y,test[2].x, test[2].y, test[3].x,test[3].y);
	freeBezierFunction(func);
}