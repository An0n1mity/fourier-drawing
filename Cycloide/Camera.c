#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MenuTextures.h"
#include "SDL.h"
#include "Camera.h"


void Camera_ViewToWorld(Camera** camera, int screenw, int screenh, int res) {

	(*camera)->deplacementx = (*camera)->xp - (*camera)->x;
	(*camera)->deplacementy = (*camera)->yp - (*camera)->y;
	(*camera)->ratioX = (float)screenw / 1024;
	(*camera)->ratioY = (float)screenh / 720;
	if (res == 0) {
		(*camera)->screen->x = (*camera)->screen->x + (*camera)->ratioX * (*camera)->deplacementx;
		(*camera)->screen->y = (*camera)->screen->y + (*camera)->ratioY * (*camera)->deplacementy;
	}
	(*camera)->ratiox = (1024 / (float)screenw);
	(*camera)->ratioy = (720 / (float)screenh);



}


void Camera_ImageToWorld_deplacement(Camera* camera, MenuTextures** textures, MenuTextures** textures_statique) {
	for (int i = 0; i < (*textures)->nbImage; i++) {
		(*textures)->list[i]->r->x -= camera->ratioX * camera->deplacementx;
		(*textures)->list[i]->r->y -= camera->ratioY * camera->deplacementy;
	}

}

void Camera_ImageToWorld_swap(Camera* camera, MenuTextures** textures, MenuTextures** textures_statique) {

	//printf("%0.2f\n %0.2f\n %0.2f\n %0.2f\n\n\n", camera->ratioX, camera->ratioY, camera->deplacementx, camera->deplacementy);
	//printf("ratiox = %0.2f\nratioy = %0.2f\nh = %d\nw = %d\nx = %d\ny = %d\n nom = %s\n\n\n ", camera->ratiox, camera->ratioy, (*textures)->list[0]->r->h, (*textures)->list[0]->r->w, (*textures)->list[0]->r->x, (*textures)->list[0]->r->y, (*textures)->list[0]->name);

	for (int i = 0; i < (*textures)->nbImage; i++) {

		//printf("ratiox = %0.2f\nratioy = %0.2f\nh = %d\nw = %d\nx = %d\ny = %d\n nom = %s\n\n\n ", camera->ratiox, camera->ratioy, (*textures)->list[i]->r->h, (*textures)->list[i]->r->w, (*textures)->list[i]->r->x, (*textures)->list[i]->r->y, (*textures)->list[i]->name);

		//(*textures)->list[i]->r->h = (*textures_statique)->list[i]->r->h / camera->ratioy;
		//(*textures)->list[i]->r->w = (*textures_statique)->list[i]->r->w / camera->ratiox;
	}
}


void Positionnement_Camera(Camera* camera, SHAPE_Point current) {

	if (camera->xp > camera->screen->x - 1024) {
		int sub = camera->xp - camera->screen->x - 1024;
		camera->screen->w += sub;
	}
	if (camera->yp > camera->screen->y - 720) {
		int sub = camera->yp - camera->screen->y;
		camera->screen->h += sub;
	}

	if (current.x > camera->screen->x) {
		float sub = current.x - camera->screen->x;
		camera->screen->w += sub;
	}
	if (current.y < camera->screen->y - 720) {
		float sub = camera->screen->h - current.y;
		camera->screen->h += sub;
	}

}