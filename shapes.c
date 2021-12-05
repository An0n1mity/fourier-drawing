#define _CRT_SECURE_NO_WARNINGS

#include <stdbool.h>
#include <string.h>
#include "shapes.h"
#include "svgparser.h"


ShapeAbstract* SHAPE_CreateAbstract(char* type, void* data)
{
    ShapeAbstract* abstract_shape = (ShapeAbstract*)calloc(sizeof(ShapeAbstract), 1);
    if (!abstract_shape)
        return NULL;

    strcpy(abstract_shape->type, type);
    abstract_shape->data = data;

    return abstract_shape;
}

ShapePoint* SHAPE_CreatePoint(float x, float y)
{
    ShapePoint* point = (ShapePoint*)calloc(sizeof(ShapePoint), 1);
    point->x = x, point->y = y;
    return point;
}

void SHAPE_AddShapeToList(ShapeAbstract** abstract_shape_list, char* type, void* data)
{
    ShapeAbstract* abstract_shape_to_add = SHAPE_CreateAbstract(type, data);
    abstract_shape_to_add->next = *abstract_shape_list;
    *abstract_shape_list = abstract_shape_to_add;
}


ShapeRectangle* SHAPE_CreateRectangle(float x, float y, float w, float h, float rx, float ry)
{
    ShapeRectangle* rectangle = (ShapeRectangle*)malloc(sizeof(ShapeRectangle));
    ShapeRectangle parameters = { {x, y}, w, h, rx, ry };
    
    if (!rectangle)
        return NULL;
    
    *rectangle = parameters;

    return rectangle;
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

ShapeEllipse* SHAPE_CreateEllipse(float cx, float cy, float rx, float ry)
{
    ShapeEllipse* ellipse = (ShapeEllipse*)malloc(sizeof(ShapeEllipse));
    ShapeEllipse parameters = { {cx, cy}, rx, ry };

    if (!ellipse)
        return;

    *ellipse = parameters;
    return ellipse;
}

ShapePolyline* SHAPE_CreatePolyline(char* points)
{
    ShapePolyline* polyline = (ShapePolyline*)calloc(sizeof(ShapePolyline), 1);
    if (!polyline)
        return;
    ShapePoint* head = polyline->p;
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
            ShapePoint* point_to_add = SHAPE_CreatePoint(x, y);
            SHAPE_AddPoint(&polyline->p, point_to_add);

        }
        token = strtok(NULL, " ");
    }

    return polyline;
}

ShapePolygone* SHAPE_CreatePolygone(char* points)
{

    ShapePolygone* polygone = (ShapePolygone*)calloc(sizeof(ShapePolygone), 1);
    ShapePoint* point_to_add = NULL;
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
ShapePath* CreatePathFromSVGPath(svgAttributeStack* path_attribute)
{
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
                return;
            path_bloc_data[end_index - start_index - 1] = '\0';
            strncpy(path_bloc_data, path_attribute->value + start_index + 1, end_index - start_index - 1);

            // Parsing points data
            char* save_point_data = NULL, * save_x_y_data = NULL;
            char* point_data = strtok_s(path_bloc_data, " ", &save_point_data);
            char* x_y_data;
            float x, y;
            while (point_data)
            {
                x_y_data = strtok_s(point_data, ",", &save_x_y_data);
                x = atof(x_y_data);
                x_y_data = strtok_s(NULL, ",", &save_x_y_data);
                y = atof(x_y_data);
                ShapePoint* point_to_add = SHAPE_CreatePoint(x, y);

                // Add the new created points to the current path block
                SHAPE_AddPoint(&path_block->p, point_to_add);
                point_data = strtok_s(NULL, " ", &save_point_data);

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

    ShapePoint* cursor = *points;
    while (cursor->np)
        cursor = cursor->np;

    cursor->np = point_to_add;

}

