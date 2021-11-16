//
// Created by Alexandre on 14/11/2021.
//

#ifndef SVG_PARSER_SVG_H
#define SVG_PARSER_SVG_H

#include "stdlib.h"
#include "shapes.h"

typedef struct SVG_Image_s SVG_Image;

typedef struct SVG_Shape_s SVG_Shape;

struct SVG_Image_s{
    size_t width;
    size_t height;
    char* data;
};

// Shapes are stored as linked list
struct SVG_Shape_s{
    char type[10];

    void* data;
    struct SVG_Shape_s* next_shape;
};

#endif //SVG_PARSER_SVG_H
