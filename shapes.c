//
// Created by Alexandre on 14/11/2021.
//

#include <stdarg.h>
#include "shapes.h"
#include "svg.h"

/*
 *  Return an abstract shape
 *  type : string defining the abstract shape type
 *  data : pointing to the mathematical representation
 */
SHAPE_Abstract* SHAPE_CreateAbstract(char* type, void* data)
{
    SHAPE_Abstract* abstract_shape = (SHAPE_Abstract*)malloc(sizeof(SHAPE_Abstract));

    strcpy(abstract_shape->type, type);
    abstract_shape->data = data;
    abstract_shape->next = NULL;

    return abstract_shape;
}

/*
 * Given a list of abstract shape, append a new abstract shape to it
 * abstract_shape_list : the list of abstract shapes
 * type : string defining the new abstract shape type
 * data : pointing to the mathematical representation of the new shape to add
 */
void SHAPE_AddShapeToList(SHAPE_Abstract** abstract_shape_list, char* type, void* data)
{
    SHAPE_Abstract* abstract_shape_to_add = SHAPE_CreateAbstract(type, data);
    abstract_shape_to_add->next = *abstract_shape_list;
    *abstract_shape_list = abstract_shape_to_add;
}

/*
 * Create a mathematical representation of a rectangle
 */
SHAPE_Rectangle* SHAPE_CreateRectangle(float x, float y, float w, float h, float rx, float ry)
{
    SHAPE_Rectangle* rectangle = malloc(sizeof(SHAPE_Rectangle));
    SHAPE_Rectangle parameters = {x, y, w, h, rx, ry};
    *rectangle = parameters;

    return rectangle;
}

SHAPE_Circle* SHAPE_CreateCircle(float cx, float cy, float r)
{
    SHAPE_Circle* circle = malloc(sizeof(SHAPE_Circle));
    SHAPE_Circle parameters = {cx, cy, r};
    *circle = parameters;
    return circle;
}

SHAPE_Ellipse* SHAPE_CreateEllipse(float cx, float cy, float rx, float ry)
{
    SHAPE_Ellipse* ellipse = malloc(sizeof(SHAPE_Ellipse));
    SHAPE_Ellipse parameters = {cx, cy, rx, ry};
    *ellipse = parameters;
    return ellipse;
}

SHAPE_Polyline** SHAPE_CreatePolyline(float nb, ...)
{
    SHAPE_Polyline** polyline = (SHAPE_Polyline**)calloc(sizeof(SHAPE_Polyline*), 1);
    SHAPE_Polyline** head = polyline;
    va_list arguments;
    va_start(arguments, nb);
    for (int i = 0; i < nb; ++i) {
        float x = va_arg(arguments, float);
        float y = va_arg(arguments, float);
        SHAPE_PolylineAddPoint(polyline, x, y);
        polyline = &(*polyline)->next;
    }
    va_end(arguments);
    return head;
}

void SHAPE_PolylineAddPoint(SHAPE_Polyline** polyline, float x, float y){
    SHAPE_Polyline* points_to_add = (SHAPE_Polyline*)calloc(sizeof(SHAPE_Polyline), 1);
    points_to_add->x = x;
    points_to_add->y = y;

    // if it is the first point to add
    if(!(*polyline)){
        *polyline = points_to_add;
        return;
    }
    (*polyline)->next = points_to_add;
}

SHAPE_Polygone** SHAPE_CreatePolygone(float nb, ...)
{
    SHAPE_Polygone** polygone = (SHAPE_Polygone**)calloc(sizeof(SHAPE_Polygone*), 1);
    SHAPE_Polygone** head = polygone;
    va_list arguments;
    va_start(arguments, nb);
    float x_first_last = va_arg(arguments, float);
    float y_first_last = va_arg(arguments, float);
    SHAPE_PolygoneAddPoint(polygone, x_first_last, y_first_last);
    for (int i = 0; i < nb; ++i) {
        float x = va_arg(arguments, float);
        float y = va_arg(arguments, float);
        SHAPE_PolygoneAddPoint(polygone, x, y);
        polygone = &(*polygone)->next;
    }
    SHAPE_PolygoneAddPoint(polygone, x_first_last, y_first_last);
    return head;
}

void SHAPE_PolygoneAddPoint(SHAPE_Polygone** polygone, float x, float y)
{
    SHAPE_Polygone* point_to_add = (SHAPE_Polygone*)malloc(sizeof(SHAPE_Polygone));
    point_to_add->x = x;
    point_to_add->y = y;
    if(!(*polygone)){
        *polygone = point_to_add;
        return;
    }
    polygone = &(*polygone)->next;
}

