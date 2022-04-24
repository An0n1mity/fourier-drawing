#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "MenuTextures.h"
#include "SDL.h"
#include "SDL_image.h"


MenuTextures* MenuTextures_new(SDL_Renderer* renderer) {
    MenuTextures* textures = (MenuTextures*)calloc(1, sizeof(MenuTextures));
    textures->nbImage = 0;
    textures->hide = 0;


    Create_Menu(&textures, renderer, "../Assets/Images/Menu/rouleau.png", "rouleau", 330, 0, 720, 50);

    Create_Menu(&textures, renderer, "../Assets/Images/Menu/black_background.png", "background", 0, 0, 720, 330);

    Create_Menu(&textures, renderer, "../Assets/Images/Menu/point.png", "point", 10, 100, 100, 100);

    Create_Menu(&textures, renderer, "../Assets/Images/Menu/segment.png", "segment", 10, 210, 100, 100);

    Create_Menu(&textures, renderer, "../Assets/Images/Menu/cercle.png", "cercle", 130, 100, 100, 100);



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
    SDL_DestroyTexture(im->t);
    SDL_FreeSurface(im->s);
    free(im->r);

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

    for (int i = 0; i <= texture->nbImage; i++) {
        Image_free(texture->list[i]);

    }

    free(texture);
}
