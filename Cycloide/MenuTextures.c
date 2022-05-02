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
    textures->replace = 0;

    //Clair
    Create_Menu(&textures, renderer, "../Assets/Images/Labeur_pas_paye/Barl_L1ght.png", "rouleau",  330,  0, 720, 50);

    Create_Menu(&textures, renderer, "../Assets/Images/Menu/white_background.png", "background", 0,  0, 720, 330);

    Create_Menu(&textures, renderer, "../Assets/Images/Labeur_pas_paye/Cur_L1ght.png", "souris", 10, 100, 140, 140);

    Create_Menu(&textures, renderer, "../Assets/Images/Labeur_pas_paye/Dot_L1ght.png", "point", 10, 250, 140, 140);

    Create_Menu(&textures, renderer, "../Assets/Images/Labeur_pas_paye/Seg_L1ght.png", "segment", 170, 100, 140, 140);

    Create_Menu(&textures, renderer, "../Assets/Images/Labeur_pas_paye/Circle_L1ght.png", "cercle", 170, 250, 140, 140);

    Create_Menu(&textures, renderer, "../Assets/Images/Labeur_pas_paye/Min_L1ght.png", "minus",  100,  30, 50, 50);

    Create_Menu(&textures, renderer, "../Assets/Images/Labeur_pas_paye/Plu_L1ght.png", "plus", 160, 30, 50, 50);

    Create_Menu(&textures, renderer, "../Assets/Images/Labeur_pas_paye/Moo_L1ght.png", "moon", 900, 30, 70, 70);

    //Sombre si on veut commencer direct à sombre
    /*
    Create_Menu(&textures, renderer, "../Assets/Images/Labeur_pas_paye/Barl_D4rk.png", "rouleau_s", 330, 0, 720, 50);

    Create_Menu(&textures, renderer, "../Assets/Images/Menu/black_background.png", "background_s", 0, 0, 720, 330);

    Create_Menu(&textures, renderer, "../Assets/Images/Labeur_pas_paye/Cur_D4rk.png", "souris_s", 10, 100, 140, 140);

    Create_Menu(&textures, renderer, "../Assets/Images/Labeur_pas_paye/Dot_D4rk.png", "point_s", 10, 250, 140, 140);

    Create_Menu(&textures, renderer, "../Assets/Images/Labeur_pas_paye/Seg_D4rk.png", "segment_s", 170, 100, 140, 140);

    Create_Menu(&textures, renderer, "../Assets/Images/Labeur_pas_paye/Cir_D4rk.png", "cercle_s", 170, 250, 140, 140);

    Create_Menu(&textures, renderer, "../Assets/Images/Labeur_pas_paye/Min_D4rk.png", "minus_s", 100, 30, 50, 50);

    Create_Menu(&textures, renderer, "../Assets/Images/Labeur_pas_paye/Plu_D4rk.png", "plus_s", 160, 30, 50, 50);

    Create_Menu(&textures, renderer, "../Assets/Images/Labeur_pas_paye/Sun_D4rk.png", "sun", 900, 30, 70, 70);
    */

    
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
    texture->r = (SDL_Rect*)calloc(1, sizeof(SDL_Rect));

    int i = 0;
    while (name[i]) {
        i++;
    }

    texture->name = (char*)calloc(i, sizeof(char));
    for (int j = 0; j <= i; j++) {
        texture->name[j] = name[j];
    }

    texture->s = IMG_Load(path);
    texture->t = SDL_CreateTextureFromSurface(renderer, texture->s);
    SDL_FreeSurface(texture->s);

    texture->r->x = x;
    texture->r->y = y;
    texture->r->h = h;
    texture->r->w = w;

    return texture;

}

int Found(MenuTextures* textures, char* name) {

    if (!textures)return NULL;
    int i = 0;
    int a;

    for (i; i < textures->nbImage; i++) {
        int j = 0;
        a = 0;
        
        while (name[j]) {
            
            if (textures->list[i]->name[j] != name[j]) {
                a = 1;
                break;
            }
            j++;

        }
        if (a == 0) {
            return i;
        }
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


    
    for (int i = 0; i < texture->nbImage; i++) {
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
