#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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