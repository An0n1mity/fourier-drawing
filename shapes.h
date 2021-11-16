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
    float x;
    float y;
    float w;
    float h;
    float rx;
    float ry;
};

struct SHAPE_Circle_s{
    float cx;
    float cy;
    float r;
};

struct SHAPE_Ellipse_s{
    float cx;
    float cy;
    float rx;
    float ry;
};

struct SHAPE_Line_s{
    float x1;
    float x2;
    float y1;
    float y2;
};

struct SHAPE_Polyline_s{
    float x;
    float y;
    SHAPE_Polyline_s* next;
};

struct SHAPE_Polygone_s{
    float x;
    float y;
    SHAPE_Polygone_s* next;
};

SHAPE_Abstract* SHAPE_Create_Abstract(char* type, void* data);
void SHAPE_AddShapeToList(SHAPE_Abstract** abstract_shape_list, char* type, void* data);
SHAPE_Rectangle* SHAPE_CreateRectangle(float x, float y, float w, float h, float rx, float ry);
SHAPE_Circle* SHAPE_CreateCircle(float rx, float ry, float r);
SHAPE_Ellipse* SHAPE_CreateEllipse(float cx, float cy, float rx, float ry);
SHAPE_Line* SHAPE_CreateLine(float x1, float x2, float y1, float y2);
SHAPE_Polyline** SHAPE_CreatePolyline(float nb, ...);
void SHAPE_PolylineAddPoint(SHAPE_Polyline** polyline, float x, float y);
SHAPE_Polygone** SHAPE_CreatePolygone(float nb, ...);
void SHAPE_PolygoneAddPoint(SHAPE_Polygone** polygone, float x, float y);

#endif //SVG_PARSER_SHAPES_H