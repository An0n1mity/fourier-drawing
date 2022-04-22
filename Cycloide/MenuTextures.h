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
/*
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

typedef struct Textures_s
{
    Image* image;
    char* name[128];
    struct MenuTextures* m_next;

}Textures;

typedef struct MenuTextures_s
{
    Textures* begin;

} MenuTextures;
*/
typedef struct MenuTextures_s
{
    int nbImage;
    Image* list[5];

} MenuTextures;


void Create_Menu(MenuTextures** textures, SDL_Renderer* renderer, char* path,char *name, int x, int y, int h, int w);
Image* CreateMenuTexture(SDL_Renderer*  renderer, char*  path, char*  name, int x, int y, int h, int w);


MenuTextures* MenuTextures_new(SDL_Renderer* renderer);
void MenuTextures_free(MenuTextures* textures);
void Image_free(Image* im);
void Deroulement(MenuTextures* textures, bool rouleau, bool rouleau2);

int Found(MenuTextures* textures, char *name);