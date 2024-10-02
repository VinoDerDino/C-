#ifndef TILE_H_
#define TILE_H_

#include <SDL2/SDL.h>
#include <stdbool.h>

#define TILE_SIZE 64
#define TILE_TEXTURE_COUNT 4
#define PLAYER_TEXTURE_COUNT 1

typedef struct {
    int type;
    int enemyId;
    int eventId;
    int id;
   // bool triggered;
} Tile;

SDL_Texture* load_texture(const char *file, SDL_Renderer* renderer);

#endif