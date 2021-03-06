//
// Created by biist on 06/05/2022.
//

#ifndef CYCLOIDE_GUI_H
#define CYCLOIDE_GUI_H

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include "fft.h"
#include "audio.h"

struct UserData_s{
    char* filename;
    GdkPixbuf* svgpixbuf;
    GtkDrawingArea* drawing_area;
    gboolean drawsvg;
    gboolean drawcycloides;
    int width;
    int height;

    int precision;
    gboolean turn;
    float actual_time;
    float previous_time;
    struct FFT_s* fft_array;
    ShapePoint* points_list;
    size_t nb_points;

    PaStream* stream;
    paData* pa_data;
    gboolean playfrequency;
};

void GetFileNameFromFileChooser(GtkFileChooser* file_chooser, gpointer user_data);
void GetPrecisionFromScale(GtkScale* precision_scale, gpointer user_data);
void DrawOnScreen(GtkDrawingArea* drawing_area, cairo_t* cr, gpointer user_data);
void DrawSVG(cairo_t* cr, gpointer user_data);
void PointsCheckButton(GtkCheckButton* button, gpointer user_data);
ShapePoint* DrawEpicycloides(cairo_t* cr, gpointer user_data);
gint ForceRenderUpdate(gpointer user_data);
void DrawPoints(cairo_t* cr, gpointer user_data);
void SVGCheckButton(GtkCheckButton* button, gpointer user_data);
void TakeScreenshotOfDrawing(GtkButton* button, gpointer user_data);
void FrequencyCheckButton(GtkCheckButton* button, gpointer user_data);
void CycloidesCheckButton(GtkCheckButton* button, gpointer user_data);

#endif //CYCLOIDE_GUI_H
