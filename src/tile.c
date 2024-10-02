#include "tile.h"
#include <stdio.h>

SDL_Texture *load_texture(const char *file, SDL_Renderer *renderer) {
    SDL_Surface *surface = SDL_LoadBMP(file);
    if(!surface) {
        printf("Unalbe to load BMP: %s\n", file);
        return NULL;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}