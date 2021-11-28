#include "shapes.h"
#include "svgparser.h"

int main()
{
	xmlDocPtr svgfile = PARSER_LoadSVG("../Cycloide/test.xml");
	xmlNodeShape* shapes = PARSER_GetShapesFromSVG(svgfile);


	return 0;
}