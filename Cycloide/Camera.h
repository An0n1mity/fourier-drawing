#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
#include "MenuTextures.h"

//Précision de la courbe
static int precision = 1;



typedef struct SCamera {
	SDL_Rect *screen;

	//Points passés
	int x;
	int y;
	
	//Points présents
	int xp;
	int yp;

	//ratio < 1, pour le scalaire de l'écran
	float ratiox ;
	float ratioy ;

	//déplcement fait dans la fenetre
	int deplacementx ;
	int deplacementy ;

	//ratio > 1, pour le deplacement
	float ratioX ;
	float ratioY ;

}Camera;


void Camera_ViewToWorld(Camera** camera,int screenw, int screenh, int res);

void Camera_ImageToWorld(Camera* camera, MenuTextures** textures, int h, int w);