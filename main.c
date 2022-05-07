
#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include "shapes.h"
#include "svgparser.h"
#include "GUI.h"
#include <inttypes.h>
#include "fft.h"


#define gtk
//#include "svgpathparser.h"

void parseSVGPath(char* path);

void RENDERER_DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
    const int32_t diameter = (radius * 2);

    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    while (x >= y)
    {
        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

        if (error <= 0)
        {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0)
        {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}

void RENDERER_DrawPoints(SDL_Renderer* renderer, ShapePoint* points)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    ShapePoint* cursor = points;
    while (cursor->np)
    {
        SDL_RenderDrawPointF(renderer, cursor->x, cursor->y);
        if(cursor->np)
            SDL_RenderDrawLineF(renderer, cursor->x, cursor->y, cursor->np->x, cursor->np->y);
        cursor = cursor->np;
    }
}

ShapePoint* RENDERER_DrawEpycyles(SDL_Renderer* renderer, DFT* dft, size_t size, float rotation, float x, float y)
{
    float prev_x, prev_y;
    static float time = 0.f;
    const float dt = M_PI*2/(float)size;
    static bool turn = false;

    if(time >= 5.f){
        time = 0.f;
        turn = true;
    }
    for (int i = 0; i < size-1000; i+=1) {
        prev_x = x, prev_y = y;
        x += dft[i].amplitude * cosf(dft[i].frequency * time +  dft[i].phase + rotation);
        y += dft[i].amplitude * sinf(dft[i].frequency * time + dft[i].phase + rotation);

        SDL_RenderDrawLineF(renderer, prev_x, prev_y,x, y);
        RENDERER_DrawCircle(renderer, prev_x, prev_y, (float)dft[i].amplitude*2);
    }
    time += dt;
    if(!turn)
        return SHAPE_CreatePoint(x, y);
    return NULL;
}

ShapePoint* Test(SDL_Renderer* renderer, kiss_fft_cpx* complex_fft_array, struct FFT_s* fft, size_t nbcomplex)
{
    double amplitude, phase;
    size_t frequency;
    float x = 0, y = 0;
    static float prev_x, prev_y;
    static float time = 0.f;
    if(time >= 2*M_PI)
        time = 0.f;
    const float dt = M_PI*2/(float)nbcomplex;

    for (size_t i = 0; i < nbcomplex; i+=1) {
        prev_x = x;
        prev_y = y;

        amplitude = fft[i].amplitude;
        phase = fft[i].phase;
        frequency = fft[i].frequency;

        x += amplitude * cosf(frequency * time + phase);
        y += amplitude * sinf(frequency * time + phase);


        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 50);
        if(prev_x && prev_y)
            SDL_RenderDrawLineF(renderer, prev_x, prev_y, x, y);
        RENDERER_DrawCircle(renderer, x, y, amplitude);
    }
    time +=dt;
    return SHAPE_CreatePoint(x, y);

}

SDL_Texture* RENDERER_GetTextureFromRenderer(SDL_Renderer* renderer) {
    SDL_Texture *target = SDL_GetRenderTarget(renderer);
    int width, height;
    SDL_GetRendererOutputSize(renderer, &width, &height);
    SDL_Surface *surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    SDL_RenderReadPixels(renderer, NULL, surface->format->format, surface->pixels, surface->pitch);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return texture;
}


int main(int argc, char** argv) {
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        fprintf(stderr, "%s\n", SDL_GetError());
    // Load the svg file in memory
    xmlDocPtr svgfile = PARSER_LoadSVG("../fourier.svg");
    // Parse the svg file and create SVG shapes with attributes of the file
    svgShapeStack *svg_shapes = PARSER_GetShapesFromSVG(svgfile);
    // Transform to mathematical represnetation from svg shapes
    ShapeAbstract *abstract_shapes = SHAPE_CreateAbstractFromSVG(svg_shapes);

    // Get the sets of points from the abstract shapes this operation frees the abstract shapes stack
    size_t nb_points = 0;
    ShapePoint *points = SHAPE_GetPointsFromAbstractShapes(abstract_shapes, 0.1f, &nb_points);
    kiss_fft_cpx* complex_array = GetComplexArrayFromPoints(points, nb_points);
    struct FFT_s* fft = GetFFTOfComplexArray(complex_array, nb_points);

#ifdef SDL
    SDL_Window *win = SDL_CreateWindow("Epicycloide",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       1920, 1000, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(win, 0, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    RENDERER_DrawPoints(renderer, points);
    SDL_RenderPresent(renderer);
    SDL_Texture* texture = RENDERER_GetTextureFromRenderer(renderer);
    ShapePoint* points_fourier = NULL;

    while (1)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        //SDL_RenderCopy(renderer, texture, NULL, NULL);
        SHAPE_AddPoints(&points_fourier, Test(renderer, NULL, fft, nb_points));
        RENDERER_DrawPoints(renderer, points_fourier);
        SDL_RenderPresent(renderer);

        SDL_Delay(1);
    }

#else
    gtk_init(&argc, &argv);

    GtkBuilder* builder = gtk_builder_new ();
    GError* error = NULL;
    struct UserData_s user_data = {0};
    user_data.precision = 1;

    gtk_builder_add_from_file (builder, "../gui.glade", &error);
    if(error)
    {
        g_error ("%s", error->message);
    }
    GtkWidget * window = (GtkWidget *) gtk_builder_get_object (builder, "window");
    GtkFileChooserButton* file_chooser_button = (GtkFileChooserButton *) gtk_builder_get_object (builder, "file_chooser");
    GtkScale* precision_scale = (GtkScale *) gtk_builder_get_object (builder, "precision_scale");
    GtkDrawingArea* drawing_area = (GtkDrawingArea *) gtk_builder_get_object (builder, "drawing_area");
    user_data.drawing_area = drawing_area;

    g_timeout_add (1 /* milliseconds */, ForceRenderUpdate, &user_data);
    g_signal_connect (G_OBJECT (file_chooser_button), "selection-changed", G_CALLBACK (GetFileNameFromFileChooser), &user_data);
    g_signal_connect(G_OBJECT(precision_scale), "value_changed", G_CALLBACK( GetPrecisionFromScale), &user_data);
    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(DrawOnScreen), &user_data);

    gtk_widget_show_all (window);
    gtk_main ();
    return 0;
#endif
    /*while (1)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        //SDL_RenderCopy(renderer, texture, NULL, NULL);
        SHAPE_AddPoints(&points_fourier, Test(renderer, complex_fft_array, fft, nb_points));
        RENDERER_DrawPoints(renderer, points_fourier);
        SDL_RenderPresent(renderer);

        SDL_Delay(1);
    }*/
/*
    RENDERER_DrawPoints(renderer, points);
    SDL_RenderPresent(renderer);
    SDL_Texture* texture = RENDERER_GetTextureFromRenderer(renderer);

    ShapePoint* point_to_draw_y;
    ShapePoint* point_to_draw_x;
    ShapePoint* points_to_draw = NULL;*/

    //return 0;
/*
    while (1){
       SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        point_to_draw_x = RENDERER_DrawEpycyles(renderer, dft_x, nb_points, 0, points_array[0].x, points_array[0].y);
        point_to_draw_y = RENDERER_DrawEpycyles(renderer, dft_y, nb_points, M_PI/2, points_array[0].x, points_array[0].y);
        SHAPE_AddPoints(&points_to_draw, SHAPE_CreatePoint(point_to_draw_x->x, point_to_draw_y->y));
        SDL_RenderDrawLineF(renderer, point_to_draw_x->x, point_to_draw_x->y, points_to_draw->x, points_to_draw->y);
        SDL_RenderDrawLineF(renderer, point_to_draw_y->x, point_to_draw_y->y, points_to_draw->x, points_to_draw->y);
        RENDERER_DrawPoints(renderer, points_to_draw);
        SDL_RenderPresent(renderer);
        SDL_Delay(10);

    }
    return 0;*/
}