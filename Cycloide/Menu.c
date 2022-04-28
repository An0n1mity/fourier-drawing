#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "MenuTextures.h"
#include "SDL.h"
#include "SDL_image.h"
#include "Menu.h"
#include "Camera.h"



void Interact(MenuTextures** textures, MenuTextures** textures_statique, Camera** camera) {

    int fonc = 0;
    int nbImage = 0;

    int var_x = (*camera)->xp + (*camera)->screen->x;
    int rouleau_x = (*textures_statique)->list[nbImage]->r->x;
    if (( var_x >= rouleau_x )&& (var_x <= (rouleau_x + (*textures_statique)->list[nbImage]->r->w)) && (*textures)->hide == 0) {
        Deroulement(textures);
        printf("Derouleent");
    }

    if ((*camera)->xp <= (*textures)->list[0]->r->w && (*textures)->hide == 1) {
        Deroulement(textures);
        printf("Derouleent");
    }
   

    if ((*textures)->hide == 0) {
        nbImage = 2;
        for (nbImage = 2; nbImage < (*textures_statique)->nbImage; nbImage++) {
            if ((*camera)->xp >= (*textures_statique)->list[nbImage]->r->x && (*camera)->xp <= (*textures_statique)->list[nbImage]->r->x + (*textures_statique)->list[nbImage]->r->w)
                if ((*camera)->yp >= (*textures_statique)->list[nbImage]->r->y && (*camera)->yp <= (*textures_statique)->list[nbImage]->r->y + (*textures_statique)->list[nbImage]->r->h) {
                    fonc = nbImage;
                }
        }

    }
    if (fonc == 2)printf("%d\n", fonc);
    //Point
    if (fonc == 3)printf("%d\n", fonc);
    //Segment
    if (fonc == 4)printf("%d\n", fonc);
    //Cercle    
    if (fonc == 5) {
        (*camera)->precision -= 10;
        if ((*camera)->precision <= 1)(*camera)->precision = 10;
    }
    if (fonc == 6) {
        (*camera)->precision += 10;
    }
}


void Deroulement(MenuTextures** textures) {

    if ((*textures)->hide == 0) {
        for (int i = 0; i < (*textures)->nbImage; i++) {
            for (int j = 0; j < 340; j++) {
                (*textures)->list[i]->r->x -= 1;
            }
        }
        (*textures)->hide = 1;
        return;
    }

    if ((*textures)->hide == 1) {
        for (int i = 0; i < (*textures)->nbImage; i++) {
            for (int j = 0; j < 340; j++) {
                (*textures)->list[i]->r->x += 1;
            }
        }
        (*textures)->hide = 0;
        return;
    }
}