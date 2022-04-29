#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
#include "MenuTextures.h"
#include "shapes.h"



typedef struct ColorBackgroud_s {
	int r, g, b;
	int a;
}ColorBackground;

typedef struct Background_s {
	ColorBackground clair;
	ColorBackground sombre;
}Background;


typedef struct SCamera {
	SDL_Rect* screen;

	//Points pass�s
	int x;
	int y;

	//Points pr�sents
	int xp;
	int yp;

	//Coeff zomm dezoom
	float ratio;
	

	//Pr�cision de la courbe
	int precision;

	//d�plcement fait dans la fenetre
	int deplacementx;
	int deplacementy;

	//ratio > 1, pour le deplacement
	float ratioX;
	float ratioY;

	Background* back;
	Background* trait;

}Camera;


void Camera_ViewToWorld(Camera** camera, Camera* camera_statique, MenuTextures** textures, int res);
