
#define SDL_MAIN_HANDLED

#include "shapes.h"
#include "svgparser.h"
#include <SDL2/SDL.h>
#include <inttypes.h>

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
        SDL_RenderDrawPoint(renderer, cursor->x, cursor->y);
        cursor = cursor->np;
    }
}

void RENDERER_DrawEpycyles(SDL_Renderer* renderer, DFT* dft, size_t size, float dx, float dy)
{
    float x = 0, y = 0, prev_x, prev_y;
    static float time = 0.f;
    const float dt = M_PI*2/size;
    for (int i = 0; i < size; i+=10) {
        prev_x = x, prev_y = y;
        //phase = atan2(complexe[i].i, complexe[i].r);
        x += dft[i].amplitude * cosf(dft[i].frequency * time +  dft[i].phase + M_PI/2);
        y += dft[i].amplitude * sinf(dft[i].frequency * time + dft[i].phase + M_PI/2);

        SDL_RenderDrawLine(renderer, prev_x+dx, prev_y+dy,x+dx, y+dy);
        RENDERER_DrawCircle(renderer, prev_x+dx, prev_y+dy, dft[i].amplitude*2);
    }
    time += dt;
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
    SDL_Init(SDL_INIT_EVERYTHING);
    // Load the svg file in memory
    xmlDocPtr svgfile = PARSER_LoadSVG("../Pi.svg");
    // Parse the svg file and create SVG shapes with attributes of the file
    svgShapeStack *svg_shapes = PARSER_GetShapesFromSVG(svgfile);
    // Transform to mathematical represnetation from svg shapes
    ShapeAbstract *abstract_shapes = SHAPE_CreateAbstractFromSVG(svg_shapes);
    ShapePoint *points = SHAPE_GetPointsFromAbstractShapes(abstract_shapes, 0.1f);
    size_t nb_points = SHAPE_GetNumberOfPoints(points);
    ShapePoint *points_array = SHAPE_ListOfPointsToArray(points, nb_points);
    DFT* dft_y = SHAPE_GetDFTOfRealPointsY(points_array, nb_points);
    DFT* dft_x = SHAPE_GetDFTOfRealPointsX(points_array, nb_points);

    SDL_Window *win = SDL_CreateWindow("GAME",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       1000, 1000, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(win, 0, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    RENDERER_DrawPoints(renderer, points);
    SDL_RenderPresent(renderer);
    SDL_Texture* texture = RENDERER_GetTextureFromRenderer(renderer);

    while (1){
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        RENDERER_DrawEpycyles(renderer, dft_x, nb_points, 400, 400);
        RENDERER_DrawEpycyles(renderer, dft_y, nb_points, 500, 500);
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }
    return 0;
}