#include "map.h"

int load_world(const char *filename, World *world) {
    if(!world || !filename) {
        return -1;
    }
    int retval = 0;

    FILE *file = fopen(filename, "r");
    if(!file) {
        retval = 1; goto done;
    }
    
    if(fscanf(file, "%d %d", &world->width, &world->height) != 2) {
        retval = 1;
        (void)fclose(file);
        goto done;
    }

    int i = -1;
    while(i++ < (world->height * world->width)) {
        if (fscanf(file, "%d,%d,%d", &world->map[world->n].type, &world->map[world->n].enemyId, &world->map[world->n].eventId) != 3) {
            retval = 3;
            (void)fclose(file);
            goto done;
        }
        world->map[world->n].x = i % world->width;
        world->map[world->n++].y = i / world->width;
    }

    goto done;
done: 
    return retval;
}

void drawMap(SDL_Renderer *renderer, SDL_Texture *textures[TEXTURE_COUNT], World *world, int x, int y) {
    if (x < 0 || y < 0 || y >= TILE_SIZE * world->height || x >= TILE_SIZE * world->width) {
        printf("out of mapbounds\n");
        return;
    }

    int x_off = x / TILE_SIZE;
    int y_off = y / TILE_SIZE;

    int startCol = (x_off / COLS) * COLS;
    int startRow = (y_off / ROWS) * ROWS;

    if (startCol < 0) startCol = 0;
    if (startRow < 0) startRow = 0;

    if (startCol + COLS > world->width) startCol = world->width - COLS;
    if (startRow + ROWS > world->height) startRow = world->height - ROWS;

    for (int row = startRow; row < startRow + ROWS; row++) {
        for (int col = startCol; col < startCol + COLS; col++) {
            int index = row * world->width + col;

            int screenX = (col - startCol) * TILE_SIZE;
            int screenY = (row - startRow) * TILE_SIZE;

            SDL_Rect rect = { screenX, screenY, TILE_SIZE, TILE_SIZE };

            SDL_Texture *texture = NULL;
            if (world->map[index].type >= 0 && world->map[index].type < TEXTURE_COUNT) {
                texture = textures[world->map[index].type];
            }

            if (texture != NULL) {
                SDL_RenderCopy(renderer, texture, NULL, &rect);
            } else {
                SDL_RenderCopy(renderer, textures[0], NULL, &rect);
            }
        }
    }

    int redPointX = x % TILE_SIZE;
    int redPointY = y % TILE_SIZE;

    SDL_Rect redPoint = {
        (x_off - startCol) * TILE_SIZE + redPointX - TILE_SIZE / 4,
        (y_off - startRow) * TILE_SIZE + redPointY - TILE_SIZE / 4,
        TILE_SIZE / 2,
        TILE_SIZE / 2
    };

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &redPoint);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}