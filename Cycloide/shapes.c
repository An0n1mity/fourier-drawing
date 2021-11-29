#define _CRT_SECURE_NO_WARNINGS

#include <stdbool.h>
#include <string.h>
#include "shapes.h"


SHAPE_Abstract* SHAPE_CreateAbstract(char* type, void* data)
{
    SHAPE_Abstract* abstract_shape = (SHAPE_Abstract*)calloc(sizeof(SHAPE_Abstract), 1);
    if (!abstract_shape)
        return NULL;

    strcpy(abstract_shape->type, type);
    abstract_shape->data = data;

    return abstract_shape;
}


void SHAPE_AddShapeToList(SHAPE_Abstract** abstract_shape_list, char* type, void* data)
{
    SHAPE_Abstract* abstract_shape_to_add = SHAPE_CreateAbstract(type, data);
    abstract_shape_to_add->next = *abstract_shape_list;
    *abstract_shape_list = abstract_shape_to_add;
}


SHAPE_Rectangle* SHAPE_CreateRectangle(float x, float y, float w, float h, float rx, float ry)
{
    SHAPE_Rectangle* rectangle = (SHAPE_Rectangle*)malloc(sizeof(SHAPE_Rectangle));
    SHAPE_Rectangle parameters = { {x, y}, w, h, rx, ry };
    
    if (!rectangle)
        return NULL;
    
    *rectangle = parameters;

    return rectangle;
}

SHAPE_Circle* SHAPE_CreateCircle(float cx, float cy, float r)
{
    SHAPE_Circle* circle = (SHAPE_Circle*)malloc(sizeof(SHAPE_Circle));
    SHAPE_Circle parameters = { {cx, cy}, r };

    if (!circle)
        return NULL;

    *circle = parameters;
    return circle;
}

SHAPE_Ellipse* SHAPE_CreateEllipse(float cx, float cy, float rx, float ry)
{
    SHAPE_Ellipse* ellipse = (SHAPE_Ellipse*)malloc(sizeof(SHAPE_Ellipse));
    SHAPE_Ellipse parameters = { {cx, cy}, rx, ry };

    if (!ellipse)
        return;

    *ellipse = parameters;
    return ellipse;
}

SHAPE_Polyline* SHAPE_CreatePolyline(char* points)
{
    SHAPE_Polyline* polyline = (SHAPE_Polyline*)calloc(sizeof(SHAPE_Polyline), 1);
    if (!polyline)
        return;
    SHAPE_Point* head = polyline->p;
    char argument[100]; strcpy(argument, points);

    char* token = strtok(argument, " ");
    bool read_x = true, add_point = false;
    float x, y;
    while (token) {
        if (read_x) {
            x = atof(token);
            read_x = false;
            add_point = false;
        }
        else {
            y = atof(token);
            read_x = true;
            add_point = true;
        }

        if (add_point) {
            polyline->p = SHAPE_AddPoint(&polyline->p, x, y);

        }
        token = strtok(NULL, " ");
    }

    return polyline;
}

SHAPE_Polygone* SHAPE_CreatePolygone(char* points)
{
    SHAPE_Polygone* polygone = (SHAPE_Polygone*)calloc(sizeof(SHAPE_Polygone), 1);
    if (!polygone)
        return;
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

        if (add_point)
            polygone->p = SHAPE_AddPoint(&polygone->p, x, y);

        token = strtok(NULL, " ");
    }

    SHAPE_AddPoint(&polygone->p, x_first_last, y_first_last);

    return polygone;
}

SHAPE_Path* SHAPE_CreatePath(char* points)
{
    SHAPE_Path* path = (SHAPE_Path*)malloc(sizeof(SHAPE_Path));
    char argument[100]; strcpy(argument, points);

    // Read each block and stock it in the Path block list
    char* token = strtok(argument, " ");
    SHAPE_Pathblock* pathblock = NULL;
    bool read_x = true, add_point = false, add_block = false;
    int readed_blocks = 0;
    float x, y;
    while (token) {
        // If it is a command, create a new block based on that command
        switch (*token) {
        case 'M':
            if (readed_blocks > 0)
                path->b = SHAPE_PathAddBlock(&path->b, pathblock);
            pathblock = (SHAPE_Pathblock*)malloc(sizeof(SHAPE_Pathblock));
            if(pathblock)
                pathblock->id = 'M';
            readed_blocks++;
            break;


        default:
            // Add points to the current block
            if (read_x)
            {
                x = atof(token);
                read_x = false;
                add_point = false;
            }
            else {
                y = atof(token);
                read_x = true;
                add_point = true;
            }
            break;
        }
        if (add_point)
            pathblock->p = SHAPE_AddPoint(&pathblock->p, x, y);
        token = strtok(NULL, " ");
    }
    if(path)
        path->b = SHAPE_PathAddBlock(&path->b, pathblock);
    return path;
}

SHAPE_Pathblock* SHAPE_PathAddBlock(SHAPE_Pathblock** block, SHAPE_Pathblock* block_to_add)
{
    // If there is no block in the list
    if (!(*block)) {
        (*block) = block_to_add;
        return (*block);
    }

    block_to_add->nb = (*block);
    return block_to_add;
}

SHAPE_Point* SHAPE_AddPoint(SHAPE_Point** points, float x, float y)
{
    SHAPE_Point* point_to_add = (SHAPE_Point*)calloc(sizeof(SHAPE_Point), 1);
    if (!point_to_add)
        return NULL;
    point_to_add->x = x; point_to_add->y = y;

    if (!(*points)) {
        *points = point_to_add;
        return *points;
    }

    (*points)->np = point_to_add;
    return point_to_add;
}
