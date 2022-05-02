#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>
#include "MenuTextures.h"
#include "Camera.h"


void Interact(MenuTextures** textures, MenuTextures* textures_statique, Camera** camera, SDL_Renderer* renderer);

void Deroulement(MenuTextures **textures, SDL_Renderer *renderer); 

void replace_texture(MenuTextures** textures, MenuTextures* textures_statique,  SDL_Renderer* renderer, int x, int y);

void replace1(MenuTextures** textures, SDL_Renderer* renderer, int image);

void replace2(MenuTextures** textures, SDL_Renderer* renderer);

void replace_surface(Image** image, SDL_Renderer* renderer, char* path);

void Switch_Dark_White(MenuTextures** textures, SDL_Renderer* renderer);
