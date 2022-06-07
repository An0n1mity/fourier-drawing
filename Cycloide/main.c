#include "shapes.h"
#include "svgparser.h"
#include "BezierCurve.h"
#include "complex.h"
#include "SDL2/SDL.h"
#include "Fourier.h"
#include <time.h>
#include "main.h"

int g_nbCircles = 10;
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
				g_timeScale *= 2.0;
				break;
			case SDLK_MINUS:
			case SDLK_KP_MINUS:
				g_timeScale /= 2.0;
				break;
			default:
				break;
			}
		}
	}
}

int main(int argc, char* argv[])
{
	srand(time(NULL));

	xmlDocPtr svgfile = PARSER_LoadSVG("../Cycloide/test.xml");
	xmlNodeShape* shapes = PARSER_GetShapesFromSVG(svgfile);
	SDL_Window* window = SDL_CreateWindow("Fourier drawing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 720, SDL_WINDOW_OPENGL);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

	double*** bezierList = (double***) malloc(4 * sizeof(double**));

	struct Circle_s* circleList = initFourier(bezierList);

	int currentNbCircles = g_nbCircles;
	double startTime = clock();
	double currentTime = startTime;
	double currentDeltaTime = startTime;

	SHAPE_Point currentPosition = { 0 }, lastPosition = getPositionFromCircles(circleList, bezierList, 4, 0);
	SDL_RenderClear(renderer);


	SHAPE_Point* drawPointList = (SHAPE_Point*) calloc(1, sizeof(SHAPE_Point));
	if (!drawPointList)
		return -1;

	drawPointList->x = lastPosition.x;
	drawPointList->y = lastPosition.y;
	double prevTimeScale = g_timeScale;
	SHAPE_Point* currentPoint = drawPointList;
	while (1)
	{
		currentPoint = drawPointList;
		lastPosition.x = currentPoint->x;
		lastPosition.y = currentPoint->y;
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		currentTime =  (double)(clock() - startTime) / ((double) CLOCKS_PER_SEC * g_timeScale);

		if (currentTime >= 1.0 || prevTimeScale != g_timeScale)
		{
			startTime = clock();
			destroySHAPE_PointList(drawPointList->np);
			drawPointList->np = NULL;
			currentTime = 0.0;
			prevTimeScale = g_timeScale;
		}

		printf("currentTime : %.4lf\n", currentTime);
		currentPosition = getPositionFromCircles(circleList, bezierList, 4, currentTime);
		SHAPE_AddPoint(&drawPointList, currentPosition.x, currentPosition.y);
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
			addLastCircles(&circleList, (currentNbCircles/2) + 1, bezierList, 4);
			currentNbCircles += 2;
		}

		SDL_SetRenderDrawColor(renderer, 0, 100, 255, 255);
		drawCircles(renderer, circleList);
		SDL_RenderPresent(renderer);
		SDL_Delay(0);
	}

	destroySHAPE_PointList(drawPointList);
	for (int i = 0; i < 4; ++i)
		freeBezierFunction(bezierList[i]);

	freeCircles(circleList);
	return 0;
}