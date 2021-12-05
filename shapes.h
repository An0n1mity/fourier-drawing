#ifndef SVG_SHAPES_H
#define SVG_SHAPES_H

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "svgparser.h"

typedef struct ShapeAbstract_s ShapeAbstract;
typedef struct ShapeRectangle_s ShapeRectangle;
typedef struct ShapeCircle_s ShapeCircle;
typedef struct ShapeEllipse_s ShapeEllipse;
typedef struct SHAPE_Line_s ShapeLine;
typedef struct SHAPE_Polyline_s ShapePolyline;
typedef struct SHAPE_Polygone_s ShapePolygone;
typedef struct ShapePoint_s ShapePoint;
typedef struct SHAPE_Pathblock_s ShapePathblock;
typedef struct SHAPE_Path_s ShapePath;


struct ShapeAbstract_s {
    char* type;
    void* data;
    struct ShapeAbstract_s* next;
};

/**
 * @brief Mathematical representation of points
 * Points are stored as linked list
 */
struct ShapePoint_s {
    float x; ///< x coordinate
    float y; ///< y coordinate
    struct ShapePoint_s* np; ///< pointer to the next point
};

/**
 * @brief Mathematical representation of rectangles
 * Rectangles are defined by their position, width, and height. 
 * Rectangles may have their corners rounded.
 */
struct ShapeRectangle_s {
    struct ShapePoint_s lc; ///< left corner point
    float w; ///< width
    float h; ///< height
    float rx; ///< x round corners
    float ry; ///< y round corners
};

/**
 * @brief Mathematical representation of circles
 * Circle are defined by their center point and radius
 */
struct ShapeCircle_s {
    struct ShapePoint_s c; ///< center point
    float r; ///< radius
};

/**
 * @brief Mathematical representation of ellipses
 * Ellipse is closely related to a circle. 
 * The difference is that an ellipse has an x and a y radius that differs from each other.
 */
struct ShapeEllipse_s {
    struct ShapePoint_s c; ///< center point
    float rx; ///< x radius
    float ry; ///< y radius
};

/**
 * @brief Mathematical representation of lines
 * A line is just a set of two points
 */
struct SHAPE_Line_s {
    struct SHAPE_Point_s* p; ///< linked list of points
};

/**
 * @brief Mathematical representation of multiples lines
 * A polyline is a set of segements having same extremities
 * Points of the polyline are stored in stack
 */
struct SHAPE_Polyline_s {
    struct SHAPE_Point_s* p; ///< linked list of points
};

/**
 * @brief Mathematical representation of a polygone
 * The polygone is a closed shapes.
 * Points of the polygone are stored in stack
 */
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
 * A path is the generic element to define a shape. 
 * All the basic shapes can be created with a path element.
 */
struct SHAPE_Path_s {
    struct SHAPE_Pathblock_s* b; ///< linked list of paths blocks
};

ShapeAbstract* SHAPE_CreateAbstract(char* type, void* data);
void SHAPE_AddShapeToList(ShapeAbstract** abstract_shape_list, char* type, void* data);
ShapeRectangle* SHAPE_CreateRectangle(float x, float y, float w, float h, float rx, float ry);
ShapeCircle* SHAPE_CreateCircle(float rx, float ry, float r);
ShapeEllipse* SHAPE_CreateEllipse(float cx, float cy, float rx, float ry);
ShapeLine* SHAPE_CreateLine(float x1, float x2, float y1, float y2);
ShapePolyline* SHAPE_CreatePolyline(char* points);
/**
 * @brief Create a polygone based on string of points coordinates
 * \param[in] points coordinates exemple: "x1 y1 x2 y2 ..."
 * \return polyline
 */
ShapePolygone* SHAPE_CreatePolygone(char* command_points);
/**
 * @brief Adding point to a list of points
 * \param[in] point last point added to a given shape
 * \param[in] x coordinate of point to add
 * \param[in] y coordinate of point to add
 * \return point_to_add
 */
void SHAPE_AddPoint(ShapePoint** points, ShapePoint* point_to_add);
 /**
 * @brief Create a path based on string of commands/points coordinates
 * \param[in] command_points command/points for block generation exemple: "M x1 y1 x2 y2 ..."
 * \return path
 */
//ShapePath* CreatePathFromSVGPath(svgAttributeStack* path_attribute);
void SHAPE_PathAddBlock(ShapePathblock** block, ShapePathblock* block_to_add);
ShapePoint* SHAPE_CreatePoint(float x, float y);
#endif //SVG_PARSER_SHAPES_H