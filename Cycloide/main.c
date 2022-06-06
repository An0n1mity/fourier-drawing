#include "shapes.h"
#include "svgparser.h"
#include "BezierCurve.h"
#include "complex.h"
#include "SDL2/SDL.h"
#include "Fourier.h"
#include <time.h>
#include "main.h"

int precision = 1;
double timeScale = 10.0;

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
				precision++;
				break;
			case SDLK_DOWN:
				precision -= precision > 1 ? 1 : 0;
				break;
			case SDL_QUIT:
				exit(0);
				break;
			case SDLK_PLUS:
				timeScale *= 2.0;
				break;
			case SDLK_MINUS:
				timeScale /= 2.0;
				break;
			default:
				break;
			}
		}
	}
}

int main(int argc, char* argv[])
{
#if 0
	xmlDocPtr svgfile = PARSER_LoadSVG("../Cycloide/test.xml");
	xmlNodeShape* shapes = PARSER_GetShapesFromSVG(svgfile);
	SHAPE_Point test[4], testResult[4];
	test[0].x = 50; test[0].y = 50;
	test[1].x = 300; test[1].y = 10;
	test[2].x = 400; test[2].y = 600;
	test[3].x = 1000; test[3].y = 500;
	double** func = getBezierFunction(test[0], test[1], test[2], test[3]);

	testResult[0] = getBezierPoint(func, 0.25);
	testResult[1] = getBezierPoint(func, 0.5);
	testResult[2] = getBezierPoint(func, 0.75);
	testResult[3] = getBezierPoint(func, 1);

	SDL_Window* window = SDL_CreateWindow("BezierCurve", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 720, SDL_WINDOW_OPENGL);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	while (1)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		doInput();
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawLineF(renderer, test[0].x, test[0].y, test[1].x, test[1].y);
		SDL_RenderDrawLineF(renderer, test[1].x, test[1].y, test[2].x, test[2].y);
		SDL_RenderDrawLineF(renderer, test[2].x, test[2].y, test[3].x, test[3].y);

		SDL_SetRenderDrawColor(renderer, 0, 100, 255, 255);
		SHAPE_Point lastPoint = test[0];
		for (int i = 0; i <= precision; ++i)
		{
			SHAPE_Point current = getBezierPoint(func, (double)i/(double)precision);
			SDL_RenderDrawLineF(renderer, lastPoint.x, lastPoint.y, current.x, current.y);
			lastPoint = current;
		}
		SDL_RenderPresent(renderer);
		SDL_Delay(0);
	}
#elif 0
	xmlDocPtr svgfile = PARSER_LoadSVG("../Cycloide/test.xml");
	xmlNodeShape* shapes = PARSER_GetShapesFromSVG(svgfile);
	
	SHAPE_Point pointList[4][4] = {0};
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

	double*** bezierList = (double***)malloc(4 * sizeof(double**));
	if (!bezierList)
		return -1;
	bezierList[0] = getBezierFunction(pointList[0][0], pointList[0][1], pointList[0][2], pointList[0][3]);
	bezierList[1] = getBezierFunction(pointList[1][0], pointList[1][1], pointList[1][2], pointList[1][3]);
	bezierList[2] = getBezierFunction(pointList[2][0], pointList[2][1], pointList[2][2], pointList[2][3]);
	bezierList[3] = getBezierFunction(pointList[3][0], pointList[3][1], pointList[3][2], pointList[3][3]);

	SDL_Window* window = SDL_CreateWindow("BezierCurve", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 720, SDL_WINDOW_OPENGL);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	while (1)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		doInput();

		SDL_SetRenderDrawColor(renderer, 0, 100, 255, 255);
		SHAPE_Point lastPoint = pointList[0][0];
		for (int i = 0; i <= precision; ++i)
		{
			SHAPE_Point current = getBezierPoint(bezierList[0], (double)i / (double)precision);
			SDL_RenderDrawLineF(renderer, lastPoint.x, lastPoint.y, current.x, current.y);
			lastPoint = current;
		}

		SDL_SetRenderDrawColor(renderer, 255, 0, 100, 255);
		lastPoint = pointList[1][0];
		for (int i = 0; i <= precision; ++i)
		{
			SHAPE_Point current = getBezierPoint(bezierList[1], (double)i / (double)precision);
			SDL_RenderDrawLineF(renderer, lastPoint.x, lastPoint.y, current.x, current.y);
			lastPoint = current;
		}

		SDL_SetRenderDrawColor(renderer, 100, 255, 0, 255);
		lastPoint = pointList[2][0];
		for (int i = 0; i <= precision; ++i)
		{
			SHAPE_Point current = getBezierPoint(bezierList[2], (double)i / (double)precision);
			SDL_RenderDrawLineF(renderer, lastPoint.x, lastPoint.y, current.x, current.y);
			lastPoint = current;
		}

		SDL_SetRenderDrawColor(renderer, 200, 100, 255, 255);
		lastPoint = pointList[3][0];
		for (int i = 0; i <= precision; ++i)
		{
			SHAPE_Point current = getBezierPoint(bezierList[3], (double)i / (double)precision);
			SDL_RenderDrawLineF(renderer, lastPoint.x, lastPoint.y, current.x, current.y);
			lastPoint = current;
		}
		SDL_RenderPresent(renderer);
		SDL_Delay(0);
	}


	for (int i = 0; i < 4; ++i)
		freeBezierFunction(bezierList[i]);
#else
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

	int nbCircles = 40;

	struct Circle_s* circleList = createCircle(0, getCircleCoeff(0, bezierList, 4));
	struct Circle_s* currentCircle;

	for (int i = 1; i <= nbCircles / 2; ++i)
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
		currentTime =  (double)(clock() - startTime) / ((double) CLOCKS_PER_SEC * timeScale);
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
		SDL_SetRenderDrawColor(renderer, 0, 100, 255, 255);
		drawCircles(renderer, circleList);
		SDL_RenderPresent(renderer);
		SDL_Delay(0);
	}

	destroySHAPE_PointList(drawPointList);
	for (int i = 0; i < 4; ++i)
		freeBezierFunction(bezierList[i]);
#endif
	return 0;
}