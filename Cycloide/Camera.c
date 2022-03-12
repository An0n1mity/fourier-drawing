#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MenuTextures.h"
#include "SDL.h"
#include "Camera.h"


void Camera_ViewToWorld(Camera **camera,int screenw, int screenh, int res) {
	
	(* camera)->deplacementx = (*camera)->xp - (*camera)->x;
	(*camera)->deplacementy = (*camera)->yp - (*camera)->y;
	(*camera)->ratioX = (float)screenw / 1024;
	(*camera)->ratioY = (float)screenh / 720;
	if (res == 0) {
		(*camera)->screen->x = (*camera)->screen->x +(*camera)->ratioX * (*camera)->deplacementx;
		(*camera)->screen->y = (*camera)->screen->y+(*camera)->ratioY * (*camera)->deplacementy;
	}
	(*camera)->ratiox = (1024 / (float)screenw);
	(*camera)->ratioy = (720 / (float)screenh);
}


void Camera_ImageToWorld(Camera* camera, MenuTextures **textures, int h, int w) {
	/*(*textures)->point->r->x -= camera->ratioX * camera->deplacementx;
	(*textures)->point->r->y -= camera->ratioY * camera->deplacementy;
	//(*textures->point->r->h *= h / camera->ratioy;
	//textures->point->r->w *= w / camera->ratiox;

	(*textures)->cercle->r->x -= camera->ratioX * camera->deplacementx;
	(*textures)->cercle->r->y -= camera->ratioY * camera->deplacementy;
	//textures->cercle->r->h *= h / camera->ratioy;
	//textures->cercle->r->w *= w / camera->ratiox;

	(*textures)->segment->r->x -= camera->ratioX * camera->deplacementx;
	(*textures)->segment->r->y -= camera->ratioY * camera->deplacementy;
	//textures->segment->r->h *= h / camera->ratioy;
	//textures->segment->r->w *= w / camera->ratiox;

	(*textures)->background->r->x -= camera->ratioX * camera->deplacementx;
	(*textures)->background->r->y -= camera->ratioY * camera->deplacementy;
	//textures->background->r->h *= h / camera->ratioy;
	//textures->background->r->w *= w / camera->ratiox;


	printf("%d %d    ", (*textures)->rouleau->r->h, (*textures)->rouleau->r->w);
	(*textures)->rouleau->r->x -= camera->ratioX * camera->deplacementx;
	(*textures)->rouleau->r->y -= camera->ratioY * camera->deplacementy;
	(*textures)->rouleau->r->h = h / camera->ratioy;
	(*textures)->rouleau->r->w = w / camera->ratiox;

	printf("%d %d\n", (*textures)->rouleau->r->h, (*textures)->rouleau->r->w);
	*/
}