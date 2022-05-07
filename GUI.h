//
// Created by biist on 06/05/2022.
//

#ifndef CYCLOIDE_GUI_H
#define CYCLOIDE_GUI_H

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include "fft.h"

struct UserData_s{
    char* filename;
    GdkPixbuf* svgpixbuf;
    GtkDrawingArea* drawing_area;

    int precision;
    gboolean turn;
    float actual_time;
    float previous_time;
    struct FFT_s* fft_array;
    ShapePoint* points_list;
    size_t nb_points;
};

void GetFileNameFromFileChooser(GtkFileChooser* file_chooser, gpointer user_data);
void GetPrecisionFromScale(GtkScale* precision_scale, gpointer user_data);
void DrawOnScreen(GtkDrawingArea* drawing_area, cairo_t* cr, gpointer user_data);
void DrawSVG(cairo_t* cr, gpointer user_data);
ShapePoint* DrawEpicycloides(cairo_t* cr, gpointer user_data);
gint ForceRenderUpdate(gpointer user_data);
void DrawPoints(cairo_t* cr, gpointer user_data);

#endif //CYCLOIDE_GUI_H
