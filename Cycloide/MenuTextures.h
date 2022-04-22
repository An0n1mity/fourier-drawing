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

typedef struct Textures_s
{
    int b;

}Textures;

typedef struct MenuTextures_s
{
    
    Image* image;
    char* name[128];
    struct MenuTextures* m_next;

} MenuTextures;


void Create_Menu(MenuTextures** textures, SDL_Renderer* renderer, char* path,char *name, int x, int y, int h, int w);
MenuTextures* CreateMenuTexture(SDL_Renderer*  renderer, char*  path, char*  name, int x, int y, int h, int w);


MenuTextures* MenuTextures_new(SDL_Renderer* renderer);
void MenuTextures_free(MenuTextures* textures);
void Image_free(Image* im);
void Deroulement(MenuTextures* textures, bool rouleau, bool rouleau2);

MenuTextures* Found(MenuTextures* textures, char *name);