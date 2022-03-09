#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "MenuTextures.h"
#include "SDL.h"
#include "SDL_image.h"



MenuTextures* MenuTextures_new(SDL_Renderer* renderer) {
    MenuTextures* textures = (MenuTextures*) calloc(1, sizeof(MenuTextures));


    /*textures->logo->s = IMG_Load("../Assets/Images/Menu/Logo.png");
    textures->logo->t = SDL_CreateTextureFromSurface(renderer, textures->logo->s);
    SDL_FreeSurface(textures->logo->s);

    textures->logo->r->x = 0;
    textures->logo->r->y = 0;
    textures->logo->r->h = 0;
    textures->logo->r->w = 0;*/

    textures->point = (Image*)calloc(1, sizeof(Image));
    textures->point->s = IMG_Load("../Assets/Images/Menu/point.png");
    textures->point->t = SDL_CreateTextureFromSurface(renderer, textures->point->s);
    SDL_FreeSurface(textures->point->s);

    textures->point->r = (SDL_Rect*)calloc(1, sizeof(SDL_Rect));
    textures->point->r->x = 10;
    textures->point->r->y = 100;
    textures->point->r->h = 100;
    textures->point->r->w = 100;

    textures->cercle = (Image*)calloc(1, sizeof(Image));
    textures->cercle->s = IMG_Load("../Assets/Images/Menu/cercle.png");
    textures->cercle->t = SDL_CreateTextureFromSurface(renderer, textures->cercle->s);
    SDL_FreeSurface(textures->cercle->s);

    textures->cercle->r = (SDL_Rect*)calloc(1, sizeof(SDL_Rect));
    textures->cercle->r->x = 130 ;
    textures->cercle->r->y = 100;
    textures->cercle->r->h = 100;
    textures->cercle->r->w = 100;


    textures->segment= (Image*)calloc(1, sizeof(Image));
    textures->segment->s = IMG_Load("../Assets/Images/Menu/segment.png");
    textures->segment->t = SDL_CreateTextureFromSurface(renderer, textures->segment->s);
    SDL_FreeSurface(textures->segment->s);

    textures->segment->r = (SDL_Rect*)calloc(1, sizeof(SDL_Rect));
    textures->segment->r->x = 10;
    textures->segment->r->y = 210;
    textures->segment->r->h = 100;
    textures->segment->r->w = 100;


    textures->rouleau = (Image*)calloc(1, sizeof(Image));
    textures->rouleau->s = IMG_Load("../Assets/Images/Menu/rouleau.png");
    textures->rouleau->t = SDL_CreateTextureFromSurface(renderer, textures->rouleau->s);
    SDL_FreeSurface(textures->rouleau->s);

    textures->rouleau->r = (SDL_Rect*)calloc(1, sizeof(SDL_Rect));
    textures->rouleau->r->x = 330;
    textures->rouleau->r->y = 0;
    textures->rouleau->r->h = 720;
    textures->rouleau->r->w = 50;

    textures->background = (Image*)calloc(1, sizeof(Image));
    textures->background->s = IMG_Load("../Assets/Images/Menu/black_background.png");
    textures->background->t = SDL_CreateTextureFromSurface(renderer, textures->background->s);
    SDL_FreeSurface(textures->background->s);

    textures->background->r = (SDL_Rect*)calloc(1, sizeof(SDL_Rect));
    textures->background->r->x = 0;
    textures->background->r->y = 0;
    textures->background->r->h = 720;
    textures->background->r->w = 330;




    return textures;
}


void Image_free(Image* im) {
    if (!im)
        return;
    SDL_DestroyTexture(im->t);
    SDL_FreeSurface(im->s);
    free(im->r);
   
}

void MenuTextures_free(MenuTextures* textures) {

    if (!textures)
        return;

    //Image_free(textures->logo);
    Image_free(textures->point);
    Image_free(textures->segment);
    Image_free(textures->cercle);
    Image_free(textures->rouleau);
    Image_free(textures->background);


    free(textures);
}



void Deroulement(MenuTextures* textures, bool rouleau, bool rouleau2) {

   /*
    if (textures->rouleau->r->x < -10)
        rouleau = false;

    if (textures->rouleau->r->x > 330)
        rouleau2 = false;

    if (rouleau == true) {
        printf("Je suis dans rouleau\n");
        textures->rouleau->r->x -= 1;
        textures->background->r->x -= 1;
        if (textures->rouleau->r->x == (textures->cercle->r->x+ textures->segment->r->w)) {
            textures->cercle->r->x -= 1;
            textures->segment->r->x -= 1;
            textures->point->r->x -= 1;
        }
        printf("%d \n", textures->rouleau->r->x);
        
    }
    if (rouleau2 == true) {

        printf("Je suis dans rouleau2\n");
        textures->rouleau->r->x += 1;
        textures->background->r->x += 1;
        textures->cercle->r->x += 1;
        textures->segment->r->x += 1;
        textures->point->r->x += 1;

      
    }
    
    
   */
}