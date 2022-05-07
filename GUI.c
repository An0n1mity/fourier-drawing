//
// Created by biist on 06/05/2022.
//

#include "GUI.h"

void GetFileNameFromFileChooser(GtkFileChooser* file_chooser, gpointer user_data)
{
    gchar *filename = gtk_file_chooser_get_filename (file_chooser);
    struct UserData_s* data = (struct UserData_s*)user_data;
    if (!filename) return;

    // Get the file name of the svg
    data->filename = filename;

    // Load svg into a pixbuffer
    data->svgpixbuf = gdk_pixbuf_new_from_file_at_scale(filename, 800, 800, TRUE, NULL);
    //data->svgpixbuf = gdk_pixbuf_new_from_file (filename, NULL);
    //data->svgpixbuf = gdk_pixbuf_scale_simple(data->svgpixbuf, 800, 800, GDK_INTERP_BILINEAR);

    // Calculate the FFT of all the svg points
    // Load the svg file in memory
    xmlDocPtr svgfile = PARSER_LoadSVG(filename);
    // Parse the svg file and create SVG shapes with attributes of the file
    svgShapeStack *svg_shapes = PARSER_GetShapesFromSVG(svgfile);
    // Transform to mathematical represnetation from svg shapes
    ShapeAbstract *abstract_shapes = SHAPE_CreateAbstractFromSVG(svg_shapes);

    // Get the sets of points from the abstract shapes this operation frees the abstract shapes stack
    size_t nb_points = 0;
    ShapePoint *points = SHAPE_GetPointsFromAbstractShapes(abstract_shapes, 0.1f, &nb_points);
    kiss_fft_cpx* complex_array = GetComplexArrayFromPoints(points, nb_points);
    data->fft_array = GetFFTOfComplexArray(complex_array, nb_points);
    data->nb_points = nb_points;

    gtk_widget_queue_draw(GTK_WIDGET(data->drawing_area));

}

void GetPrecisionFromScale(GtkScale* precision_scale, gpointer user_data)
{
    struct UserData_s* data = (struct UserData_s*)user_data;
    data->precision = (int)gtk_range_get_value(GTK_RANGE(precision_scale));
    SHAPE_FreePoints(data->points_list);
    data->points_list = NULL;
    data->turn = false;
    data->previous_time = data->actual_time;
}

gint ForceRenderUpdate(gpointer user_data)
{
    struct UserData_s* data = (struct UserData_s*)user_data;
    gtk_widget_queue_draw(GTK_WIDGET(data->drawing_area));

    return G_SOURCE_CONTINUE;

}

void DrawOnScreen(GtkDrawingArea* drawing_area, cairo_t* cr, gpointer user_data)
{
    struct UserData_s* data = (struct UserData_s*)user_data;
    DrawSVG(cr, user_data);
    if(data->fft_array)
        SHAPE_AddPoint(&data->points_list, DrawEpicycloides(cr, user_data));
    DrawPoints(cr, user_data);
}

void DrawSVG(cairo_t* cr, gpointer user_data)
{
    struct UserData_s* data = (struct UserData_s*)user_data;
    if(!data->svgpixbuf)
        return;

    gdk_cairo_set_source_pixbuf(cr, data->svgpixbuf, 0, 0);
    cairo_paint (cr);
}

void DrawPoints(cairo_t* cr, gpointer user_data)
{
    struct UserData_s* data = (struct UserData_s*)user_data;
    ShapePoint* points = data->points_list;
    cairo_set_source_rgba(cr, 1.f, 0, 0, 1.f);
    while(points)
    {
        cairo_arc(cr, points->x, points->y, 1.f, 0, 2 * M_PI);
        cairo_set_line_width(cr, 2.f);
        cairo_fill(cr);

        if(points->np)
        {
            cairo_move_to(cr, points->x, points->y);
            cairo_line_to(cr, points->np->x, points->np->y);
            cairo_stroke(cr);
        }

        points = points->np;
    }
}

ShapePoint* DrawEpicycloides(cairo_t* cr, gpointer user_data) {
    float amplitude, phase;
    size_t frequency;
    float x = 0, y = 0;
    static float prev_x, prev_y;
    struct UserData_s *data = (struct UserData_s *) user_data;
    const float dt = M_PI * 2 / (float) data->nb_points;

    if (data->actual_time >= 2 * M_PI){
        data->turn = TRUE;
        data->actual_time = 0.f;
    }


        for (size_t i = 0; i < (data->precision/100.f)*data->nb_points; i++) {
            prev_x = x;
            prev_y = y;

            amplitude = (float)data->fft_array[i].amplitude;
            phase =  (float)data->fft_array[i].phase;
            frequency = data->fft_array[i].frequency;

            x += amplitude * cosf(frequency *  data->actual_time + phase);
            y += amplitude * sinf(frequency *  data->actual_time + phase);

            cairo_set_source_rgba(cr, 0, 0, 0, 0.5);
            if(prev_x && prev_y) {
                cairo_move_to(cr, prev_x, prev_y);
                cairo_line_to(cr, x, y);
                cairo_stroke(cr);
            }
            cairo_arc(cr, x, y, amplitude, 0, 2 * M_PI);
            cairo_stroke(cr);
        }
        data->actual_time +=dt;

        if(!data->turn)
            return SHAPE_CreatePoint(x, y);
        return NULL;

}