//
// Created by Alexandre on 14/11/2021.
//

#ifndef SVG_SHAPES_H
#define SVG_SHAPES_H

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "svg.h"

typedef struct SHAPE_Abstract_s SHAPE_Abstract;
typedef struct SHAPE_Rectangle_s SHAPE_Rectangle;
typedef struct SHAPE_Circle_s SHAPE_Circle;
typedef struct SHAPE_Ellipse_s SHAPE_Ellipse;
typedef struct SHAPE_Line_s SHAPE_Line;
typedef struct SHAPE_Polyline_s SHAPE_Polyline;
typedef struct SHAPE_Polygone_s SHAPE_Polygone;
typedef struct SHAPE_Point_s SHAPE_Point;
typedef struct SHAPE_Pathblock_s SHAPE_Pathblock;
typedef struct SHAPE_Path_s SHAPE_Path;

/*
 * Abstract shapes are the generic svg shapes
 * type : define if the shape is a square, rectangle...
 * data : point to the actual matrhematical represnetation of shape
 * next : point to the next abstract shape
 */
struct SHAPE_Abstract_s{
    char type[10];
    void* data;
    struct SHAPE_Abstract_s* next;
};

struct SHAPE_Point_s{
    float x;
    float y;
    struct SHAPE_Point_s* np;
};

/*
 * Rectangle are one type of shape
 *  x : Left corner x position
 *  y : Left corner y position
 *  w : the width of the rectangle
 *  h : the height of the rectangle
 *  rx :
 *  ry :
 */
struct SHAPE_Rectangle_s{
    struct SHAPE_Point_s lc;
    float w;
    float h;
    float rx;
    float ry;
};

struct SHAPE_Circle_s{
    struct SHAPE_Point_s c;
    float r;
};

struct SHAPE_Ellipse_s{
    struct SHAPE_Point_s c;
    float rx;
    float ry;
};

struct SHAPE_Line_s{
    struct SHAPE_Point_s* p;
};

struct SHAPE_Polyline_s{
    struct SHAPE_Point_s* p;
};

struct SHAPE_Polygone_s{
    struct SHAPE_Point_s* p;
};

/*
 * Block that are contained inside the path structure
 *  id : Command type
 *  p : List of points
 *  nb : Point to the next block
 */
struct SHAPE_Pathblock_s{
    char id;
    struct SHAPE_Point_s* p;
    struct SHAPE_Pathblock_s* nb;
};

/*
 * Path structure
 * b : list of all the blocks
 */
struct SHAPE_Path_s{
    struct SHAPE_Pathblock_s* b;
};

SHAPE_Abstract* SHAPE_Create_Abstract(char* type, void* data);
void SHAPE_AddShapeToList(SHAPE_Abstract** abstract_shape_list, char* type, void* data);
SHAPE_Rectangle* SHAPE_CreateRectangle(float x, float y, float w, float h, float rx, float ry);
SHAPE_Circle* SHAPE_CreateCircle(float rx, float ry, float r);
SHAPE_Ellipse* SHAPE_CreateEllipse(float cx, float cy, float rx, float ry);
SHAPE_Line* SHAPE_CreateLine(float x1, float x2, float y1, float y2);
SHAPE_Polyline* SHAPE_CreatePolyline(char* points);
SHAPE_Point* SHAPE_PolylineAddPoint(SHAPE_Point** point, float x, float y);
SHAPE_Polygone* SHAPE_CreatePolygone(char* points);
SHAPE_Point* SHAPE_PolygoneAddPoint(SHAPE_Point** point, float x, float y);
SHAPE_Path* SHAPE_CreatePath(char* points);
SHAPE_Pathblock* SHAPE_PathAddBlock(SHAPE_Pathblock** block, SHAPE_Pathblock* block_to_add);
SHAPE_Point* SHAPE_PathblockAddPoint(SHAPE_Point** points, float x, float y);

#endif //SVG_PARSER_SHAPES_H
