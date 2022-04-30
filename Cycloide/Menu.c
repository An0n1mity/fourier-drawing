#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "MenuTextures.h"
#include "SDL.h"
#include "SDL_image.h"
#include "Menu.h"
#include "Camera.h"



void Interact(MenuTextures** textures, MenuTextures* textures_statique, Camera** camera, SDL_Renderer *renderer) {

    int fonc = 0;
    int rouleau = Found(*textures, "rouleau");

    int var_x = (*camera)->xp;
    int rouleau_x = textures_statique->list[rouleau]->r->x;

    if (( var_x >= rouleau_x )&& (var_x <= (rouleau_x + textures_statique->list[rouleau]->r->w)) && (*textures)->hide == 0) {
        Deroulement(textures, renderer);
    }
    if ((*camera)->xp <= textures_statique->list[0]->r->w && (*textures)->hide == 1) {
        Deroulement(textures, renderer);
    }

    if ((*textures)->hide == 0) {
        rouleau = 2;
        int a = (textures_statique->nbImage) / 2;
        if (a % 2 == 1) a++;
        for (rouleau = 2; rouleau < a; rouleau++) {
            if ((*camera)->xp >= textures_statique->list[rouleau]->r->x && (*camera)->xp <= textures_statique->list[rouleau]->r->x + textures_statique->list[rouleau]->r->w){
                if ((*camera)->yp >= textures_statique->list[rouleau]->r->y && (*camera)->yp <= textures_statique->list[rouleau]->r->y + textures_statique->list[rouleau]->r->h) {
                    fonc = rouleau;
                }
            }
        }

    }
    
    //Souris
    if (fonc == Found(*textures, "souris"))printf("%d\n", fonc);
    //Point
    if (fonc == Found(*textures, "point"))printf("%d\n", fonc);
    //Segment
    if (fonc == Found(*textures, "segment"))printf("%d\n", fonc);
    //Cercle    
    if (fonc == Found(*textures, "cercle"))printf("%d\n", fonc);

    if (fonc == Found(*textures, "minus")) {
        (*camera)->precision -= 10;
        if ((*camera)->precision <= 1)(*camera)->precision = 10;
    }
    if (fonc == Found(*textures, "plus")) {
        (*camera)->precision += 10;
    }
    if (fonc == Found(*textures, "moon")) {
        (*textures)->color = ((*textures)->color + 1) %2;
        
    }
}




void Deroulement(MenuTextures** textures, SDL_Renderer* renderer) {
    

    if ((*textures)->color == 0) {
        int a = Found(*textures, "rouleau_s");

        if ((*textures)->hide == 0) {
            for (int i = 0; i < a; i++) {
                if (i == Found(*textures, "moon"))continue;
                for (int j = 0; j < 340; j++) {
                    (*textures)->list[i]->r->x -= 1;
                }
            }
            (*textures)->hide = 1;


            replace_surface(&((*textures)->list[Found( *textures, "rouleau")]), renderer, "../Assets/Images/Labeur_pas_paye/Barr_L1ght.png");

            return;
        }

        if ((*textures)->hide == 1) {
            for (int i = 0; i < a-2; i++) {
                for (int j = 0; j < 340; j++) {
                    if (i == Found(*textures, "moon"))continue;
                    (*textures)->list[i]->r->x += 1;
                }
            }
            (*textures)->hide = 0;
            replace_surface(&((*textures)->list[Found(*textures, "rouleau")]), renderer, "../Assets/Images/Labeur_pas_paye/Barl_L1ght.png");
            return;
        }
    }
    if ((*textures)->color == 1) {
        int a = Found(*textures, "rouleau_s");

        if ((*textures)->hide == 0) {
            for (int i = a; i < (*textures)->nbImage; i++) {
                if (i == Found(*textures, "sun"))continue;
                for (int j = 0; j < 340; j++) {
                    
                    (*textures)->list[i]->r->x -= 1;
                }
            }
            (*textures)->hide = 1;
            replace_surface(&((*textures)->list[Found(*textures, "rouleau_s")]), renderer, "../Assets/Images/Labeur_pas_paye/Barr_D4rk.png");

            return;
        }

        if ((*textures)->hide == 1) {
            for (int i = a; i < (*textures)->nbImage; i++) {
                if (i == Found(*textures, "sun"))continue;
                for (int j = 0; j < 340; j++) {
                    (*textures)->list[i]->r->x += 1;
                }
            }
            replace_surface(&((*textures)->list[Found(*textures, "rouleau_s")]), renderer, "../Assets/Images/Labeur_pas_paye/Barl_D4rk.png");

            (*textures)->hide = 0;
           
            return;
        }
    }
    
    
    
}

void replace_texture(MenuTextures** textures, MenuTextures* textures_statique, SDL_Renderer * renderer, int x, int y) {
    
    int nbImage;
    int m = (*textures)->replace;

    
    if ((*textures)->hide == 0 && (*textures)->replace ==0) {
        int a = (textures_statique->nbImage) / 2;
        if (a % 2 == 1) a++;
        for (nbImage = 2; nbImage < a-1; nbImage++) {
            if (x >= textures_statique->list[nbImage]->r->x && x <= textures_statique->list[nbImage]->r->x + textures_statique->list[nbImage]->r->w)
                if (y >= textures_statique->list[nbImage]->r->y && y <= textures_statique->list[nbImage]->r->y + textures_statique->list[nbImage]->r->h) {
                    replace1(textures, renderer, nbImage);
                    return;
                }
        }

    }
    
    
    if ( m!=0) {
        if (x< textures_statique->list[m]->r->x || x >textures_statique->list[m]->r->x + textures_statique->list[m]->r->w) {  
            printf("Et merde");
            replace2(textures, renderer);
        }
        if (y <textures_statique->list[m]->r->y || y >textures_statique->list[m]->r->y + textures_statique->list[m]->r->h) {
            replace2(textures, renderer);
            printf("Et merde");
        }
        
    }
}

void replace1(MenuTextures** textures, SDL_Renderer* renderer, int image) {
    if ((*textures)->replace == 0) {
        if ((*textures)->color == 0) {
            if (image == Found(*textures, "souris")) {
                replace_surface(&((*textures)->list[image]), renderer, "../Assets/Images/Labeur_pas_paye/Cur_L1ght1.png");
            }
            if (image == Found(*textures, "segment")) {
                replace_surface(&((*textures)->list[image]), renderer, "../Assets/Images/Labeur_pas_paye/Seg_L1ght1.png");
            }
            if (image == Found(*textures, "cercle")) {
                replace_surface(&((*textures)->list[image]), renderer, "../Assets/Images/Labeur_pas_paye/Cir_L1ght1.png");
            }
            if (image == Found(*textures, "minus")) {
                replace_surface(&((*textures)->list[image]), renderer, "../Assets/Images/Labeur_pas_paye/Min_L1ght1.png");
            }
            if (image == Found(*textures, "plus")) {
                replace_surface(&((*textures)->list[image]), renderer, "../Assets/Images/Labeur_pas_paye/Plu_L1ght1.png");
            }
            if (image == Found(*textures, "point")) {
                replace_surface(&((*textures)->list[image]), renderer, "../Assets/Images/Labeur_pas_paye/Dot_L1ght1.png");
            }
            if (image == Found(*textures, "moon")) {
                replace_surface(&((*textures)->list[image]), renderer, "../Assets/Images/Labeur_pas_paye/Moo_L1ght1.png");
            }
            
        }
        if ((*textures)->color == 1) {
            
            if (image == Found(*textures, "souris")) {
                replace_surface(&((*textures)->list[Found(*textures, "souris_s")]), renderer, "../Assets/Images/Labeur_pas_paye/Cur_D4rk1.png");
            }
            if (image == Found(*textures, "segment")) {
                replace_surface(&((*textures)->list[Found(*textures, "segment_s")]), renderer, "../Assets/Images/Labeur_pas_paye/Seg_D4rk1.png");
            }
            if (image == Found(*textures, "cercle")) {
                replace_surface(&((*textures)->list[Found(*textures, "cercle_s")]), renderer, "../Assets/Images/Labeur_pas_paye/Cir_D4rk1.png");
            }
            if (image == Found(*textures, "minus")) {
                replace_surface(&((*textures)->list[Found(*textures, "minus_s")]), renderer, "../Assets/Images/Labeur_pas_paye/Min_D4rk1.png");
            }
            if (image == Found(*textures, "plus")) {
                replace_surface(&((*textures)->list[Found(*textures, "plus_s")]), renderer, "../Assets/Images/Labeur_pas_paye/Plu_D4rk1.png");
            }
            if (image == Found(*textures, "point")) {
                replace_surface(&((*textures)->list[Found(*textures, "point_s")]), renderer, "../Assets/Images/Labeur_pas_paye/Dot_D4rk1.png");
            }
            if (image == Found(*textures, "moon")) {
                replace_surface(&((*textures)->list[Found(*textures, "sun")]), renderer, "../Assets/Images/Labeur_pas_paye/Sun_D4rk1.png");
            }
           
        }
    }
    (*textures)->replace = image;
}
void replace2(MenuTextures** textures, SDL_Renderer* renderer) {
    int image = (*textures)->replace;
    if ((*textures)->color == 0) {
        if (image == Found(*textures, "souris")) {
            replace_surface(&((*textures)->list[image]), renderer, "../Assets/Images/Labeur_pas_paye/Cur_L1ght.png");
        }
        if (image == Found(*textures, "segment")) {
            replace_surface(&((*textures)->list[image]), renderer, "../Assets/Images/Labeur_pas_paye/Seg_L1ght.png");
        }
        if (image == Found(*textures, "cercle")) {
            replace_surface(&((*textures)->list[image]), renderer, "../Assets/Images/Labeur_pas_paye/Circle_L1ght.png");
        }
        if (image == Found(*textures, "minus")) {
            replace_surface(&((*textures)->list[image]), renderer, "../Assets/Images/Labeur_pas_paye/Min_L1ght.png");
        }
        if (image == Found(*textures, "plus")) {
            replace_surface(&((*textures)->list[image]), renderer, "../Assets/Images/Labeur_pas_paye/Plu_L1ght.png");
        }
        if (image == Found(*textures, "point")) {
            replace_surface(&((*textures)->list[image]), renderer, "../Assets/Images/Labeur_pas_paye/Dot_L1ght.png");
        }
        if (image == Found(*textures, "moon")) {
            replace_surface(&((*textures)->list[image]), renderer, "../Assets/Images/Labeur_pas_paye/Moo_L1ght.png");
        }
        
    }
    if ((*textures)->color == 1) {
        if (image == Found(*textures, "souris")) {
            replace_surface(&((*textures)->list[Found(*textures, "souris_s")]), renderer, "../Assets/Images/Labeur_pas_paye/Cur_D4rk.png");
        }
        if (image == Found(*textures, "segment")) {
            replace_surface(&((*textures)->list[Found(*textures, "segment_s")]), renderer, "../Assets/Images/Labeur_pas_paye/Seg_D4rk.png");
        }
        if (image == Found(*textures, "cercle")) {
            replace_surface(&((*textures)->list[Found(*textures, "cercle_s")]), renderer, "../Assets/Images/Labeur_pas_paye/Cir_D4rk.png");
        }
        if (image == Found(*textures, "minus")) {
            replace_surface(&((*textures)->list[Found(*textures, "minus_s")]), renderer, "../Assets/Images/Labeur_pas_paye/Min_D4rk.png");
        }
        if (image == Found(*textures, "plus")) {
            replace_surface(&((*textures)->list[Found(*textures, "plus_s")]), renderer, "../Assets/Images/Labeur_pas_paye/Plu_D4rk.png");
        }
        if (image == Found(*textures, "point")) {
            replace_surface(&((*textures)->list[Found(*textures, "point_s")]), renderer, "../Assets/Images/Labeur_pas_paye/Dot_D4rk.png");
        }
        if (image == Found(*textures, "moon")) {
            replace_surface(&((*textures)->list[Found(*textures, "sun")]), renderer, "../Assets/Images/Labeur_pas_paye/Sun_D4rk.png");
        }
    }
    (*textures)->replace = 0;
}

void replace_surface(Image** image, SDL_Renderer* renderer, char* path) {
    printf("Je le suis");
    (*image)->s = IMG_Load(path);
    (*image)->t = SDL_CreateTextureFromSurface(renderer, (*image)->s);
    SDL_FreeSurface((*image)->s);

}