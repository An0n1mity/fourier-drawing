#include "rendering.h"

void RENDERER_DrawPath(SDL_Renderer* renderer, ShapePath* path)
{
	// Read the commands of path
	float actual_x = 0.f, actual_y = 0.f;
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	while (path->b)
	{
		if (path->b->id == 'M')
		{
			actual_x = path->b->p->x;
			actual_y = path->b->p->y;
		}

		else if (path->b->id == 'C')
		{
			// Draw line to the current to next
			ShapePoint* point = path->b->p;
			while (point)
			{
				SDL_RenderDrawLine(renderer, actual_x*10, actual_y*10, point->x*10, point->y*10);

				actual_x = point->x;
				actual_y = point->y;
				point = point->np;
			}
		}

		path->b = path->b->nb;
	}
}

void RENDERER_DrawPoints(SDL_Renderer* renderer, ShapePoint* points)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	while (points->np)
	{
		SDL_RenderDrawPoint(renderer, points->x, points->y);
		points = points->np;
	}
}
