//
// Created by biist on 02/05/2022.
//

#include "pathparser.h"

void ParsePath(svgAttributeStack* path_attribute)
{
    char* delimiters[1000] = {0};
    size_t pathlen = strlen(path_attribute->value);
    int j = 0;
    for (int i = 0; i < pathlen; ++i) {
        if (path_attribute->value[i] == 'M' || path_attribute->value[i] == 'H' ||
            path_attribute->value[i] == 'V' || path_attribute->value[i] == 'v' ||
            path_attribute->value[i] == 'L' || path_attribute->value[i] == 'Z' ||
            path_attribute->value[i] == 'h' || path_attribute->value[i] == 'v' ||
            path_attribute->value[i] == 'C' || path_attribute->value[i] == 'Q' ||
            path_attribute->value[i] == 'c' || path_attribute->value[i] == 'z' ||
            path_attribute->value[i] == 'l' || path_attribute->value[i] == 's' ||
            path_attribute->value[i] == 'S'
                )
        {
            delimiters[j] = &path_attribute->value[i];
            j++;
        }
    }

    // We are going to parse each string between delimiters
    ptrdiff_t bytes;
    ptrdiff_t char_idx;
    char x[20] = {0};
    char y[20] = {0};
    bool reading_x, reading_y;
    int nb = 0;
    float x0, y0, x1, y1, x2, y2, x3, y3, x4, y4;
    for (int i = 0; i < j; ++i) {
        bytes = ((char *)delimiters[i+1]) - ((char *)delimiters[i]);
        char_idx = ((char *)delimiters[i]) - ((char *)delimiters[0]);
        reading_x = reading_y = false;
        nb = 0;
        memset(x, 0, 20);
        memset(y, 0, 20);
        for (int k = char_idx, l = 0; k <= char_idx + bytes; ++k){
            if(*delimiters[i] == 'M' || *delimiters[i] == 'm' || *delimiters[i] == 'L' || *delimiters[i] == 'l' || *delimiters[i] == 'T' || *delimiters[i] == 't')
            {
                if(isdigit(path_attribute->value[k]) && !reading_x && !reading_y)
                    reading_x = true;
                if(reading_x && (path_attribute->value[k] == ',' || path_attribute->value[k] == ' ' || path_attribute->value[k] == '-')) {
                    x0 = atof(x);
                    reading_x = false;
                    reading_y = true;
                    l = 0;
                }

                else if(reading_y && path_attribute->value[k] == *delimiters[i+1])
                {
                    y0 = atof(y);
                    printf("%c %f %f\n", *delimiters[i], x0, y0);
                }

                if((isdigit(path_attribute->value[k]) || (path_attribute->value[k] == '.') || path_attribute->value[k] == '-')) {
                    if (reading_x)
                        x[l] = path_attribute->value[k];
                    else
                        y[l] = path_attribute->value[k];
                    l++;
                }

            }

            else if(*delimiters[i] == 'h' || *delimiters[i] == 'v') {
                if(isdigit(path_attribute->value[k]) || (path_attribute->value[k] == '-') && !reading_x && !reading_y)
                    reading_x = true;

                if(path_attribute->value[k] == *delimiters[i+1])
                {
                    x0 = atof(x);
                    printf("%c %f\n", *delimiters[i], x0);
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
                    reading_x = false;
                    reading_y = true;
                    l = 0;
                }

                else if(reading_y && (path_attribute->value[k] == ',' || path_attribute->value[k] == ' ' || path_attribute->value[k] == '-' || path_attribute->value[k] == *delimiters[i+1] )) {
                    reading_x = true;
                    reading_y = false;
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
                    }
                    memset(x, 0, 20);
                    memset(y, 0, 20);
                    nb++;
                }

                if(nb > 1)
                {
                    printf("%c %f %f, %f %f\n", *delimiters[i], x0, y0, x1, y1);
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
                    l = 0;
                }

                else if(reading_y && (path_attribute->value[k] == ',' || path_attribute->value[k] == ' ' || path_attribute->value[k] == '-' || path_attribute->value[k] == *delimiters[i+1] )) {
                    reading_x = true;
                    reading_y = false;
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
                    memset(x, 0, 20);
                    memset(y, 0, 20);
                    nb++;
                }

                if(nb > 2)
                {
                    printf("%c %f %f, %f %f, %f %f\n", *delimiters[i], x0, y0, x1, y1, x2, y2);
                    reading_x = reading_y = false;
                    nb = 0;
                    continue;
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

    }
}