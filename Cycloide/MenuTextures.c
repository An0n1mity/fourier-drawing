#include <stdio.h>
#include <stdlib.h>
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
    textures->point->r->x = 0;
    textures->point->r->y = 0;
    textures->point->r->h = 100;
    textures->point->r->w = 100;

    textures->cercle = (Image*)calloc(1, sizeof(Image));
    textures->cercle->s = IMG_Load("../Assets/Images/Menu/cercle.png");
    textures->cercle->t = SDL_CreateTextureFromSurface(renderer, textures->cercle->s);
    SDL_FreeSurface(textures->cercle->s);

    textures->cercle->r = (SDL_Rect*)calloc(1, sizeof(SDL_Rect));
    textures->cercle->r->x = 0;
    textures->cercle->r->y = 0;
    textures->cercle->r->h = 0;
    textures->cercle->r->w = 0;


    textures->segment= (Image*)calloc(1, sizeof(Image));
    textures->segment->s = IMG_Load("../Assets/Images/Menu/segment.png");
    textures->segment->t = SDL_CreateTextureFromSurface(renderer, textures->segment->s);
    SDL_FreeSurface(textures->segment->s);

    textures->segment->r = (SDL_Rect*)calloc(1, sizeof(SDL_Rect));
    textures->segment->r->x = 0;
    textures->segment->r->y = 0;
    textures->segment->r->h = 0;
    textures->segment->r->w = 0;


    textures->menu = (Image*)calloc(1, sizeof(Image));
    textures->menu->s = IMG_Load("../Assets/Images/Menu/menu.png");
    textures->menu->t = SDL_CreateTextureFromSurface(renderer, textures->menu->s);
    SDL_FreeSurface(textures->menu->s);

    textures->menu->r = (SDL_Rect*)calloc(1, sizeof(SDL_Rect));
    textures->menu->r->x = 0;
    textures->menu->r->y = 0;
    textures->menu->r->h = 0;
    textures->menu->r->w = 0;



    return textures;
}


void Image_free(Image* im) {
    if (!im)
        return;
    SDL_DestroyTexture(im->t);
    SDL_FreeSurface(im->s);
   
}

void MenuTextures_free(MenuTextures* textures) {

    if (!textures)
        return;

    //Image_free(textures->logo);
    Image_free(textures->point);
    Image_free(textures->segment);
    Image_free(textures->cercle);
    Image_free(textures->menu);
    free(textures);
}