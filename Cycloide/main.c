#include <stdbool.h>
#include "shapes.h"
#include "svgparser.h"
#include "BezierCurve.h"
#include "Camera.h"
#include "Menutextures.h"
#include "SDL.h"
#include "SDL_image.h"

int main(int argc, char* argv[])
{
	xmlDocPtr svgfile = PARSER_LoadSVG("../Cycloide/test.xml");
	xmlNodeShape* shapes = PARSER_GetShapesFromSVG(svgfile);


	Camera* camera = NULL;
	camera = (Camera*)calloc(1, sizeof(Camera));
	Camera* camera_statique = NULL;
	camera_statique = (Camera*)calloc(1, sizeof(Camera));
	MenuTextures* textures = NULL;
	MenuTextures* textures_statique = NULL;
	MenuTextures* textures_backup = NULL;

	camera->screen = (SDL_Rect*)calloc(1, sizeof(SDL_Rect));
	camera->screen->h = 720;
	camera->screen->w = 1024;
	camera->screen->x = 0;
	camera->screen->y = 0;
	camera->precision = 100;

	//Ne change jamais
	camera_statique->screen = (SDL_Rect*)calloc(1, sizeof(SDL_Rect));
	camera_statique->screen->h = 720;
	camera_statique->screen->w = 1024;
	camera_statique->screen->x = 0;
	camera_statique->screen->y = 0;

	
	//Initialisation Couleur background 
	camera->back = (Background*)calloc(1, sizeof(Background));
	camera->back->clair.r = 240;
	camera->back->clair.g = 240;
	camera->back->clair.b = 240;
	camera->back->clair.a = 255;

	camera->back->sombre.r = 10;
	camera->back->sombre.g = 10;
	camera->back->sombre.b = 10;
	camera->back->sombre.a = 255;

	//Initialisation Couleur trait
	camera->trait = (Background*)calloc(1, sizeof(Background));
	camera->trait->clair.r = 0;
	camera->trait->clair.g = 0;
	camera->trait->clair.b = 0;
	camera->trait->clair.a = 255;

	camera->trait->sombre.r = 255;
	camera->trait->sombre.g = 255;
	camera->trait->sombre.b = 255;
	camera->trait->sombre.a = 255;
	




	SHAPE_Point test[4], testResult[4];
	

	if (!SDL_WasInit(SDL_INIT_EVERYTHING))
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			printf("[-] ERROR - Failed to initialise SDL (%s)\n", SDL_GetError());
			return EXIT_FAILURE;
		}
	}

	if (!IMG_Init(IMG_INIT_PNG))
	{
		printf("[-] ERROR - Failed to initialise SDL_Image (%s)\n", SDL_GetError());
		return EXIT_FAILURE;
	}
	SDL_Window* window = SDL_CreateWindow("BezierCurve", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 720, SDL_WINDOW_OPENGL);

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

	bool a = false;
	
	//Initialisation Caméra
	
	camera->ratioX = 1;
	camera->ratioY = 1;

	

	//Initialisation Textures
	textures = MenuTextures_new(renderer);

	//Initialisation Textures qui ne change jamais
	textures_statique = MenuTextures_new(renderer);
	

	bool changement = false;

	
	float incr = 1;
	float c = 0.1f;

	//Zoom de l'écran
	camera->ratio = incr;

	
	while (1)
	{
		if(textures->color == 0)
			SDL_SetRenderDrawColor(renderer, camera->back->clair.r, camera->back->clair.g, camera->back->clair.b, camera->back->clair.a);
		if(textures->color == 1)
			SDL_SetRenderDrawColor(renderer, camera->back->sombre.r, camera->back->sombre.g, camera->back->sombre.b, camera->back->sombre.a);
		SDL_RenderClear(renderer);
		SDL_Event event;

		while (SDL_PollEvent(&event))
		{
			int wheel = 0;
			switch (event.type)
			{
			case SDL_MOUSEMOTION:
				replace_texture(&textures, textures_statique,renderer, event.motion.x, event.motion.y);
				break;
			case SDL_MOUSEBUTTONDOWN:
				a = true;
				camera->xp = event.button.x;
				camera->yp = event.button.y;
				
				Interact(&textures, textures_statique, &camera, renderer);
				printf("\n\n%d %d \n\n", camera->xp, camera->yp);

				break;

			case SDL_MOUSEBUTTONUP:
				a = false;

				if (event.motion.x <= 0) event.motion.x = 0;
				if (event.motion.y <= 0) event.motion.y = 0;
				if (event.motion.x >= 1024) event.motion.x = 1024;
				if (event.motion.y >= 720) event.motion.y = 720;

				camera->xp = event.motion.x;
				camera->yp = event.motion.y;
				Camera_ViewToWorld(&camera, camera_statique, &textures, 0);
				break;

			case SDL_MOUSEWHEEL:

				wheel = event.wheel.y;

				if (wheel == -1) {
					incr += c;
					changement = true;
					
				}
				if (wheel == 1) {
					incr -= c;
					changement = true;
				}
				if (incr <= 1)incr = 1;
				
				wheel = 0;
				break;

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					exit(0);
					break;
				case SDLK_UP:
					camera->precision++;
					break;
				case SDLK_DOWN:
					camera->precision -= 1;
					if (camera->precision <= 1)camera->precision = 1;
					break;
				case SDL_QUIT:
					exit(0);
					break;
				default:
					break;
				}
			}
			if (a == true) {
				camera->x = camera->xp;
				camera->y = camera->yp;

				if (event.motion.x <= 0) event.motion.x = 0;
				if (event.motion.y <= 0) event.motion.y = 0;
				if (event.motion.x >= 1024) event.motion.x = 1024;
				if (event.motion.y >= 720) event.motion.y = 720;
				camera->xp = event.motion.x;
				camera->yp = event.motion.y;
				Camera_ViewToWorld(&camera, camera_statique, &textures, 0);
				
			}
			SDL_RenderSetViewport(renderer, camera->screen);
		}
		test[0].x = 500 * incr; test[0].y = 500 * incr;
		test[1].x = 500 * incr; test[1].y = 700 * incr;
		test[2].x = 700 * incr; test[2].y = 500 * incr;
		test[3].x = 500 * incr; test[3].y = 500 * incr;
		double** func = getBezierFunction(test[0], test[1], test[2], test[3]);

		if(textures->color == 0)
			SDL_SetRenderDrawColor(renderer, camera->trait->clair.r, camera->trait->clair.g, camera->trait->clair.b, camera->trait->clair.a);
		if (textures->color == 1)
			SDL_SetRenderDrawColor(renderer, camera->trait->sombre.r, camera->trait->sombre.g, camera->trait->sombre.b, camera->trait->sombre.a);

		SDL_RenderDrawLineF(renderer, test[0].x, test[0].y, test[1].x, test[1].y);
		SDL_RenderDrawLineF(renderer, test[1].x, test[1].y, test[2].x, test[2].y);
		SDL_RenderDrawLineF(renderer, test[2].x, test[2].y, test[3].x, test[3].y);

		SHAPE_Point lastPoint = test[0];
		for (int i = 0; i <= camera->precision; ++i)
		{
			SHAPE_Point current = getBezierPoint(func, (double)i / (double)camera->precision);
			if (changement == true) {
				camera->ratio = incr;
				Camera_ViewToWorld(&camera, camera_statique, &textures, 1);
				changement = false;
			}

			SDL_RenderDrawLineF(renderer, lastPoint.x , lastPoint.y , current.x , current.y );
			lastPoint = current;
		}

		render(renderer, textures);

		SDL_RenderPresent(renderer);
		SDL_Delay(0);
		freeBezierFunction(func);
	}
	MenuTextures_free(textures);
	SDL_Quit();
	
	SDL_DestroyRenderer(renderer);

}