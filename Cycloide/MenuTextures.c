#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "MenuTextures.h"
#include "SDL.h"
#include "SDL_image.h"
#include "Camera.h"


MenuTextures* MenuTextures_new(SDL_Renderer* renderer) {
    MenuTextures* textures = (MenuTextures*)calloc(1, sizeof(MenuTextures));
    textures->nbImage = 0;
    textures->hide = 0;
    textures->color = 0;

    //Clair
    Create_Menu(&textures, renderer, "../Assets/Images/Menu/rouleau.png", "rouleau",  310,  0, 720, 50);

    Create_Menu(&textures, renderer, "../Assets/Images/Menu/white_background.png", "background", 0,  0, 720, 330);

    Create_Menu(&textures, renderer, "../Assets/Images/Labeur_pas_paye/Cur_L1ght.png", "souris", 10, 100, 140, 140);

    Create_Menu(&textures, renderer, "../Assets/Images/Labeur_pas_paye/Dot_L1ght.png", "point", 10, 250, 140, 140);

    Create_Menu(&textures, renderer, "../Assets/Images/Labeur_pas_paye/Seg_L1ght.png", "segment", 170, 100, 140, 140);

    Create_Menu(&textures, renderer, "../Assets/Images/Labeur_pas_paye/Circle_L1ght.png", "cercle", 170, 250, 140, 140);

    Create_Menu(&textures, renderer, "../Assets/Images/Labeur_pas_paye/Min_L1ght.png", "1min",  100,  30, 50, 50);

    Create_Menu(&textures, renderer, "../Assets/Images/Labeur_pas_paye/Plu_L1ght.png", "plus", 160, 30, 50, 50);

    Create_Menu(&textures, renderer, "../Assets/Images/Labeur_pas_paye/Moo_D4rk.png", "moon", 900, 30, 70, 70);

    //Sombre
    Create_Menu(&textures, renderer, "../Assets/Images/Menu/rouleau.png", "rouleau", 310, 0, 720, 50);

    Create_Menu(&textures, renderer, "../Assets/Images/Menu/black_background.png", "background", 0, 0, 720, 330);

    Create_Menu(&textures, renderer, "../Assets/Images/Labeur_pas_paye/Cur_D4rk.png", "souris", 10, 100, 140, 140);

    Create_Menu(&textures, renderer, "../Assets/Images/Labeur_pas_paye/Dot_D4rk.png", "point", 10, 250, 140, 140);

    Create_Menu(&textures, renderer, "../Assets/Images/Labeur_pas_paye/Seg_D4rk.png", "segment", 170, 100, 140, 140);

    Create_Menu(&textures, renderer, "../Assets/Images/Labeur_pas_paye/Cir_D4rk.png", "cercle", 170, 250, 140, 140);

    Create_Menu(&textures, renderer, "../Assets/Images/Labeur_pas_paye/Min_D4rk.png", "1min", 100, 30, 50, 50);

    Create_Menu(&textures, renderer, "../Assets/Images/Labeur_pas_paye/Plu_D4rk.png", "plus", 160, 30, 50, 50);

    Create_Menu(&textures, renderer, "../Assets/Images/Labeur_pas_paye/Sun_L1ght.png", "sun", 900, 30, 70, 70);


    
    return textures;
}


void Create_Menu(MenuTextures** textures, SDL_Renderer* renderer, char* path, char* name, int x, int y, int h, int w)
{
    // If no textures were created
    if (!*textures)
    {
        *textures = CreateMenuTexture(renderer, path, name, x, y, h, w);
        return;
    }

    // Stack adding
    Image* texture_to_add = CreateMenuTexture(renderer, path, name, x, y, h, w);

    (*textures)->list[(*textures)->nbImage] = texture_to_add;
    (*textures)->nbImage += 1;

}

Image* CreateMenuTexture(SDL_Renderer* renderer, char* path, char* name, int x, int y, int h, int w) {
    Image* texture = (Image*)calloc(1, sizeof(Image));
    *texture->name = (char*)calloc(128, sizeof(char));
    texture->r = (SDL_Rect*)calloc(1, sizeof(SDL_Rect));


    int i = 0;
    while (name[i]) {
        texture->name[i] = name[i];
        i++;
    }
    //printf("%s %s\n", texture->name, name);

    texture->s = IMG_Load(path);
    texture->t = SDL_CreateTextureFromSurface(renderer, texture->s);
    SDL_FreeSurface(texture->s);

    texture->r->x = x;
    texture->r->y = y;
    texture->r->h = h;
    texture->r->w = w;

    return texture;

}

int Found(MenuTextures* texture, char* name) {

    if (!texture)return NULL;
    int i = 0;
    for (i; i <= texture->nbImage; i++) {
        if (name == texture->list[i]->name) return i;
    }


    return -1;
}

void Image_free(Image* im) {
    if (!im)
        return;

    //free(im->r);
    //SDL_DestroyTexture(im->t);
    //SDL_FreeSurface(im->s);

}

void render(SDL_Renderer* renderer, MenuTextures* texture) {

    if (texture == NULL)return;

    int a = 0;
    int b = 0;

    if (texture->color == 0) {
        a = 0;
        b = texture->nbImage / 2;
    }
    if (texture->color == 1) {
        a = texture->nbImage / 2;
        b = texture->nbImage;
    }
    for (int i = a; i < b; i++) {
        SDL_RenderCopy(renderer, texture->list[i]->t, NULL, texture->list[i]->r);
    }
    SDL_RenderPresent(renderer);
}

void MenuTextures_free(MenuTextures* texture) {

    if (!texture)
        return;

    for (int i = 0; i < texture->nbImage; i++) {
        Image_free(texture->list[i]);

    }

    free(texture);
}
