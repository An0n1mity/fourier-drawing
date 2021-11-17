//
// Created by Alexandre on 14/11/2021.
//

#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
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
    SHAPE_Rectangle parameters = {{x, y}, w, h, rx, ry};
    *rectangle = parameters;

    return rectangle;
}

SHAPE_Circle* SHAPE_CreateCircle(float cx, float cy, float r)
{
    SHAPE_Circle* circle = malloc(sizeof(SHAPE_Circle));
    SHAPE_Circle parameters = {{cx, cy}, r};
    *circle = parameters;
    return circle;
}

SHAPE_Ellipse* SHAPE_CreateEllipse(float cx, float cy, float rx, float ry)
{
    SHAPE_Ellipse* ellipse = malloc(sizeof(SHAPE_Ellipse));
    SHAPE_Ellipse parameters = {{cx, cy}, rx, ry};
    *ellipse = parameters;
    return ellipse;
}

SHAPE_Polyline* SHAPE_CreatePolyline(char* points)
{
    SHAPE_Polyline* polyline = (SHAPE_Polyline*)calloc(sizeof(SHAPE_Polyline), 1);
    SHAPE_Point* head = polyline->p;
    char argument[100]; strcpy(argument, points);

    char* token = strtok(argument, " ");
    bool read_x = true, add_point = false;
    float x, y;
    while(token){
        if(read_x){
            x = atof(token);
            read_x = false;
            add_point = false;
        }
        else{
            y = atof(token);
            read_x = true;
            add_point = true;
        }

        if(add_point){
            polyline->p = SHAPE_PolylineAddPoint(&polyline->p, x, y);

        }
        token = strtok(NULL, " ");
    }

    return polyline;
}

SHAPE_Point* SHAPE_PolylineAddPoint(SHAPE_Point** point, float x, float y){
    SHAPE_Point* point_to_add = (SHAPE_Point*)calloc(sizeof(SHAPE_Point), 1);
    point_to_add->x = x; point_to_add->y = y;

    if(!(*point))
    {
        (*point) = point_to_add;
        return *point;
    }

    point_to_add->np = (*point);
    return point_to_add;
}

SHAPE_Polygone* SHAPE_CreatePolygone(char* points)
{
    SHAPE_Polygone* polygone = (SHAPE_Polygone*)calloc(sizeof(SHAPE_Polygone), 1);
    char argument[100]; strcpy(argument, points);
    bool read_x = true, add_point = false, read_first = true;
    float x_first_last, y_first_last, x, y;
    char* token = strtok(argument, " ");
    while(token){
        if(read_x){
            x = atof(token);
            if(read_first)
                x_first_last = x;
            read_x = false;
            add_point = false;
        }

        else{
            y = atof(token);
            if(read_first){
                y_first_last = y_first_last;
                read_first = false;
            }
            read_x = true;
            add_point = true;
        }

        if(add_point)
            polygone->p = SHAPE_PolygoneAddPoint(&polygone->p, x, y);

        token = strtok(NULL, " ");
    }

    SHAPE_PolygoneAddPoint(&polygone->p, x_first_last, y_first_last);

    return polygone;
}

SHAPE_Point* SHAPE_PolygoneAddPoint(SHAPE_Point** point, float x, float y)
{
    SHAPE_Point* point_to_add = (SHAPE_Point*)malloc(sizeof(SHAPE_Point));
    point_to_add->x = x;
    point_to_add->y = y;
    if(!(*point)){
        *point = point_to_add;
        return *point;
    }

    point_to_add->np = (*point);
    return point_to_add;
}

SHAPE_Path* SHAPE_CreatePath(char* points)
{
    SHAPE_Path* path = (SHAPE_Path*)malloc(sizeof(SHAPE_Path));
    char argument[100]; strcpy(argument, points);

    // Read each block and stock it in the Path block list
    char* token = strtok(argument, " ");
    char current_block;
    SHAPE_Pathblock* pathblock;
    bool read_x = true, add_point = false, add_block = false;
    int readed_blocks = 0;
    float x, y;
    while(token){
        // If it is a command, create a new block based on that command
        switch (*token) {
            case 'M':
                if(readed_blocks > 0)
                    path->b = SHAPE_PathAddBlock(&path->b, pathblock);
                pathblock = (SHAPE_Pathblock *) malloc(sizeof(SHAPE_Pathblock));
                pathblock->id = 'M';
                readed_blocks++;
                break;


            default:
                // Add points to the current block
                if(read_x)
                {
                    x = atof(token);
                    read_x = false;
                    add_point = false;
                }
                else{
                    y = atof(token);
                    read_x = true;
                    add_point = true;
                }
                break;
        }
        if(add_point)
            pathblock->p = SHAPE_PathblockAddPoint(&pathblock->p, x, y);
        token = strtok(NULL, " ");
    }
    path->b = SHAPE_PathAddBlock(&path->b, pathblock);
    return path;
}

SHAPE_Pathblock* SHAPE_PathAddBlock(SHAPE_Pathblock** block, SHAPE_Pathblock* block_to_add)
{
    // If there is no block in the list
    if(!(*block)){
        (*block) = block_to_add;
        return (*block);
    }

    block_to_add->nb = (*block);
    return block_to_add;
}

SHAPE_Point* SHAPE_PathblockAddPoint(SHAPE_Point** points, float x, float y)
{
    SHAPE_Point* point_to_add = (SHAPE_Point*)calloc(sizeof(SHAPE_Point), 1);
    point_to_add->x = x; point_to_add->y = y;

    if(!(*points)){
        *points = point_to_add;
        return *points;
    }

    (*points)->np = point_to_add;
    return point_to_add;
}

