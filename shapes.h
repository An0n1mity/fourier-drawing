#ifndef SVG_SHAPES_H
#define SVG_SHAPES_H

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


#include "svgparser.h"

typedef struct ShapeAbstract_s ShapeAbstract;
typedef struct ShapeRectangle_s ShapeRectangle;
typedef struct ShapeCircle_s ShapeCircle;
typedef struct ShapeEllipse_s ShapeEllipse;
typedef struct ShapeLine_s ShapeLine;
typedef struct ShapePolyline_s ShapePolyline;
typedef struct ShapePolygon_s ShapePolygon;
typedef struct ShapeCubicBezier_s ShapeCubicBezier;
typedef struct ShapePoint_s ShapePoint;
typedef struct ShapePathblock_s ShapePathblock;
typedef struct ShapePath_s ShapePath;


/**
* @brief Absract representation of a shape in memory
*/
struct ShapeAbstract_s {
    char* type; ///< the type of the abstract shape
    void* data; ///< the data of the abstract shape
    struct ShapeAbstract_s* next; ///< pointer to the next abstract shape
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
    float x;
    float y;
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
struct ShapeLine_s {
    struct ShapePoint_s* p; ///< linked list of points
};

/**
 * @brief Mathematical representation of multiples lines
 * A polyline is a set of segements having same extremities
 * Points of the polyline are stored in stack
 */
struct ShapePolyline_s {
    struct ShapePoint_s* p; ///< linked list of points
};

/**
 * @brief Mathematical representation of a polygone
 * The polygone is a closed shapes.
 * Points of the polygone are stored in stack
 */
struct ShapePolygon_s {
    struct ShapePoint_s* p;
};

struct ShapeCubicBezier_s{
    struct ShapePoint_s* p;
};

/**
 * @brief Used with set of command and points
 */
struct ShapePathblock_s {
    char id; ///< command id
    float n;
    struct ShapePoint_s* p; ///< linked list of points
    struct ShapePathblock_s* nb; ///< linked list of blocks
};

/**
 * @brief Used to represent any types of shapes
 * A path is the generic element to define a shape. 
 * All the basic shapes can be created with a path element.
 */
struct ShapePath_s {
    struct ShapePathblock_s* b; ///< linked list of paths blocks
};

/**
 * @brief Create an abstract shape based on is type and data
 * \param[in] type of the abstract shape exemple : "circle ..."
 * \param[in] data of the absract shape
 * @return 
*/
ShapeAbstract* SHAPE_CreateAbstract(char* type, void* data);
/**
 * @brief Create an abstract shape from parsing an SVG
 * \param[in] the stack of svg shapes, shapes define by svg attributes
 * @return 
*/
ShapeAbstract* SHAPE_CreateAbstractFromSVG(svgShapeStack* svg_shapes);
/**
 * @brief Add an abstract shape to the current stack of abstract shapes
 * \param[in] the stack of abstract shapes
 * @param rectangle 
*/
void SHAPE_AddAbstractShapeToAbstractShapeStack(ShapeAbstract** abstract_shape_stack, ShapeAbstract* abstract_shape_to_add);
void SHAPE_FreeAbstractShape(ShapeAbstract* abstract_shapes);

ShapeRectangle* SHAPE_CreateRectangle(float x, float y, float w, float h, float rx, float ry);
ShapeRectangle* SHAPE_CreateRectangleFromSVGRectangle(svgAttributeStack* rectangle_attributes);
ShapePoint* SHAPE_GetPointsFromRectangle(ShapeRectangle* rectangle, float step);
void SHAPE_FreeRectangle(ShapeRectangle* rectangle);

ShapeCircle* SHAPE_CreateCircle(float rx, float ry, float r);
ShapeCircle* SHAPE_CreateCircleFromSVGCircle(svgAttributeStack* attributes);
ShapePoint* SHAPE_GetPointsFromCircle(ShapeCircle* circle, float step);
void SHAPE_FreeCircle(ShapeCircle* circle);

ShapeEllipse* SHAPE_CreateEllipse(float cx, float cy, float rx, float ry);
ShapeEllipse* SHAPE_CreateEllipseFromSVGEllipse(svgAttributeStack* attributes);
ShapePoint* SHAPE_GetPointsFromEllipse(ShapeEllipse* ellipse, float step);

ShapeLine* SHAPE_CreateLine(float x1, float x2, float y1, float y2);
ShapeLine* SHAPE_CreateLineFromSVGLine(svgAttributeStack* attributes);
ShapePoint* SHAPE_GetPointsFromLine(ShapeLine* line, float step);

ShapePolyline* SHAPE_CreatePolyline(char* points);
/**
 * @brief Create a polygone based on string of points coordinates
 * \param[in] points coordinates exemple: "x1 y1 x2 y2 ..."
 * \return polyline
 */
ShapePolyline* SHAPE_CreatePolylineFromSVGPolyline(svgAttributeStack* attributes);
ShapePoint* SHAPE_GetPointsFromPolyline(ShapePolyline* polyline, float step);

ShapePolygon* SHAPE_CreatePolygon(char* command_points);
ShapePolygon* SHAPE_CreatePolygonFromSVGPolygon(svgAttributeStack* attributes);
ShapePoint* SHAPE_GetPointsFromPolygon(ShapePolygon* polygon, float step);
/**
 * @brief Adding point to a list of points
 * \param[in] point last point added to a given shape
 * \param[in] x coordinate of point to add
 * \param[in] y coordinate of point to add
 * \return point_to_add
 */

ShapeCubicBezier* SHAPE_CreateCubicBezier(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);
ShapePoint* SHAPE_GetPointsFromCubicBezier(ShapeCubicBezier* cubic_bezier, float step);

void SHAPE_AddPoint(ShapePoint** points, ShapePoint* point_to_add);
 /**
 * @brief Create a path based on string of commands/points coordinates
 * \param[in] command_points command/points for block generation exemple: "M x1 y1 x2 y2 ..."
 * \return path
 */
void SHAPE_AddPoints(ShapePoint** points, ShapePoint* points_to_add);
//ShapePath* CreatePathFromSVGPath(svgAttributeStack* path_attribute);
void SHAPE_PathAddBlock(ShapePathblock** block, ShapePathblock* block_to_add);
ShapePoint* SHAPE_CreatePoint(float x, float y);

ShapePathblock* SHAPE_CreatePathBlock(char type, char* points);
ShapePathblock* SHAPE_CreatePathblocksFromSVGPathblocks(svgAttributeStack* path_attribute);
ShapePoint* SHAPE_GetPointsFromPathblocks(ShapePathblock* blocks, float step);

ShapePoint* SHAPE_GetPointsFromAbstractShape(ShapeAbstract* abstract_shape, float step);
ShapePoint* SHAPE_GetPointsFromAbstractShapes(ShapeAbstract* abstract_shape_stack, float step);
#endif //SVG_PARSER_SHAPES_H