#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MenuTextures.h"
#include "SDL.h"
#include "Camera.h"


void Camera_ViewToWorld(Camera** camera,Camera* camera_statique, MenuTextures** textures, int res) {

	(*camera)->deplacementx = (*camera)->xp - (*camera)->x;
	(*camera)->deplacementy = (*camera)->yp - (*camera)->y;

	if (res == 0) {
		
		//printf("position = %d %d et la taille st = %d %d\n\n\n", (*camera)->screen->x, (*camera)->screen->y, (*camera)->screen->w, (*camera)->screen->h);
		(*camera)->screen->x = (*camera)->screen->x +  (*camera)->deplacementx;
		(*camera)->screen->y = (*camera)->screen->y +  (*camera)->deplacementy;

		//printf("position = %d %d et la taille st = %d %d\n", (*camera)->screen->x, (*camera)->screen->y, (*camera)->screen->w, (*camera)->screen->h);

		for (int i = 0; i < (*textures)->nbImage; i++) {
			(*textures)->list[i]->r->x -= (*camera)->deplacementx;
			(*textures)->list[i]->r->y -= (*camera)->deplacementy;
		}
	}
	if (res == 1) {
		float sub_x = (camera_statique->screen->w * (*camera)->ratio) - (*camera)->screen->w;
		float sub_y = ((camera_statique->screen->h * (*camera)->ratio) - (*camera)->screen->h);
		
		(*camera)->screen->x = (*camera)->screen->x - sub_x;
		(*camera)->screen->y = (*camera)->screen->y - sub_y;

		for (int i = 0; i < (*textures)->nbImage; i++) {
			(*textures)->list[i]->r->x += sub_x;
			(*textures)->list[i]->r->y += sub_y;
		}
	}
	
	(*camera)->screen->w = camera_statique->screen->w * (*camera)->ratio;
	(*camera)->screen->h = camera_statique->screen->h * (*camera)->ratio;

	//plus grand x 
	int moon = Found(*textures, "moon");
	//Plus petit x et y
	int background = Found(*textures, "background");
	//Plus grand y
	int rouleau = Found(*textures, "rouleau");

	int largeur_x = (*textures)->list[moon]->r->x + (*textures)->list[moon]->r->w;

	if ( largeur_x > (*camera)->screen->w) {
		(*camera)->screen->w = largeur_x;
	}
	if ((*textures)->list[rouleau]->r->y + (*textures)->list[rouleau]->r->h > (*camera)->screen->h) {
		(*camera)->screen->h = (*textures)->list[rouleau]->r->y + (*textures)->list[rouleau]->r->h;
	}
}
