#ifndef MAP_H_
#define MAP_H_

#include "tile.h"
#include <stdio.h>
#include <stdlib.h>

#define ROWS 10
#define COLS 10

typedef struct
{
    Tile map[1024];
    int n;
    int width, height;
} World;

int load_world(const char *filename, World *world);
void drawMap(SDL_Renderer *renderer, SDL_Texture *textures[TEXTURE_COUNT], World *world, int x, int y);

#endif