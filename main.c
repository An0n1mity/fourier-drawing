//
// Created by an0n1mity on 16/11/2021.
//

#include <stdio.h>
#include "svg.h"
#include "shapes.h"

int main() {
    // Create a polyline
    SHAPE_Polyline* polyline = SHAPE_CreatePolyline(4, 1.f, 1.f, 2.f, 2.f);
    printf("%f\n", polyline->p->np->x);
    return 0;
}