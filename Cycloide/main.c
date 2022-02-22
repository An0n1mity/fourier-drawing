#include "shapes.h"
#include "svgparser.h"
#include "BezierCurve.h"
#include "complex.h"
#include "SDL2/SDL.h"
int precision = 1;

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

	freeBezierFunction(func);
#else
	struct Complex_s* a = createComplex(3, -6);
	struct Complex_s* b = createComplex(2, 1);
	struct ComplexList_s* list = createComplexList(a);
	addObjectComplexList(list, b);
	struct Complex_s* result = multiplyComplex(a, b);
	printComplex(a, 2);
	printComplex(b,2);
	printComplex(result,2);
	result = addComplexList(list);
	//add Complex List à corriger
	printComplex(result, 2);
#endif
}