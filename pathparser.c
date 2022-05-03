//
// Created by biist on 02/05/2022.
//

#include "pathparser.h"

ShapePathblock* ParsePath(svgAttributeStack* path_attribute)
{
    ShapePathblock* path_blocks = NULL;

    char* delimiters[1000] = {0};
    int delimiters_idx[1000] = {0};
    //char* end_delimiter = "\0";
    //strcat(path_attribute->value, end_delimiter);
    size_t pathlen = strlen(path_attribute->value);
    int j = 0;
    for (int i = 0; i <= pathlen; ++i) {
        if (path_attribute->value[i] == 'M' || path_attribute->value[i] == 'H' ||
            path_attribute->value[i] == 'V' || path_attribute->value[i] == 'v' ||
            path_attribute->value[i] == 'L' || path_attribute->value[i] == 'Z' ||
            path_attribute->value[i] == 'h' || path_attribute->value[i] == 'v' ||
            path_attribute->value[i] == 'C' || path_attribute->value[i] == 'Q' ||
            path_attribute->value[i] == 'c' || path_attribute->value[i] == 'z' ||
            path_attribute->value[i] == 'l' || path_attribute->value[i] == 's' ||
            path_attribute->value[i] == 'S' || path_attribute->value[i] == '\0' ||
                path_attribute->value[i] == 'm'
                )
        {
            delimiters[j] = &path_attribute->value[i];
            delimiters_idx[j] = i;
            j++;
        }
    }


    // We are going to parse each string between delimiters
    ptrdiff_t bytes;
    uint64_t char_idx;
    char x[10] = {0};
    char y[10] = {0};
    bool reading_x, reading_y;
    int nb = 0;
    float x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;
    for (int i = 0; i < j-1; ++i) {
        bytes = delimiters_idx[i+1] - delimiters_idx[i];//((char *)delimiters[i+1]) - ((char *)delimiters[i]);
        char_idx = delimiters_idx[i];//((char *)delimiters[i]) - ((char *)delimiters[0]);
        reading_x = reading_y = false;
        nb = 0;
        memset(x, 0, 10);
        memset(y, 0, 10);
        for (int k = char_idx, l = 0, r = 0; k <= char_idx + bytes; ++k, r++){
            if(*delimiters[i] == 'M' || *delimiters[i] == 'm' || *delimiters[i] == 'L' || *delimiters[i] == 'l' || *delimiters[i] == 'T' || *delimiters[i] == 't')
            {
                if((isdigit(path_attribute->value[k]) || (path_attribute->value[k] == '-')) && !reading_x && !reading_y)
                    reading_x = true;

                else if(reading_x && (path_attribute->value[k] == ',' || path_attribute->value[k] == ' ' || path_attribute->value[k] == '-')) {
                    reading_x = false;
                    reading_y = true;
                    if(path_attribute->value[k] == ',' || path_attribute->value[k] == ' ')
                        k++;
                    l = 0;
                }

                else if(reading_y && (path_attribute->value[k] == *delimiters[i+1] || path_attribute->value[k] == ' '))
                {
                    x0 = atof(x);
                    y0 = atof(y);
                    reading_x = false;
                    reading_y = false;
                    memset(x, 0, sizeof(char)*10);
                    memset(x, 0, sizeof(char)*10);
                    l = 0;
                    SHAPE_PathAddBlock(&path_blocks, SHAPE_CreatePathBlockWithPoints(*delimiters[i], SHAPE_CreatePoint(x0, y0)));

                    printf("\n%c %f %f\n", *delimiters[i], x0, y0);

                }

                if((isdigit(path_attribute->value[k]) || (path_attribute->value[k] == '.') || path_attribute->value[k] == '-')) {
                    if (reading_x)
                        x[l] = path_attribute->value[k];
                    else
                        y[l] = path_attribute->value[k];
                    l++;
                }

            }

            else if(*delimiters[i] == 'h' || *delimiters[i] == 'v' || *delimiters[i] == 'H' || *delimiters[i] == 'V') {
                if((isdigit(path_attribute->value[k]) || (path_attribute->value[k] == '-')) && !reading_x && !reading_y)
                    reading_x = true;

                if(path_attribute->value[k] == *delimiters[i+1])
                {
                    x0 = atof(x);
                    SHAPE_PathAddBlock(&path_blocks, SHAPE_CreatePathBlockWithPoints(*delimiters[i], SHAPE_CreatePoint(x0, 0)));

                    printf("\n%c %f\n", *delimiters[i], x0);
                }

                if ((isdigit(path_attribute->value[k]) || (path_attribute->value[k] == '.') || (path_attribute->value[k] == '-')) && reading_x){
                    x[l] = path_attribute->value[k];
                    l++;
                }
            }

            else if(*delimiters[i] == 'S' || *delimiters[i] == 's')
            {
                if((isdigit(path_attribute->value[k]) || (path_attribute->value[k] == '-')) && !reading_x && !reading_y)
                    reading_x = true;

                else if(reading_x && (path_attribute->value[k] == ',' || path_attribute->value[k] == ' ' || path_attribute->value[k] == '-')) {
                    if(path_attribute->value[k] == ',' || path_attribute->value[k] == ' ')
                        k++;
                    reading_x = false;
                    reading_y = true;
                    l = 0;
                }

                else if(reading_y && (path_attribute->value[k] == ',' || path_attribute->value[k] == ' ' || path_attribute->value[k] == '-' || path_attribute->value[k] == *delimiters[i+1] )) {
                    if(path_attribute->value[k] == ',' || path_attribute->value[k] == ' ')
                        k++;
                    reading_x = true;
                    reading_y = false;
                    l = 0;

                    switch (nb){
                        // The second control point
                        case 0:
                            x0 = atof(x);
                            y0 = atof(y);
                            break;
                            // The end point
                        case 1:
                            x1 = atof(x);
                            y1 = atof(y);
                            break;
                    }
                    memset(x, 0, 10);
                    memset(y, 0, 10);
                    nb++;
                }

                if(nb > 1)
                {
                    ShapePoint* p0 = SHAPE_CreatePoint(x0, y0);
                    SHAPE_AddPoint(&p0, SHAPE_CreatePoint(x1, y1));
                    SHAPE_PathAddBlock(&path_blocks, SHAPE_CreatePathBlockWithPoints(*delimiters[i], p0));

                    reading_x = true;
                    reading_y = false;
                    nb = 0;
                    printf("\n%c %f %f, %f %f\n", *delimiters[i], x0, y0, x1, y1);
                }

                if ((isdigit(path_attribute->value[k]) || (path_attribute->value[k] == '.') || (path_attribute->value[k] == '-'))){
                    if(reading_x)
                        x[l] = path_attribute->value[k];
                    else
                        y[l] = path_attribute->value[k];
                    l++;
                }
            }

            else if(*delimiters[i] == 'c' || *delimiters[i] == 'C')
            {
                if((isdigit(path_attribute->value[k]) || (path_attribute->value[k] == '-')) && !reading_x && !reading_y)
                    reading_x = true;

                else if(reading_x && (path_attribute->value[k] == ',' || path_attribute->value[k] == ' ' || path_attribute->value[k] == '-')) {
                    reading_x = false;
                    reading_y = true;
                    if(path_attribute->value[k] == ',' || path_attribute->value[k] == ' ')
                        k++;
                    l = 0;
                }

                else if(reading_y && (path_attribute->value[k] == ',' || path_attribute->value[k] == ' ' || path_attribute->value[k] == '-' || path_attribute->value[k] == *delimiters[i+1] )) {
                    reading_x = true;
                    reading_y = false;
                    if(path_attribute->value[k] == ',' || path_attribute->value[k] == ' ')
                        k++;
                    l = 0;

                    switch (nb){
                        case 0:
                            x0 = atof(x);
                            y0 = atof(y);
                            break;
                        case 1:
                            x1 = atof(x);
                            y1 = atof(y);
                            break;
                        case 2:
                            x2 = atof(x);
                            y2 = atof(y);
                            break;
                    }
                    memset(x, 0, 10);
                    memset(y, 0, 10);
                    nb++;
                }

                if(nb > 2)
                {
                    ShapePoint* p0 = SHAPE_CreatePoint(x0, y0);
                    SHAPE_AddPoint(&p0, SHAPE_CreatePoint(x1, y1));
                    SHAPE_AddPoint(&p0, SHAPE_CreatePoint(x2, y2));
                    SHAPE_PathAddBlock(&path_blocks, SHAPE_CreatePathBlockWithPoints(*delimiters[i], p0));

                    printf("\n%c %f %f, %f %f, %f %f\n", *delimiters[i], x0, y0, x1, y1, x2, y2);
                    //reading_x = reading_y = false;
                    nb = 0;
                }

                if ((isdigit(path_attribute->value[k]) || (path_attribute->value[k] == '.') || (path_attribute->value[k] == '-')) && (reading_x || reading_y)){
                    if(reading_x)
                        x[l] = path_attribute->value[k];
                    else
                        y[l] = path_attribute->value[k];
                    l++;
                }
            }
        }

        //printf("%c\n", *delimiters[i]);
       // ShapePoint* points = SHAPE_GetPointsFromPathblocks(path_blocks, 1.f);

    }

    return path_blocks;
}

