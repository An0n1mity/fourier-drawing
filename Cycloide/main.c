
#include "shapes.h"
#include "svgparser.h"
#include "BezierCurve.h"
#include "complex.h"
#include "SDL2/SDL.h"
#include "Fourier.h"
#include <time.h>

int g_nbCircles = 20;
double g_timeScale = 10.0;
void doInput()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				exit(0);
				break;
			case SDLK_UP:
				g_nbCircles += 2;
				break;
			case SDLK_DOWN:
				g_nbCircles -= (g_nbCircles > 2) ? 2 : 0;
				break;
			case SDL_QUIT:
				exit(0);
				break;
			case SDLK_PLUS:
			case SDLK_KP_PLUS:
				g_timeScale /= 1.5;
				break;
			case SDLK_MINUS:
			case SDLK_KP_MINUS:
				g_timeScale *= 1.5;
				break;
			default:
				break;
			}
		}
	}
}

int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		fprintf(stderr, "%s\n", SDL_GetError());

	srand(time(NULL));
	size_t nbPoints = 0;
	size_t nbFunctions = 0;

	// Load the svg file in memory
	xmlDocPtr svgfile = PARSER_LoadSVG("fourier.svg");

	// Parse the svg file and create SVG shapes with attributes of the file
	svgShapeStack* svg_shapes = PARSER_GetShapesFromSVG(svgfile);

	// Transform to mathematical represnetation from svg shapes
	ShapeAbstract* abstract_shapes = SHAPE_CreateAbstractFromSVG(svg_shapes);
	ShapePoint* pointsList = SHAPE_GetPointsFromAbstractShapes(abstract_shapes, 0.001f, &nbPoints);

	SDL_Window* window = SDL_CreateWindow("Fourier drawing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 720, SDL_WINDOW_OPENGL);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

	double**** bezierList = (double****) calloc(1, sizeof(double***));

	struct Circle_s* circleList = initFourier(bezierList, pointsList, nbPoints, &nbFunctions);
	int currentNbCircles = g_nbCircles;
	double startTime = clock();
	double currentTime = startTime;
	double currentDeltaTime = startTime;

	ShapePoint* currentPosition = NULL, lastPosition = getPositionFromCircles(circleList, *bezierList, nbFunctions, 0);
	SDL_RenderClear(renderer);


	ShapePoint* drawPointList = (ShapePoint*) calloc(1, sizeof(ShapePoint));
	if (!drawPointList)
		return -1;

	drawPointList->x = lastPosition.x;
	drawPointList->y = lastPosition.y;
	double prevTimeScale = g_timeScale;
	ShapePoint* currentPoint = drawPointList;

	while (1)
	{
		currentTime = (double)(clock() - startTime) / ((double)CLOCKS_PER_SEC * g_timeScale);
		if (currentTime >= 2.0 || prevTimeScale != g_timeScale)
		{
			startTime = clock();
			SHAPE_FreePoints(drawPointList);
			drawPointList = NULL;
			currentTime = 0.0;
			prevTimeScale = g_timeScale;
		}

		if (currentPoint = drawPointList)
		{

			lastPosition.x = currentPoint->x;
			lastPosition.y = currentPoint->y;
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		printf("currentTime : %.4lf \t nb circles : %d\n", currentTime, g_nbCircles);
		currentPosition = (ShapePoint*) malloc(sizeof(ShapePoint));

		(*currentPosition) = getPositionFromCircles(circleList, *bezierList, nbFunctions, currentTime);

		SHAPE_AddPoint(&drawPointList, currentPosition);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		while (currentPoint)
		{
			SDL_RenderDrawLineF(renderer, lastPosition.x, lastPosition.y, currentPoint->x, currentPoint->y);
			lastPosition = (*currentPoint);
			currentPoint = currentPoint->np;
		}

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		doInput();

		while (currentNbCircles > g_nbCircles)
		{
			freeLastCircles(&circleList);
			currentNbCircles -= (currentNbCircles > 2) ? 2 : 0;
		}

		while (currentNbCircles < g_nbCircles)
		{
			addLastCircles(&circleList, (currentNbCircles/2) + 1, *bezierList, nbFunctions);
			currentNbCircles += 2;
		}

		SDL_SetRenderDrawColor(renderer, 0, 100, 255, 255);
		drawCircles(renderer, circleList);
		SDL_RenderPresent(renderer);
		SDL_Delay(0);
	}

	SHAPE_FreePoints(drawPointList);
	for (size_t i = 0; i < nbFunctions; ++i)
		freeBezierFunction((*bezierList)[i]);
	free(*bezierList);
	free(bezierList);
	freeCircles(circleList);
	return 0;
}