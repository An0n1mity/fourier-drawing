#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "MenuTextures.h"
#include "SDL.h"
#include "SDL_image.h"
#include "Menu.h"
#include "Camera.h"



void Interact(MenuTextures** textures, MenuTextures** textures_statique, Camera** camera, SDL_Renderer *renderer) {

    int fonc = 0;
    int nbImage = 0;

    int var_x = (*camera)->xp;
    int rouleau_x = (*textures_statique)->list[nbImage]->r->x;

    if (( var_x >= rouleau_x )&& (var_x <= (rouleau_x + (*textures_statique)->list[nbImage]->r->w)) && (*textures)->hide == 0) {
        Deroulement(textures);
    }
    if ((*camera)->xp <= (*textures_statique)->list[0]->r->w && (*textures)->hide == 1) {
        Deroulement(textures);
    }

    if ((*textures)->hide == 0) {
        nbImage = 2;
        int a = ((*textures_statique)->nbImage) / 2;
        if (((*textures_statique)->nbImage) % 2 == 1) a++;
        for (nbImage = 2; nbImage < a; nbImage++) {
            if ((*camera)->xp >= (*textures_statique)->list[nbImage]->r->x && (*camera)->xp <= (*textures_statique)->list[nbImage]->r->x + (*textures_statique)->list[nbImage]->r->w)
                if ((*camera)->yp >= (*textures_statique)->list[nbImage]->r->y && (*camera)->yp <= (*textures_statique)->list[nbImage]->r->y + (*textures_statique)->list[nbImage]->r->h) {
                    fonc = nbImage;
                }
        }

    }

    //Souris
    if (fonc == 2)printf("%d\n", fonc);
    //Point
    if (fonc == 3)printf("%d\n", fonc);
    //Segment
    if (fonc == 4)printf("%d\n", fonc);
    //Cercle    
    if (fonc == 5)printf("%d\n", fonc);

    if (fonc == 6) {
        (*camera)->precision -= 10;
        if ((*camera)->precision <= 1)(*camera)->precision = 10;
    }
    if (fonc == 7) {
        (*camera)->precision += 10;
    }
    if (fonc == 8) {
        (*textures)->color = ((*textures)->color + 1) %2;
        
    }
}




void Deroulement(MenuTextures** textures) {
    int a = (*textures)->nbImage/2;
    if (a % 2 == 1)a++;
    

    if ((*textures)->color == 0) {
        if ((*textures)->hide == 0) {
            for (int i = 0; i < a-2; i++) {
                for (int j = 0; j < 340; j++) {
                    (*textures)->list[i]->r->x -= 1;
                }
            }
            (*textures)->hide = 1;

            return;
        }

        if ((*textures)->hide == 1) {
            printf("Je suis censé deouler");
            for (int i = 0; i < a-2; i++) {
                for (int j = 0; j < 340; j++) {
                    
                    (*textures)->list[i]->r->x += 1;
                }
            }
            (*textures)->hide = 0;
            
            return;
        }
    }
    if ((*textures)->color == 1) {
        a -= 1;
        if ((*textures)->hide == 0) {
            for (int i = a; i < (*textures)->nbImage; i++) {
                for (int j = 0; j < 340; j++) {
                    if (i == (*textures)->nbImage-1)continue;
                    (*textures)->list[i]->r->x -= 1;
                }
            }
            (*textures)->hide = 1;
            
            return;
        }

        if ((*textures)->hide == 1) {
            for (int i = a; i < (*textures)->nbImage; i++) {
                for (int j = 0; j < 340; j++) {
                    if (i ==(*textures)->nbImage -1 )continue;
                    (*textures)->list[i]->r->x += 1;
                }
            }
            (*textures)->hide = 0;
           
            return;
        }
    }
    
    
}