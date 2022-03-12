#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "MenuTextures.h"
#include "SDL.h"
#include "SDL_image.h"


void Create(MenuTextures **textures, SDL_Renderer* renderer, char* path,char* name, int x, int y, int h, int w) {

    int a = 0;
    int b = 0;
    if ((*textures)->image == NULL)b = 1;

    while ((textures != NULL) || b==1) {
        textures = (*textures)->m_next;
        a++;
    }
    if(a!=0)
        textures = (MenuTextures*)calloc(1, sizeof(MenuTextures));

   
    (*textures)->name = "stp";
    (*textures)->image = (Image*)calloc(1, sizeof(Image));

    (*textures)->image->s = IMG_Load(path);
    (*textures)->image->t = SDL_CreateTextureFromSurface(renderer, (*textures)->image->s);
    SDL_FreeSurface((*textures)->image->s);

    (*textures)->image->r = (SDL_Rect*)calloc(1, sizeof(SDL_Rect));
    (*textures)->image->r->x = x;
    (*textures)->image->r->y = y;
    (*textures)->image->r->h = h;
    (*textures)->image->r->w = w;
        
}



MenuTextures* MenuTextures_new(SDL_Renderer* renderer) {
    MenuTextures* textures = (MenuTextures*)calloc(1, sizeof(MenuTextures));



    Create(&textures, renderer, "../Assets/Images/Menu/rouleau.png","rouleau", 330, 0, 720, 50);

    Create(&textures, renderer, "../Assets/Images/Menu/black_background.png", "background", 0, 0, 720, 330);

    Create(&textures, renderer, "../Assets/Images/Menu/point.png","point", 10, 100, 100, 100);

    Create(&textures, renderer, "../Assets/Images/Menu/cercle.png","cercle", 130, 100, 100, 100);


    Create(&textures, renderer, "../Assets/Images/Menu/segment.png","segment", 10, 210, 100, 100);


    

    return textures;
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

    while (textures != NULL) {
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