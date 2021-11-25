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


struct SHAPE_Abstract_s {
    char type[10];
    void* data;
    struct SHAPE_Abstract_s* next;
};

/**
 * @brief Mathematical representation of points
 */
struct SHAPE_Point_s {
    float x; ///< x coordinate
    float y; ///< y coordinate
    struct SHAPE_Point_s* np; ///< pointer to the next point
};

/**
 * @brief Mathematical representation of rectangles
 */
struct SHAPE_Rectangle_s {
    struct SHAPE_Point_s lc; ///< left corner point
    float w; ///< width
    float h; ///< height
    float rx; ///< x radius
    float ry; ///< y radius
};

/**
 * @brief Mathematical representation of circles
 */
struct SHAPE_Circle_s {
    struct SHAPE_Point_s c; ///< center point
    float r; ///< radius
};

/**
 * @brief Mathematical representation of ellipses
 */
struct SHAPE_Ellipse_s {
    struct SHAPE_Point_s c; ///< center point
    float rx; ///< x radius
    float ry; ///< y radius
};

/**
 * @brief Mathematical representation of lines
 */
struct SHAPE_Line_s {
    struct SHAPE_Point_s* p; ///< linked list of points
};

/**
 * @brief Mathematical representation of multiples lines
 */
struct SHAPE_Polyline_s {
    struct SHAPE_Point_s* p; ///< linked list of points
};

struct SHAPE_Polygone_s {
    struct SHAPE_Point_s* p;
};

/**
 * @brief Used with set of command and points
 */
struct SHAPE_Pathblock_s {
    char id; ///< command id
    struct SHAPE_Point_s* p; ///< linked list of points
    struct SHAPE_Pathblock_s* nb; ///< linked list of blocks
};

/**
 * @brief Used to represent any types of shapes
 */
struct SHAPE_Path_s {
    struct SHAPE_Pathblock_s* b; ///< linked list of paths blocks
};

SHAPE_Abstract* SHAPE_Create_Abstract(char* type, void* data);
void SHAPE_AddShapeToList(SHAPE_Abstract** abstract_shape_list, char* type, void* data);
SHAPE_Rectangle* SHAPE_CreateRectangle(float x, float y, float w, float h, float rx, float ry);
SHAPE_Circle* SHAPE_CreateCircle(float rx, float ry, float r);
SHAPE_Ellipse* SHAPE_CreateEllipse(float cx, float cy, float rx, float ry);
SHAPE_Line* SHAPE_CreateLine(float x1, float x2, float y1, float y2);
SHAPE_Polyline* SHAPE_CreatePolyline(char* points);
SHAPE_Point* SHAPE_PolylineAddPoint(SHAPE_Point** point, float x, float y);
/**
 * @brief Create a polygone based on string of points coordinates
 * \param[in] points coordinates exemple: "x1 y1 x2 y2 ..."
 * \return polyline
 */
SHAPE_Polygone* SHAPE_CreatePolygone(char* command_points);
SHAPE_Point* SHAPE_PolygoneAddPoint(SHAPE_Point** point, float x, float y);
/**
 * @brief Create a path based on string of commands/points coordinates
 * \param[in] command_points command/points for block generation exemple: "M x1 y1 x2 y2 ..."
 * \return path
 */
SHAPE_Path* SHAPE_CreatePath(char* points);
SHAPE_Pathblock* SHAPE_PathAddBlock(SHAPE_Pathblock** block, SHAPE_Pathblock* block_to_add);
SHAPE_Point* SHAPE_PathblockAddPoint(SHAPE_Point** points, float x, float y);

#endif //SVG_PARSER_SHAPES_H

