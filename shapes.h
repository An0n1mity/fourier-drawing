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
    size_t x;
    size_t y;
    size_t w;
    size_t h;
    size_t rx;
    size_t ry;
};

struct SHAPE_Circle_s{
    size_t cx;
    size_t cy;
    size_t r;
};

struct SHAPE_Ellipse_s{
    size_t cx;
    size_t cy;
    size_t rx;
    size_t ry;
};

struct SHAPE_Line_s{
    size_t x1;
    size_t x2;
    size_t y1;
    size_t y2;
};

struct SHAPE_Polyline_s{
    size_t x;
    size_t y;
    SHAPE_Polyline_s* next;
};

struct SHAPE_Polygone_s{
    size_t x;
    size_t y;
    SHAPE_Polygone_s* next;
};

SHAPE_Abstract* SHAPE_Create_Abstract(char* type, void* data);
void SHAPE_AddShapeToList(SHAPE_Abstract** abstract_shape_list, char* type, void* data);
SHAPE_Rectangle* SHAPE_CreateRectangle(size_t x, size_t y, size_t w, size_t h, size_t rx, size_t ry);
SHAPE_Circle* SHAPE_CreateCircle(size_t rx, size_t ry, size_t r);
SHAPE_Ellipse* SHAPE_CreateEllipse(size_t cx, size_t cy, size_t rx, size_t ry);
SHAPE_Line* SHAPE_CreateLine(size_t x1, size_t x2, size_t y1, size_t y2);
SHAPE_Polyline** SHAPE_CreatePolyline(size_t nb, ...);
void SHAPE_PolylineAddPoint(SHAPE_Polyline** polyline, size_t x, size_t y);
SHAPE_Polygone** SHAPE_CreatePolygone(size_t nb, ...);
void SHAPE_PolygoneAddPoint(SHAPE_Polygone** polygone, size_t x, size_t y);

#endif //SVG_PARSER_SHAPES_H
