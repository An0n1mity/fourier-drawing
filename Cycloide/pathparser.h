//
// Created by biist on 02/05/2022.
//

#ifndef CYCLOIDE_PATHPARSER_H
#define CYCLOIDE_PATHPARSER_H

#include <string.h>
#include <stdlib.h>
#include "shapes.h"
#include <inttypes.h>

ShapePathblock* ParsePath(svgAttributeStack* path_attribute);
ShapePoint* SHAPE_GetPointsFromPathBlocksWithPoints(ShapePathblock* blocks, float step);

#endif //CYCLOIDE_PATHPARSER_H
