//
// Created by an0n1mity on 16/11/2021.
//

#include <stdio.h>
#include "svg.h"
#include "shapes.h"

int main() {
    // Create a polyline
    SHAPE_Polyline** polyline = SHAPE_CreatePolyline(4, 1, 1, 2, 2);
    printf("%d\n", (*polyline)->next->x);
    return 0;
}