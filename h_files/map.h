#ifndef MAP_H_
#define MAP_H_

#include "tile.h"
#include <stdio.h>
#include <stdlib.h>

#define ROWS 10
#define COLS 10
#define SCENE_COUNT 1

typedef struct {
    int id;
    Tile map[100];
    int neighbors[4];
} Sector;

typedef struct
{
    Sector sectors[10];
    int curr_sector;
    int n_sectors;
    int scene_portal;
} Scene;

int load_scene(const char *filename, Scene *scene);
void drawMap(SDL_Renderer *renderer, SDL_Texture *textures[TILE_TEXTURE_COUNT], Scene scene, int x, int y);
void map_check_enemyId(Scene *scene, int x, int y);

#endif