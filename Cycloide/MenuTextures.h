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

}Image;

typedef struct MenuTextures_s
{
    Image *image;
    struct MenuTextures_s* m_next;
    char *name;

} MenuTextures;


void Create(MenuTextures** textures, SDL_Renderer* renderer, char* path,char* name,  int x, int y, int h, int w);

MenuTextures** MenuTextures_new(SDL_Renderer* renderer);
void MenuTextures_free(MenuTextures* textures);
void Image_free(Image* im);
void Deroulement(MenuTextures* textures, bool rouleau, bool rouleau2);

MenuTextures* Found(MenuTextures* textures, char *name);