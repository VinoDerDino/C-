#ifndef TILE_H_
#define TILE_H_

#include <SDL2/SDL.h>

#define TILE_SIZE 64
#define TEXTURE_COUNT 4

typedef struct {
    SDL_Texture *texture;
    int type;
    int enemyId;
    int eventId;
    int x, y;
    int id;
} Tile;

SDL_Texture* load_texture(const char *file, SDL_Renderer* renderer);

#endif