#include <stdbool.h>
#include "shapes.h"
#include "svgparser.h"
#include "BezierCurve.h"
#include "Camera.h"
#include "SDL.h"
#include "SDL_ttf.h"

int precision = 1;


int main(int argc, char* argv[])
{
	xmlDocPtr svgfile = PARSER_LoadSVG("../Cycloide/test.xml");
	xmlNodeShape* shapes = PARSER_GetShapesFromSVG(svgfile);

	SHAPE_Point test[4], testResult[4];
	test[0].x = 50; test[0].y = 50;
	test[1].x = 300; test[1].y = 10;
	test[2].x = 0; test[2].y = 200;
	test[3].x = 2000; test[3].y = 2000;
	double** func = getBezierFunction(test[0], test[1], test[2], test[3]);

	SDL_Rect screen;
	screen.h = 720;
	screen.w = 1024;
	screen.x = 0;
	screen.y = 0;

	int screenw = screen.w;
	int screenh = screen.h;
	testResult[0] = getBezierPoint(func, 0.25);
	testResult[1] = getBezierPoint(func, 0.5);
	testResult[2] = getBezierPoint(func, 0.75);
	testResult[3] = getBezierPoint(func, 1);

	if (!SDL_WasInit(SDL_INIT_EVERYTHING))
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			printf("[-] ERROR - Failed to initialise SDL (%s)\n", SDL_GetError());
			return EXIT_FAILURE;
		}
	}
	SDL_Window* window = SDL_CreateWindow("BezierCurve", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 720, SDL_WINDOW_OPENGL);
	
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

	float x = 0;
	float y = 0;
	float xp = 0;
	float yp = 0;
	float zoom = 0;
	float ratiox = 1;
	float ratioy = 1;
	
	float deplacementx = 0;
	float deplacementy = 0;
	float ratioX = 1;
	float ratioY = 1;


	SDL_RenderSetScale(renderer,ratiox, ratioy);
	while (1)
	{
		SDL_SetRenderDrawColor(renderer, 0,0,0, 255);
		SDL_RenderClear(renderer);
		SDL_Event event;
		
		while (SDL_PollEvent(&event))
		{
			int wheel = 0;
			
			switch (event.type)
			{
			case SDL_MOUSEMOTION:
				//if (bas == true) {
				//	xp = event.motion.x;
				//	yp = event.motion.y;
				//	printf("%f %f\n", xp, yp);
				//}
				//break;

				break;
			case SDL_MOUSEBUTTONDOWN:
				x = event.button.x;
				y = event.button.y;
				if (event.type == SDL_MOUSEMOTION) {
					xp = event.motion.x;
					yp = event.motion.y;
				}
				
				break;

			

			case SDL_MOUSEBUTTONUP:
				xp = event.motion.x;
				yp = event.motion.y;
				deplacementx = xp - x;
				deplacementy = yp - y;
				ratioX = (float)screenw / 1024;
				ratioY = (float)screenh / 720;
				screen.x = screen.x + ratioX * deplacementx;
				screen.y = screen.y + ratioY * deplacementy;
				ratiox = (1024 / (float)screenw) + zoom;
				ratioy = (720  /  (float)screenh)+ zoom;
				SDL_RenderSetViewport(renderer, &screen);
				SDL_RenderSetScale(renderer, ratiox, ratioy);
				break;
			

			case SDL_MOUSEWHEEL:
			
				wheel = event.wheel.y;
				if (wheel == -1) {	
					screenw += 50;
					screenh += 50;
				
				}
				if (wheel == 1) {	
					screenw -= 50;
					screenh -= 50;
				}
				
				deplacementx = xp - x;
				deplacementy = yp - y;
				ratioX = (float)screenw / 1024;
				ratioY = (float)screenh / 720;
				ratiox = (1024 / (float)screenw) ;
				ratioy = (720 / (float)screenh);
				
				SDL_RenderSetScale(renderer, ratiox, ratioy);
				SDL_RenderSetViewport(renderer, &screen);
				wheel = 0;
				break;
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
		SDL_SetRenderDrawColor(renderer, 255,255,255, 255);
		SDL_RenderDrawLineF(renderer, test[0].x, test[0].y, test[1].x, test[1].y);
		SDL_RenderDrawLineF(renderer, test[1].x, test[1].y, test[2].x, test[2].y);
		SDL_RenderDrawLineF(renderer, test[2].x, test[2].y, test[3].x, test[3].y);

		SDL_SetRenderDrawColor(renderer, 255,255,255, 255);
		SHAPE_Point lastPoint = test[0];
		for (int i = 0; i <= precision; ++i)
		{
			SHAPE_Point current = getBezierPoint(func, (double)i/(double)precision);
			if (current.x > screen.w) {
				int sub = current.x - screen.w;
				screen.w += sub;
			}
			if (current.y > screen.h) {
				int sub = current.y - screen.h;
				screen.h += sub;
			}
			SDL_RenderDrawLineF(renderer, lastPoint.x, lastPoint.y, current.x, current.y);
			lastPoint = current;
		}

		SDL_RenderPresent(renderer);
		SDL_Delay(0);
	}
	SDL_Quit();
	freeBezierFunction(func);
	SDL_DestroyRenderer(renderer);
}