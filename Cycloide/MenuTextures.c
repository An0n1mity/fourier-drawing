#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "MenuTextures.h"
#include "SDL.h"
#include "SDL_image.h"


MenuTextures* CreateMenuTexture(SDL_Renderer* renderer, char* path, char* name, int x, int y, int h, int w) {
    MenuTextures* texture = (MenuTextures*)calloc(1, sizeof(MenuTextures));
    texture->image = (Image*)calloc(1, sizeof(Image));
    texture->image->r = (SDL_Rect*)calloc(1, sizeof(SDL_Rect));

    int i = 0;
    while (name[i]) {
        texture->name[i] = name[i];
        i++;
    }

    texture->image->s = IMG_Load(path);
    texture->image->t = SDL_CreateTextureFromSurface(renderer, texture->image->s);
    SDL_FreeSurface(texture->image->s);

    texture->image->r->x = x;
    texture->image->r->y = y;
    texture->image->r->h = h;
    texture->image->r->w = w;

    return texture;
    
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
    MenuTextures* texture_to_add = CreateMenuTexture(renderer, path, name, x, y, h, w);
    texture_to_add->m_next = *textures;
    *textures = texture_to_add;
}

MenuTextures* MenuTextures_new(SDL_Renderer* renderer) {
    MenuTextures* textures = NULL;
    
    
    Create_Menu(&textures, renderer, "../Assets/Images/Menu/rouleau.png","rouleau", 330, 0, 720, 50);

    Create_Menu(&textures, renderer, "../Assets/Images/Menu/black_background.png", "background", 0, 0, 720, 330);

    Create_Menu(&textures, renderer, "../Assets/Images/Menu/point.png","point", 10, 100, 100, 100);

    Create_Menu(&textures, renderer, "../Assets/Images/Menu/segment.png","segment", 10, 210, 100, 100);

    Create_Menu(&textures, renderer, "../Assets/Images/Menu/cercle.png","cercle", 130, 100, 100, 100);



    return textures;
}

void Printf(MenuTextures* textures) {
    if (!textures)return;

    while (textures) {
        //printf("%s \n", textures->name);
        textures = textures->m_next;
    }

}

MenuTextures* Found(MenuTextures* textures, char* name) {

    if (!textures)return NULL;

    while (name != textures->name) {
        textures = textures->m_next;
    }

    return textures;
}

void Image_free(Image* im) {
    if (!im)
        return;
    SDL_DestroyTexture(im->t);
    SDL_FreeSurface(im->s);
    free(im->r);
   
}

void render(SDL_Renderer* renderer, MenuTextures* textures) {

    if (textures == NULL)return;

    while (textures) {
        SDL_RenderCopy(renderer, textures->image->t, NULL, textures->image->r);
        textures = textures->m_next;
    }
}

void MenuTextures_free(MenuTextures* textures) {

    if (!textures)
        return;
    MenuTextures* tmp = textures->m_next;
    while (textures) {
        Image_free(textures->image);
        free(textures);
        textures = tmp;
        tmp = tmp->m_next;
    }

    free(textures);
}



void Deroulement(MenuTextures* textures, bool rouleau, bool rouleau2) {

   /*
    if (textures->rouleau->r->x < -10)
        rouleau = false;

    if (textures->rouleau->r->x > 330)
        rouleau2 = false;

    if (rouleau == true) {
        printf("Je suis dans rouleau\n");
        textures->rouleau->r->x -= 1;
        textures->background->r->x -= 1;
        if (textures->rouleau->r->x == (textures->cercle->r->x+ textures->segment->r->w)) {
            textures->cercle->r->x -= 1;
            textures->segment->r->x -= 1;
            textures->point->r->x -= 1;
        }
        printf("%d \n", textures->rouleau->r->x);
        
    }
    if (rouleau2 == true) {

        printf("Je suis dans rouleau2\n");
        textures->rouleau->r->x += 1;
        textures->background->r->x += 1;
        textures->cercle->r->x += 1;
        textures->segment->r->x += 1;
        textures->point->r->x += 1;

      
    }
    
    
   */
}