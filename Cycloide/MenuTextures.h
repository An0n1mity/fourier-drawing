#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <stdbool.h>

typedef struct Image_s
{
    SDL_Texture* t;
    SDL_Surface* s;
    SDL_Rect* r;
    char* name[128];

}Image;

typedef struct MenuTextures_s
{
    int hide;
    int nbImage;
    Image* list[5];

} MenuTextures;


void Create_Menu(MenuTextures** textures, SDL_Renderer* renderer, char* path, char* name, int x, int y, int h, int w);
Image* CreateMenuTexture(SDL_Renderer* renderer, char* path, char* name, int x, int y, int h, int w);


MenuTextures* MenuTextures_new(SDL_Renderer* renderer);
void MenuTextures_free(MenuTextures* textures);
void Image_free(Image* im);


int Found(MenuTextures* textures, char* name);