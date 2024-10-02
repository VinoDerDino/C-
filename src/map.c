#include "map.h"

int load_scene(const char *filename, Scene *scene) {
    if(!scene || !filename) {
        return -1;
    }

    int retval = 0;
    FILE *file = fopen(filename, "r");
    if(!file) {
        retval = 1; goto done;
    }

    int sector_count = 0;
    /* Reading Sector_Count */
    if(fscanf(file, "%d", &sector_count) != 1) {
        retval = 2; goto done;
    }

    for(int i = 0; i < sector_count; i++) {
        /* Reading Sector_ID */
        if(fscanf(file, "%d", &scene->sectors[scene->n_sectors].id) != 1) {
            retval = 3; goto done;
        }
        /* Reading Sector_Neighbors */
        if(fscanf(file, "%d,%d,%d,%d", &scene->sectors[scene->n_sectors].neighbors[0], &scene->sectors[scene->n_sectors].neighbors[1], &scene->sectors[scene->n_sectors].neighbors[2], &scene->sectors[scene->n_sectors].neighbors[3]) != 4) {
            retval = 4; goto done;
        }

        /* Reading Sector_Tiles */
        for(int i = 0; i < 100; i++) {
            if(fscanf(file, "%d,%d,%d", 
                        &scene->sectors[scene->n_sectors].map[i].type,
                        &scene->sectors[scene->n_sectors].map[i].enemyId, 
                        &scene->sectors[scene->n_sectors].map[i].eventId) != 3) {
                retval = 5; goto done;
            }
        }

        scene->n_sectors += 1;
    }

    goto done;
done: 
    (void)fclose(file);
    return retval;
}

void drawMap(SDL_Renderer *renderer, SDL_Texture *textures[TILE_TEXTURE_COUNT], Scene scene, int x, int y) {
    if (x < 0 || y < 0 || y >= TILE_SIZE * ROWS || x >= TILE_SIZE * COLS) {
        printf("out of mapbounds x=%d, y=%d\n", x, y);
        return;
    }

    for(int row = 0; row < ROWS; row++) {
        for(int col = 0; col < COLS; col++) {
            int index = row * COLS + col;

            int screenX = col * TILE_SIZE;
            int screenY = row * TILE_SIZE;

            SDL_Rect rect = { screenX, screenY, TILE_SIZE, TILE_SIZE };
            
            SDL_Texture *texture = NULL;
            if (scene.sectors[scene.curr_sector].map[index].type >= 0 && scene.sectors[scene.curr_sector].map[index].type < TILE_TEXTURE_COUNT) {
                texture = textures[scene.sectors[scene.curr_sector].map[index].type];
            }

            if (texture != NULL) {
                SDL_RenderCopy(renderer, texture, NULL, &rect);
            } else {
                SDL_RenderCopy(renderer, textures[0], NULL, &rect);
            }
        }
    }

    SDL_Rect redPoint = {
        x - TILE_SIZE / 4,
        y - TILE_SIZE / 4,
        TILE_SIZE / 2,
        TILE_SIZE / 2
    };

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &redPoint);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

void map_check_enemyId(Scene *scene, int x, int y){
    int col = x / TILE_SIZE;
    int row = y / TILE_SIZE;

    (void)col;
    (void)row;

    (void)scene;
}