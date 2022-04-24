#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
#include "MenuTextures.h"
#include "shapes.h"

//Pr�cision de la courbe
static int precision = 1;



typedef struct SCamera {
	SDL_Rect* screen;

	//Points pass�s
	int x;
	int y;

	//Points pr�sents
	int xp;
	int yp;

	//ratio < 1, pour le scalaire de l'�cran
	float ratiox;
	float ratioy;

	//d�plcement fait dans la fenetre
	int deplacementx;
	int deplacementy;

	//ratio > 1, pour le deplacement
	float ratioX;
	float ratioY;

}Camera;


void Camera_ViewToWorld(Camera** camera, int screenw, int screenh, int res);

void Camera_ImageToWorld_deplacement(Camera* camera, MenuTextures** textures, MenuTextures** textures_statique);
void Camera_ImageToWorld_swap(Camera* camera, MenuTextures** textures, MenuTextures** textures_statique);

void Positionnement_Camera(Camera* camera, SHAPE_Point current);