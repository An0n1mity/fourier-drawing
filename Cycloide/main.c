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
				g_timeScale *= 2.0;
				break;
			case SDLK_MINUS:
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

	SHAPE_Point pointList[4][4] = { 0 };

	pointList[0][0].x = 100; pointList[0][0].y = 300;
	pointList[0][1].x = 50; pointList[0][1].y = 250;
	pointList[0][2].x = 50; pointList[0][2].y = 200;
	pointList[0][3].x = 100; pointList[0][3].y = 150;

	pointList[1][0].x = 100; pointList[1][0].y = 150;
	pointList[1][1].x = 200; pointList[1][1].y = 180;
	pointList[1][2].x = 350; pointList[1][2].y = 280;
	pointList[1][3].x = 400; pointList[1][3].y = 300;

	pointList[2][0].x = 400; pointList[2][0].y = 300;
	pointList[2][1].x = 450; pointList[2][1].y = 250;
	pointList[2][2].x = 450; pointList[2][2].y = 200;
	pointList[2][3].x = 400; pointList[2][3].y = 150;

	pointList[3][0].x = 400; pointList[3][0].y = 150;
	pointList[3][1].x = 350; pointList[3][1].y = 180;
	pointList[3][2].x = 200; pointList[3][2].y = 280;
	pointList[3][3].x = 100; pointList[3][3].y = 300;

	double*** bezierList = (double***) malloc(4 * sizeof(double**));
	if (!bezierList)
		return -1;

	bezierList[0] = getBezierFunction(pointList[0][0], pointList[0][1], pointList[0][2], pointList[0][3]);
	bezierList[1] = getBezierFunction(pointList[1][0], pointList[1][1], pointList[1][2], pointList[1][3]);
	bezierList[2] = getBezierFunction(pointList[2][0], pointList[2][1], pointList[2][2], pointList[2][3]);
	bezierList[3] = getBezierFunction(pointList[3][0], pointList[3][1], pointList[3][2], pointList[3][3]);

	SDL_Window* window = SDL_CreateWindow("BezierCurve", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 720, SDL_WINDOW_OPENGL);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

	struct Circle_s* circleList = createCircle(0, getCircleCoeff(0, bezierList, 4));
	struct Circle_s* currentCircle;
	int currentNbCircles = g_nbCircles;
	for (int i = 1; i <= g_nbCircles / 2; ++i)
	{
		currentCircle = createCircle(i, getCircleCoeff(i, bezierList, 4));
		addCircleList(&circleList, currentCircle);
		currentCircle = createCircle(-1 * i, getCircleCoeff(-1 * i, bezierList, 4));
		addCircleList(&circleList, currentCircle);
	}

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

	while (1)
	{
		SHAPE_Point* currentPoint = drawPointList;
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		currentTime =  (double)(clock() - startTime) / ((double) CLOCKS_PER_SEC * g_timeScale);
		if (currentTime >= 1.0)
		{
			startTime = clock();
			destroySHAPE_PointList(drawPointList->np);
			drawPointList->np = NULL;
			currentTime = 0.0;
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

	return 0;
}