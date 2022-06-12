#include "shapes.h"
#include "svgparser.h"
#include "GUI.h"
#include <inttypes.h>
#include "fft.h"
#include "audio.h"

int main(int argc, char** argv) {
    // Get the sets of points from the abstract shapes this operation frees the abstract shapes stack
    size_t nb_points = 0;
    ShapePoint *points = SHAPE_GetPointsFromAbstractShapes(abstract_shapes, 0.1f, &nb_points);
    kiss_fft_cpx* complex_array = GetComplexArrayFromPoints(points, nb_points);
    struct FFT_s* fft = GetFFTOfComplexArray(complex_array, nb_points);
    
    Pa_Initialize();

    PaStream* stream;

    PaStreamParameters output_parameters;
    output_parameters.device = Pa_GetDefaultOutputDevice();
    output_parameters.channelCount = 2;      
    output_parameters.sampleFormat = paFloat32;
    output_parameters.suggestedLatency = Pa_GetDeviceInfo( output_parameters.device )->defaultLowOutputLatency;
    output_parameters.hostApiSpecificStreamInfo = NULL;

    paData data;
    data.idx = 0;

    Pa_OpenStream(
              &stream,
              NULL, 
              &output_parameters,
              SAMPLE_RATE,
              FRAMES_PER_BUFFER,
              paClipOff,    
              paStreamCallback,
              &data );

    Pa_StartStream(stream);

    gtk_init(&argc, &argv);

    GtkBuilder* builder = gtk_builder_new ();
    GError* error = NULL;
    struct UserData_s user_data = {0};
    user_data.stream = stream;
    user_data.precision = 1;
    user_data.pa_data = &data;

    gtk_builder_add_from_file (builder, "../gui.glade", &error);
    if(error)
    {
        g_error ("%s", error->message);
    }
    GtkWidget * window = (GtkWidget *) gtk_builder_get_object (builder, "window");
    GtkFileChooserButton* file_chooser_button = (GtkFileChooserButton *) gtk_builder_get_object (builder, "file_chooser");
    GtkScale* precision_scale = (GtkScale *) gtk_builder_get_object (builder, "precision_scale");
    GtkDrawingArea* drawing_area = (GtkDrawingArea *) gtk_builder_get_object (builder, "drawing_area");
    GtkCheckButton* svg_check_button = (GtkCheckButton *) gtk_builder_get_object (builder, "svg_check_button");
    GtkButton* screenshot_button = (GtkButton*) gtk_builder_get_object (builder, "screenshot_button");
    user_data.drawing_area = drawing_area;

    g_timeout_add (10 /* milliseconds */, ForceRenderUpdate, &user_data);
    g_signal_connect (G_OBJECT (file_chooser_button), "selection-changed", G_CALLBACK (GetFileNameFromFileChooser), &user_data);
    g_signal_connect(G_OBJECT(precision_scale), "value_changed", G_CALLBACK( GetPrecisionFromScale), &user_data);
    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(DrawOnScreen), &user_data);
    g_signal_connect(G_OBJECT(svg_check_button), "clicked", G_CALLBACK(SVGCheckButton), &user_data);
    g_signal_connect(G_OBJECT(screenshot_button), "clicked", G_CALLBACK(TakeScreenshotOfDrawing), &user_data);

    gtk_widget_show_all (window);
    gtk_main ();
    return 0;
