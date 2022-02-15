#define _CRT_SECURE_NO_WARNINGS

#include <stdbool.h>
#include <string.h>
#include "shapes.h"
#include "svgparser.h"


// Abstract shape is a stack of all the implemented shapes
ShapeAbstract* SHAPE_CreateAbstract(char* type, void* data)
{
    ShapeAbstract* abstract_shape = (ShapeAbstract*)calloc(sizeof(ShapeAbstract), 1);
    if (!abstract_shape)
        return NULL;

    abstract_shape->type = _strdup(type);
    abstract_shape->data = data;

    return abstract_shape;
}

ShapeAbstract* SHAPE_CreateAbstractFromSVG(svgShapeStack* svg_shapes)
{
    ShapeAbstract* abstract_shapes = NULL;
    ShapeAbstract* abstract_to_add = NULL;
    while (svg_shapes)
    {
        abstract_to_add = NULL;
        if (!strcmp(svg_shapes->name, "rect"))
            abstract_to_add = SHAPE_CreateAbstract(svg_shapes->name, SHAPE_CreateRectangleFromSVGRectangle(svg_shapes->attributes));
        else if (!strcmp(svg_shapes->name, "circle"))
            abstract_to_add = SHAPE_CreateAbstract(svg_shapes->name, SHAPE_CreateCircleFromSVGCircle(svg_shapes->attributes));
        else if (!strcmp(svg_shapes->name, "ellipse"))
            abstract_to_add = SHAPE_CreateAbstract(svg_shapes->name, SHAPE_CreateEllipseFromSVGEllipse(svg_shapes->attributes));
        else if (!strcmp(svg_shapes->name, "line"))
            abstract_to_add = SHAPE_CreateAbstract(svg_shapes->name, SHAPE_CreateLineFromSVGLine(svg_shapes->attributes));
        else if (!strcmp(svg_shapes->name, "polyline"))
            abstract_to_add = SHAPE_CreateAbstract(svg_shapes->name, SHAPE_CreatePolylineFromSVGPolyline(svg_shapes->attributes));
        else if (!strcmp(svg_shapes->name, "path"))
            abstract_to_add = SHAPE_CreateAbstract(svg_shapes->name, SHAPE_CreatePathFromSVGPath(svg_shapes->attributes));
        
        SHAPE_AddAbstractShapeToAbstractShapeStack(&abstract_shapes, abstract_to_add);
        svg_shapes = svg_shapes->ns;
    }

    return abstract_shapes;
}

// Add an abstract shape to a stack of abstract shape
void SHAPE_AddAbstractShapeToAbstractShapeStack(ShapeAbstract** abstract_shape_stack, ShapeAbstract* abstract_shape_to_add)
{
    if (!abstract_shape_to_add)
        return;
    if (!(*abstract_shape_stack))
    {
        *abstract_shape_stack = abstract_shape_to_add;
        return;
    }
    
    abstract_shape_to_add->next = *abstract_shape_stack;
    *abstract_shape_stack = abstract_shape_to_add;
}

void SHAPE_FreeAbstractShape(ShapeAbstract* abstract_shapes)
{
    if (!abstract_shapes)
        return;
    SHAPE_FreeAbstractShape(abstract_shapes->next);
    if (!strcmp(abstract_shapes->type, "rect"))
        SHAPE_FreeRectangle(abstract_shapes->data);
    else if (!strcmp(abstract_shapes->type, "circle"))
        SHAPE_FreeCircle(abstract_shapes->data);

    free(abstract_shapes->type);
    free(abstract_shapes);
}

// A point is a simple geometric structure than contain an x and y coordinates
ShapePoint* SHAPE_CreatePoint(float x, float y)
{
    ShapePoint* point = (ShapePoint*)calloc(sizeof(ShapePoint), 1);
    if (!point)
        return;
    point->x = x, point->y = y;
    return point;
}


ShapeRectangle* SHAPE_CreateRectangle(float x, float y, float w, float h, float rx, float ry)
{
    ShapeRectangle* rectangle = (ShapeRectangle*)malloc(sizeof(ShapeRectangle));
    ShapeRectangle parameters = { x, y, w, h, rx, ry };
    
    if (!rectangle)
        return NULL;
    
    *rectangle = parameters;

    return rectangle;
}

ShapeRectangle* SHAPE_CreateRectangleFromSVGRectangle(svgAttributeStack* rectangle_attributes)
{
    float x = 0.f;
    float y = 0.f;
    float rx = 0.f;
    float ry = 0.f;
    float w = 0.f;
    float h = 0.f;

    // While there are attributes to read from svg rectangle
    while (rectangle_attributes)
    {
        if (!strcmp(rectangle_attributes->key, "x"))
            x = atof(rectangle_attributes->value);
        else if (!strcmp(rectangle_attributes->key, "y"))
            y = atof(rectangle_attributes->value);
        else if (!strcmp(rectangle_attributes->key, "rx"))
            rx = atof(rectangle_attributes->value);
        else if (!strcmp(rectangle_attributes->key, "ry"))
            ry = atof(rectangle_attributes->value);
        else if (!strcmp(rectangle_attributes->key, "width"))
            w = atof(rectangle_attributes->value);
        else if(!strcmp(rectangle_attributes->key, "height"))
            h = atof(rectangle_attributes->value);

        rectangle_attributes = rectangle_attributes->na;
    }

    // Create a rectangle from those attributes
    return SHAPE_CreateRectangle(x, y, w, h, rx, ry);
}

void SHAPE_FreeRectangle(ShapeRectangle* rectangle)
{
    /*
    free(rectangle->x);
    free(rectangle->y);
    free(rectangle->w);
    free(rectangle->h);
    free(rectangle->rx);
    free(rectangle->ry);
    */
    free(rectangle);
}


ShapeCircle* SHAPE_CreateCircle(float cx, float cy, float r)
{
    ShapeCircle* circle = (ShapeCircle*)malloc(sizeof(ShapeCircle));
    ShapeCircle parameters = { {cx, cy}, r };

    if (!circle)
        return NULL;

    *circle = parameters;
    return circle;
}

ShapeCircle* SHAPE_CreateCircleFromSVGCircle(svgAttributeStack* attributes)
{
    float cx = 0, cy = 0, r = 0;
    while (attributes)
    {
        if (!strcmp(attributes->key, "cx"))
            cx = atof(attributes->value);
        else if (!strcmp(attributes->key, "cy"))
            cy = atof(attributes->value);
        else if (!strcmp(attributes->key, "r"))
            r = atof(attributes->value);

        attributes = attributes->na;
    }

    return SHAPE_CreateCircle(cx, cy, r);
}

ShapePoint* SHAPE_GetPointsFromCircle(ShapeCircle* circle, float step)
{
    ShapePoint* points = SHAPE_CreatePoint(circle->r * sinf(0) + circle->c.x, circle->r * cosf(0) + circle->c.y);
    for (float i = step; i < step + 2 * M_PI; i += step)
    {
        SHAPE_AddPoint(&points, SHAPE_CreatePoint(circle->r * sinf(i) + circle->c.x, circle->r * cosf(i) + circle->c.y));
    }
    return points;
}

void SHAPE_FreeCircle(ShapeCircle* circle)
{
    free(circle);
}

ShapeEllipse* SHAPE_CreateEllipse(float cx, float cy, float rx, float ry)
{
    ShapeEllipse* ellipse = (ShapeEllipse*)malloc(sizeof(ShapeEllipse));
    ShapeEllipse parameters = { {cx, cy}, rx, ry };

    if (!ellipse)
        return NULL;

    *ellipse = parameters;
    return ellipse;
}

ShapeEllipse* SHAPE_CreateEllipseFromSVGEllipse(svgAttributeStack* attributes)
{
    float cx = 0, cy = 0, rx = 0, ry = 0;
    while (attributes)
    {
        if (!strcmp(attributes->key, "cx"))
            cx = atof(attributes->value);
        else if (!strcmp(attributes->key, "cy"))
            cy = atof(attributes->value);
        else if (!strcmp(attributes->key, "rx"))
            rx = atof(attributes->value);
        else if (!strcmp(attributes->key, "ry"))
            ry = atof(attributes->value);

        attributes = attributes->na;
    }

    return SHAPE_CreateEllipse(cx, cy, rx, ry);

}

ShapePoint* SHAPE_GetPointsFromEllipse(ShapeEllipse* ellipse, float step)
{
    float rx = ellipse->rx;
    float ry = ellipse->ry;
    ShapePoint* points = SHAPE_CreatePoint(ellipse->c.x - rx, sqrtf((1 - (powf(ellipse->c.x - rx, 2) / powf(rx, 2))) * powf(ry, 2)));
    for (float x = ellipse->c.x - rx + step; x < ellipse->c.x + step + rx; x += step)
    {
        SHAPE_AddPoint(&points, SHAPE_CreatePoint(x + ellipse->c.x, sqrtf((1 - (powf(x, 2) / powf(rx, 2))) * powf(ry, 2)) + ellipse->c.y));
        SHAPE_AddPoint(&points, SHAPE_CreatePoint(x + ellipse->c.x, -1 * sqrtf((1 - (powf(x, 2) / powf(rx, 2))) * powf(ry, 2)) + ellipse->c.y));
        SHAPE_AddPoint(&points, SHAPE_CreatePoint(-x + ellipse->c.x, sqrtf((1 - (powf(-x, 2) / powf(rx, 2))) * powf(ry, 2)) + ellipse->c.y));
        SHAPE_AddPoint(&points, SHAPE_CreatePoint(-x + ellipse->c.x, -1 * sqrtf((1 - (powf(-x, 2) / powf(rx, 2))) * powf(ry, 2)) + ellipse->c.y));
    }

    return points;
}

ShapeLine* SHAPE_CreateLine(float x1, float x2, float y1, float y2)
{
    ShapeLine* line = (ShapeLine*)malloc(sizeof(sizeof(ShapeLine)));
    line->p = SHAPE_CreatePoint(x1, y1);
    SHAPE_AddPoint(&line->p, SHAPE_CreatePoint(x2, y2));

    return line;
}

ShapeLine* SHAPE_CreateLineFromSVGLine(svgAttributeStack* attributes)
{
    float x1 = 0, x2 = 0, y1 = 0, y2 = 0;

    while (attributes)
    {
        if (!strcmp(attributes->key, "x1"))
            x1 = atof(attributes->value);
        else if (!strcmp(attributes->key, "x2"))
            x2 = atof(attributes->value);
        else if (!strcmp(attributes->key, "y1"))
            y1 = atof(attributes->value);
        else if (!strcmp(attributes->key, "y2"))
            y2 = atof(attributes->value);

        attributes = attributes->na;
    }

    return SHAPE_CreateLine(x1, x2, y1, y2);
}

ShapePoint* SHAPE_GetPointsFromLine(ShapeLine* line, float step)
{
    ShapePoint* a = line->p;
    ShapePoint* b = line->p->np;
    ShapePoint* points = SHAPE_CreatePoint(a->x, a->y);

    float m = (b->y - a->y) / (b->x - a->x);
    float y, x;

    for (float i = 0; i < abs(b->x - a->x); i += step)
    {
        if (a->x < b->x) {
            x = a->x + i;
            y = m * (x - a->x) + a->y;
        }

        else {
            x = b->x + i;
            y = m * (x - a->x) + a->y;
        }

        SHAPE_AddPoint(&points, SHAPE_CreatePoint(x, y));
    }

    return points;
}

ShapePolyline* SHAPE_CreatePolyline(char* points)
{
    ShapePolyline* polyline = (ShapePolyline*)calloc(sizeof(ShapePolyline), 1);
    if (!polyline)
        return NULL;

    char* argument = _strdup(points);

    char* token = strtok(argument, " ");
    char* token_b = NULL;
    char token_c[10];
    char* token_s = NULL;
    float x, y;

    while (token) {
        strcpy(token_c, token);
        token_s = strtok_s(token_c, ",", &token_b);
        x = atof(token_s);
        token_s = strtok_s(NULL, ",", &token_b);
        y = atof(token_s);
      
        ShapePoint* point_to_add = SHAPE_CreatePoint(x, y);
        SHAPE_AddPoint(&polyline->p, point_to_add);
      
        token = strtok(NULL, " ");
    }

    return polyline;
}

ShapePolyline* SHAPE_CreatePolylineFromSVGPolyline(svgAttributeStack* attributes)
{
    while (attributes)
    {
        if (!strcmp(attributes->key, "points"))
            return SHAPE_CreatePolyline(attributes->value);

        attributes = attributes->na;
    }
}

ShapePoint* SHAPE_GetPointsFromPolyline(ShapePolyline* polyline, float step)
{
    ShapePoint* a = polyline->p;
    ShapePoint* b = polyline->p->np;

    ShapeLine* line = SHAPE_CreateLine(a->x, b->x, a->y, b->y);
    ShapePoint* points = SHAPE_GetPointsFromLine(line, step);

    while (a->np->np)
    {
        a = a->np;
        b = a->np;

        line = SHAPE_CreateLine(a->x, b->x, a->y, b->y);
        SHAPE_AddPoints(&points, SHAPE_GetPointsFromLine(line, step));
    }

    return points;
}

ShapePolygone* SHAPE_CreatePolygone(char* points)
{

    ShapePolygone* polygone = (ShapePolygone*)calloc(sizeof(ShapePolygone), 1);
    ShapePoint* point_to_add = NULL;
    if (!polygone)
        return NULL;
    char argument[100]; strcpy(argument, points);
    bool read_x = true, add_point = false, read_first = true;
    float x_first_last = 0, y_first_last = 0, x, y;
    char* token = strtok(argument, " ");
    while (token) {
        if (read_x) {
            x = atof(token);
            if (read_first)
                x_first_last = x;
            read_x = false;
            add_point = false;
        }

        else {
            y = atof(token);
            if (read_first) {
                y_first_last = y_first_last;
                read_first = false;
            }
            read_x = true;
            add_point = true;
        }

        if (add_point) {
            point_to_add = SHAPE_CreatePoint(x_first_last, y_first_last);
            SHAPE_AddPoint(&polygone->p, point_to_add);
        }
        token = strtok(NULL, " ");
    }
    
    point_to_add = SHAPE_CreatePoint(x_first_last, y_first_last);
    SHAPE_AddPoint(&polygone->p, point_to_add);

    return polygone;
}

// When reading path attributes from svg we got : M100,200 C300,400 600,700
// This function read the attributes and give a corresponding path structure
ShapePath* SHAPE_CreatePathFromSVGPath(svgAttributeStack* path_attribute)
{
    // Get to the attributes that contains commands and points
    while (path_attribute->key[0] != 'd')
    {
        path_attribute = path_attribute->na;
    }
    // Delimiter in our example start_char is M
    xmlChar readed_char, start_char;
    start_char = path_attribute->value[0];
    // Index to read data to a given path block
    size_t start_index = 0, end_index = 0;
    size_t attribute_len = strlen(path_attribute->value);

    // The shape that will contains the data 
    ShapePath* path = (ShapePath*)calloc(sizeof(ShapePath), 1);

    // Parsing all the string from start to finish
    for (size_t i = 0; i <= attribute_len; i++)
    {
        readed_char = path_attribute->value[i];
        // If the readed char is a new command, create a new path block  
        if (readed_char != start_char && (readed_char == 'C' || readed_char == 'L' 
            || readed_char == 'H' || readed_char == '\0' || readed_char == 'V' || readed_char == 'Z')
            || readed_char == 'S' || readed_char == 'Q' || readed_char == 'T'
            )
        {

            // Create a path bloc that will contains commands and point data
            ShapePathblock* path_block = (ShapePathblock*)calloc(sizeof(ShapePathblock), 1);
            // Command
            path_block->id = start_char;

            // Stocking the relative points block data in a string 
            end_index = i;
            char* path_bloc_data = (char*)malloc(sizeof(char) * (end_index - start_index));
            if (!path_bloc_data)
                return NULL;
            path_bloc_data[end_index - start_index - 1] = '\0';
            strncpy(path_bloc_data, path_attribute->value + start_index + 1, end_index - start_index - 1);

            // Parsing points data
            char* save_point_data = NULL, * save_x_y_data = NULL;
            char* point_data = strtok_s(path_bloc_data, ",", &save_point_data);
            char* x_y_data;
            float x, y;
            while (point_data)
            {
                x_y_data = strtok_s(point_data, " ", &save_x_y_data);
                x = atof(x_y_data);
                x_y_data = strtok_s(NULL, " ", &save_x_y_data);
                y = atof(x_y_data);
                ShapePoint* point_to_add = SHAPE_CreatePoint(x, y);

                // Add the new created points to the current path block
                SHAPE_AddPoint(&path_block->p, point_to_add);
                point_data = strtok_s(NULL, ",", &save_point_data);

            }

            // Add the new created path block to the path
            SHAPE_PathAddBlock(&path->b, path_block);
            start_index = end_index;
            start_char = readed_char;
            free(path_bloc_data);
        }
    }

    return path;
}


void SHAPE_PathAddBlock(ShapePathblock** blocks, ShapePathblock* block_to_add)
{
    if (!blocks)
        return;
    
    // If the queu is empty  
    if (!(*blocks)) {
        (*blocks) = block_to_add;
        return;
    }

    // Add the block to the end of the queu
    ShapePathblock* cursor = *blocks;
    while (cursor->nb)
        cursor = cursor->nb;
    cursor->nb = block_to_add;
}

void SHAPE_AddPoint(ShapePoint** points, ShapePoint* point_to_add)
{
    if (!points)
        return;

    if (!*points)
    {
        *points = point_to_add;
        return;
    }

    point_to_add->np = *points;
    *points = point_to_add;
}

void SHAPE_AddPoints(ShapePoint** points, ShapePoint* points_to_add)
{
    if (!points)
        return;

    if (!*points)
    {
        *points = points_to_add;
        return;
    }

    ShapePoint* cursor = *points;
    while (cursor->np)
    {
        cursor = cursor->np;
    }

    cursor->np = points_to_add;
}

// Return mathematical coordinates of points making the abstract shape
ShapePoint* SHAPE_GetPointsFromShape(ShapeAbstract* abstract_shape, float step)
{
    // Get the type of the abstract shape
    char* shape_type = abstract_shape->type;

    // Based on its type get the set of points
    if (!strcmp(shape_type, "rect"))
        return SHAPE_GetPointsFromRectangle(abstract_shape->data, step);

    else if (!strcmp(shape_type, "circle"))
        return SHAPE_GetPointsFromCircle(abstract_shape->data, step);

    else if (!strcmp(shape_type, "ellipse"))
        return SHAPE_GetPointsFromEllipse(abstract_shape->data, step);

    else if (!strcmp(shape_type, "line"))
        return SHAPE_GetPointsFromLine(abstract_shape->data, step);

    else if (!strcmp(shape_type, "polyline"))
        return SHAPE_GetPointsFromPolyline(abstract_shape->data, step);
}

ShapePoint* SHAPE_GetPointsFromRectangle(ShapeRectangle* rectangle, float step)
{
    ShapePoint* top_left_corner = SHAPE_CreatePoint(rectangle->x, rectangle->y);
    // Get points from the first edge 
    float value = top_left_corner->x;
    for (float x = top_left_corner->x; x < value + rectangle->w; x+=step)
    {
        SHAPE_AddPoint(&top_left_corner, SHAPE_CreatePoint(x, top_left_corner->y));
    }

    // Get points from the second edge
    value = top_left_corner->y;
    for (float y = top_left_corner->y; y < value+rectangle->h; y+=step)
    {
        SHAPE_AddPoint(&top_left_corner, SHAPE_CreatePoint(top_left_corner->x, y));
    }
    
    // Get points from the third edge
    value = top_left_corner->x;
    for (float x = top_left_corner->x; x > value-rectangle->w; x-=step)
    {
        SHAPE_AddPoint(&top_left_corner, SHAPE_CreatePoint(x, top_left_corner->y));
    }
    
    // Get points from the last edge
    value = top_left_corner->y;
    for (float y = top_left_corner->y; y > value-rectangle->h; y-=step)
    {
        SHAPE_AddPoint(&top_left_corner, SHAPE_CreatePoint(top_left_corner->x, y));
    }
    
    return top_left_corner;
}

