#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL.h"
#include "Camera.h"

void Camera_ViewToWorld(SDL_Renderer *renderer, SDL_Rect screen, float x, float y, float xp, float yp) {
	
    float deplacementx = xp - x;
    float deplacementy = yp - y;
    float ratioX = (float)screen.w / 1024;
    float ratioY = (float)screen.h / 720;
    screen.x = screen.x + ratioX * deplacementx;
    screen.y = screen.y + ratioY * deplacementy;
    float ratiox = 1024 / (float)screen.x;
    float ratioy = 720 / (float)screen.y;

    SDL_RenderSetViewport(renderer, &screen);
    SDL_RenderSetScale(renderer, ratiox, ratioy);
}