#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>
#include "MenuTextures.h"
#include "Camera.h"


void Interact(MenuTextures** texture, MenuTextures** textures_statique, Camera** camera, SDL_Renderer* renderer);

void Deroulement(MenuTextures **textures); 

