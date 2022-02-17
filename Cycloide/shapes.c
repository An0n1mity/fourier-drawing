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
        else if (!strcmp(svg_shapes->name, "polygon"))
            abstract_to_add = SHAPE_CreateAbstract(svg_shapes->name, SHAPE_CreatePolygonFromSVGPolygon(svg_shapes->attributes));
        else if (!strcmp(svg_shapes->name, "path"))
            abstract_to_add = SHAPE_CreateAbstract(svg_shapes->name, SHAPE_CreatePathblocksFromSVGPathblocks(svg_shapes->attributes));
        

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

ShapePoint* SHAPE_GetPointsFromRectangle(ShapeRectangle* rectangle, float step)
{
    ShapePoint* top_left_corner = SHAPE_CreatePoint(rectangle->x, rectangle->y);
    // Get points from the first edge 
    float value = top_left_corner->x;
    for (float x = top_left_corner->x; x < value + rectangle->w; x += step)
    {
        SHAPE_AddPoint(&top_left_corner, SHAPE_CreatePoint(x, top_left_corner->y));
    }

    // Get points from the second edge
    value = top_left_corner->y;
    for (float y = top_left_corner->y; y < value + rectangle->h; y += step)
    {
        SHAPE_AddPoint(&top_left_corner, SHAPE_CreatePoint(top_left_corner->x, y));
    }

    // Get points from the third edge
    value = top_left_corner->x;
    for (float x = top_left_corner->x; x > value - rectangle->w; x -= step)
    {
        SHAPE_AddPoint(&top_left_corner, SHAPE_CreatePoint(x, top_left_corner->y));
    }

    // Get points from the last edge
    value = top_left_corner->y;
    for (float y = top_left_corner->y; y > value - rectangle->h; y -= step)
    {
        SHAPE_AddPoint(&top_left_corner, SHAPE_CreatePoint(top_left_corner->x, y));
    }

    return top_left_corner;
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
    float y = a->y, x = a->x;

    float diff_x =  b->x - a->x;
    float diff_y =  b->y - a->y;

    size_t nb = sqrtf(powf(a->x - b->x, 2) + powf(a->y - b->y, 2))/step;

    float interval_x = diff_x / nb;
    float interval_y = diff_y / nb;

    for (size_t i = 1; i <= nb; i ++)
    {
        x = ((float)i/nb) * diff_x + a->x;
        y = ((float)i/nb) * diff_y + a->y;
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

    while (b->np)
    {
        a = b;
        b = b->np;

        line = SHAPE_CreateLine(a->x, b->x, a->y, b->y);
        SHAPE_AddPoints(&points, SHAPE_GetPointsFromLine(line, step));
    }

    return points;
}

ShapePolygon* SHAPE_CreatePolygon(char* points)
{

    ShapePolygon* polygon = (ShapePolygon*)calloc(sizeof(ShapePolygon), 1);
    if (!polygon)
        return NULL;

    char* argument = _strdup(points);

    char* token = strtok(argument, " ");
    char* token_b = NULL;
    char token_c[10];
    char* token_s = NULL;
    float x, y, first_x, first_y;
    bool first = true;

    while (token) {
        strcpy(token_c, token);
        token_s = strtok_s(token_c, ",", &token_b);
        x = atof(token_s);
        token_s = strtok_s(NULL, ",", &token_b);
        y = atof(token_s);

        if (first)
        {
            first_x = x;
            first_y = y;
            first = false;
        }

        ShapePoint* point_to_add = SHAPE_CreatePoint(x, y);
        SHAPE_AddPoint(&polygon->p, point_to_add);

        token = strtok(NULL, " ");
    }

    ShapePoint* point_to_add = SHAPE_CreatePoint(first_x, first_y);
    SHAPE_AddPoint(&polygon->p, point_to_add);

    return polygon;
}

ShapePolygon* SHAPE_CreatePolygonFromSVGPolygon(svgAttributeStack* attributes)
{
    while (attributes)
    {
        if (!strcmp(attributes->key, "points"))
            return SHAPE_CreatePolygon(attributes->value);

        attributes = attributes->na;
    }
}

ShapePoint* SHAPE_GetPointsFromPolygon(ShapePolygon* polygon, float step)
{
    ShapePoint* a = polygon->p;
    ShapePoint* b = polygon->p->np;

    ShapeLine* line = SHAPE_CreateLine(a->x, b->x, a->y, b->y);
    ShapePoint* points = SHAPE_GetPointsFromLine(line, step);
    while (b->np)
    {
        a = b;
        b = b->np;

        line = SHAPE_CreateLine(a->x, b->x, a->y, b->y);
        SHAPE_AddPoints(&points, SHAPE_GetPointsFromLine(line, step));
    }

    return points;
}

ShapeCubicBezier* SHAPE_CreateCubicBezier(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3)
{
    ShapeCubicBezier* cubic_bezier = (ShapeCubicBezier*)malloc(sizeof(ShapeCubicBezier));

    SHAPE_AddPoint(&cubic_bezier->p, SHAPE_CreatePoint(x3, y3));
    SHAPE_AddPoint(&cubic_bezier->p, SHAPE_CreatePoint(x2, y2));
    SHAPE_AddPoint(&cubic_bezier->p, SHAPE_CreatePoint(x1, y1));
    SHAPE_AddPoint(&cubic_bezier->p, SHAPE_CreatePoint(x0, y0));

    return cubic_bezier;
}

ShapePoint* SHAPE_GetPointsFromCubicBezier(ShapeCubicBezier* cubic_bezier, float step)
{
    // Point M
    ShapePoint* a = cubic_bezier->p;
    ShapePoint* b = a->np;
    ShapePoint* c = b->np;
    ShapePoint* d = c->np;

    ShapePoint* points = NULL;

    float x, y;

    for (float t = 0; t < 1.f; t+=step)
    {
        x = powf(1 - t, 3) * a->x + 3 * powf(1 - t, 2) * t * b->x + 3 * (1-t) * powf(t, 2) * c->x + powf(t, 3) * d->x;
        y = powf(1 - t, 3) * a->y + 3 * powf(1 - t, 2) * t * b->y + 3 * (1-t) * powf(t, 2) * c->y + powf(t, 3) * d->y;

        SHAPE_AddPoint(&points, SHAPE_CreatePoint(x, y));
    }
    return points;
}

ShapePathblock* SHAPE_CreatePathBlock(char type, char* points)
{
    ShapePathblock* path_block = (ShapePathblock*)calloc(sizeof(ShapePathblock), 1);
    path_block->id = type;
    float x, y;
    char* token, buffer[100];
    char* tmp;

    if (type == 'M' || type == 'L')
    {
        token = strtok(points, " ");
        x = atof(token);
        token = strtok(NULL, " ");
        y = atof(token);

        path_block->p = SHAPE_CreatePoint(x, y);
    }

    else if (type == 'H' || type == 'V' || type == 'h' || type == 'v')
    {
        path_block->n = atof(points);
    }
    
    else if (type == 'C')
    {
        token = strtok_s(points, ",", &buffer);
        while (token)
        {
            x = atof(strtok(token + 1, " "));
            y = atof(strtok(NULL, " "));

            SHAPE_AddPoint(&path_block->p, SHAPE_CreatePoint(x, y));

            token = strtok_s(NULL, ",", &buffer);
        }

    }
    

    return path_block;
}

// When reading path attributes from svg we got : M100,200 C300,400 600,700
// This function read the attributes and give a corresponding path structure
ShapePathblock* SHAPE_CreatePathblocksFromSVGPathblocks(svgAttributeStack* path_attribute)
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

    // Get pointer to the start of a bloc 
    char* block_delimiter[10] = {0}; size_t j = 0;
    for (size_t i = 0; i <= attribute_len; i++)
    {
        if (path_attribute->value[i] == 'M' || path_attribute->value[i] == 'H' ||
            path_attribute->value[i] == '\0'|| path_attribute->value[i] == 'V' ||
            path_attribute->value[i] == 'L' || path_attribute->value[i] == 'Z' ||
            path_attribute->value[i] == 'h' || path_attribute->value[i] == 'v' || 
            path_attribute->value[i] == 'C'
            )
        {
            block_delimiter[j] = &path_attribute->value[i];
            j++;
        }
    }

    // Parsing the data in a given block
    ShapePathblock* blocks = NULL;
    char block_parameters[100] = { 0 };
    size_t idx = 0;
    for (size_t i = 0; i < j-1; i++)
    {
        // Copy paramaters contain in a block
        strncpy(block_parameters, block_delimiter[i], block_delimiter[i + 1] - block_delimiter[i]);
        // Transform the parameters into a block
        SHAPE_PathAddBlock(&blocks, SHAPE_CreatePathBlock(block_parameters[0], &block_parameters[1]));
    }

    return blocks;
}

ShapePoint* SHAPE_GetPointsFromPathblocks(ShapePathblock* blocks, float step)
{
    // Go to the first point 
    ShapePoint* points = SHAPE_CreatePoint(blocks->p->x, blocks->p->y);
    float first_x = blocks->p->x, first_y = blocks->p->y;
    blocks = blocks->nb;

    while (blocks)
    {
        if (blocks->id == 'H')
        {
            ShapeLine* line = SHAPE_CreateLine(blocks->n, points->x, points->y, points->y);
            SHAPE_AddPoints(&points, SHAPE_GetPointsFromLine(line, step));
        }
        else if (blocks->id == 'h')
        {
            ShapeLine* line = SHAPE_CreateLine(points->x+blocks->n, points->x, points->y, points->y);
            SHAPE_AddPoints(&points, SHAPE_GetPointsFromLine(line, step));
        }

        else if (blocks->id == 'V')
        {
            ShapeLine* line = SHAPE_CreateLine(points->x, points->x, blocks->n, points->y);
            SHAPE_AddPoints(&points, SHAPE_GetPointsFromLine(line, step));
        }

        else if (blocks->id == 'v')
        {
            ShapeLine* line = SHAPE_CreateLine(points->x, points->x, blocks->n+points->y, points->y);
            SHAPE_AddPoints(&points, SHAPE_GetPointsFromLine(line, step));
        }

        else if (blocks->id == 'L')
        {
            ShapeLine* line = SHAPE_CreateLine(blocks->p->x, points->x, blocks->p->y, points->y);
            SHAPE_AddPoints(&points, SHAPE_GetPointsFromLine(line, step));
        }

        else if (blocks->id == 'Z')
        {
            ShapeLine* line = SHAPE_CreateLine(points->x, first_x, points->y, first_y);
            SHAPE_AddPoints(&points, SHAPE_GetPointsFromLine(line, step));
        }

        else if (blocks->id == 'C')
        {
            float x1, x2, x3, y1, y2, y3;
            x3 = blocks->p->x; y3 = blocks->p->y;
            x2 = blocks->p->np->x; y2 = blocks->p->np->y;
            x1 = blocks->p->np->np->x; y1 = blocks->p->np->np->y;

            ShapeCubicBezier* cubic_bezier = SHAPE_CreateCubicBezier(first_x, first_y, x1, y1, x2, y2, x3, y3);
            ShapePoint* cubic_bezier_points = SHAPE_GetPointsFromCubicBezier(cubic_bezier, step);
        
            SHAPE_AddPoints(&points, cubic_bezier_points);
        }


        blocks = blocks->nb;
    }

    return points;

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

    ShapePoint* cursor = malloc(sizeof(ShapePoint));
    memcpy(cursor, points_to_add, sizeof(ShapePoint));
    while (cursor->np)
    {
        cursor = cursor->np;
    }

    cursor->np = *points;
    *points = points_to_add;


    }

// Return mathematical coordinates of points making the abstract shape
ShapePoint* SHAPE_GetPointsFromAbstractShape(ShapeAbstract* abstract_shape, float step)
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

    else if (!strcmp(shape_type, "polygon"))
        return SHAPE_GetPointsFromPolygon(abstract_shape->data, step);
    
    else if (!strcmp(shape_type, "path"))
        return SHAPE_GetPointsFromPathblocks(abstract_shape->data, step);
}

ShapePoint* SHAPE_GetPointsFromAbstractShapes(ShapeAbstract* abstract_shape_stack, float step)
{
    ShapePoint* points = (ShapePoint*)calloc(sizeof(ShapePoint), 1);

    while (abstract_shape_stack)
    {
        SHAPE_AddPoints(&points, SHAPE_GetPointsFromAbstractShape(abstract_shape_stack, step));
        abstract_shape_stack = abstract_shape_stack->next;
    }

    return points;
}



