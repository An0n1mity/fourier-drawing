#include <stdio.h>
#include <stdlib.h>
#include "MenuTextures.h"
#include "SDL.h"
#include "SDL_image.h"




MenuTextures* MenuTextures_new(SDL_Renderer* renderer) {
    MenuTextures* textures = NULL;


    textures->logo->s = IMG_Load("../Assets/Images/Menu/Logo.png");
    textures->logo->t = SDL_CreateTextureFromSurface(renderer, textures->logo->s);
    SDL_FreeSurface(textures->logo->s);

    textures->logo->r->x = 0;
    textures->logo->r->y = 0;
    textures->logo->r->h = 0;
    textures->logo->r->w = 0;

    textures->logo->s = IMG_Load("../Assets/Images/Menu/point.png");
    textures->point->t = SDL_CreateTextureFromSurface(renderer, textures->point->s);
    SDL_FreeSurface(textures->point->s);

    textures->cercle->s = IMG_Load("../Assets/Images/Menu/cercle.png");
    textures->cercle->t = SDL_CreateTextureFromSurface(renderer, textures->cercle->s);
    SDL_FreeSurface(textures->cercle->s);

    textures->segment->s = IMG_Load("../Assets/Images/Menu/segment.png");
    textures->segment->t = SDL_CreateTextureFromSurface(renderer, textures->segment->s);
    SDL_FreeSurface(textures->segment->s);

    textures->menu->s = IMG_Load("../Assets/Images/Menu/menu.png");
    textures->menu->t = SDL_CreateTextureFromSurface(renderer, textures->menu->s);
    SDL_FreeSurface(textures->menu->s);



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

    Image_free(textures->logo);
    Image_free(textures->point);
    Image_free(textures->segment);
    Image_free(textures->cercle);
    Image_free(textures->menu);
    free(textures);
}