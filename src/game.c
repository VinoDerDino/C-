#include <stdio.h>
#include <SDL2/SDL.h>
#include "tile.h"
#include "map.h"
#include "player.h"

int init_state(GameState *gamestate) {

    gamestate->player.x = 120;
    gamestate->player.y = 120;
    gamestate->player.curr_scene = 0;

    gamestate->scenes[0].n_sectors = 0;
    gamestate->scenes[0].curr_sector = 0;

    int loading = load_scene("res/level1.txt", &gamestate->scenes[0]);
    if(loading != 0) {
        printf("Error while loading: %d", loading);
        return 1;
    };

    gamestate->window = SDL_CreateWindow("Tile Map", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ROWS * TILE_SIZE, COLS * TILE_SIZE, SDL_WINDOW_SHOWN);
    if (!gamestate->window) {
        (void)printf("Window could not be created!\n");
        return 1;
    }

    gamestate->renderer = SDL_CreateRenderer(gamestate->window, -1, SDL_RENDERER_ACCELERATED);
    if(!gamestate->renderer) {
        (void)printf("Renderer could not be created!\n");
        return 1;
    }

    gamestate->tile_textures[0] = load_texture("res/texture_missing.bmp", gamestate->renderer);
    gamestate->tile_textures[1] = load_texture("res/grass.bmp", gamestate->renderer);
    gamestate->tile_textures[2] = load_texture("res/stone.bmp", gamestate->renderer);
    gamestate->tile_textures[3] = load_texture("res/water.bmp", gamestate->renderer);

    if (gamestate->tile_textures[0] == NULL || gamestate->tile_textures[1] == NULL || gamestate->tile_textures[2] == NULL || gamestate->tile_textures[2] == NULL) {
        printf("Failed to load one or more textures!\n");
        return 1;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    GameState gamestate;
    if(init_state(&gamestate) != 0) {
        printf("Error while initialising 'state'\n");
        return 1;
    }

    int running = 1;
    SDL_Event event;

    int frameCount = 0;
    float fps = 0.0f;
    Uint32 startTime = SDL_GetTicks();

    while (running) {
        while (SDL_PollEvent(&event)) {

            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }
        const uint8_t *keystate = SDL_GetKeyboardState(NULL);

        player_move(keystate, 0, &gamestate);
        
        map_check_enemyId(&gamestate.scenes[0], gamestate.player.x, gamestate.player.y);

        SDL_RenderClear(gamestate.renderer);
        drawMap(gamestate.renderer, gamestate.tile_textures, gamestate.scenes[0], gamestate.player.x, gamestate.player.y);
        SDL_RenderPresent(gamestate.renderer);

        frameCount++;
        if (SDL_GetTicks() > startTime + 1000) {
            fps = frameCount / ((SDL_GetTicks() - startTime) / 1000.0f);
            frameCount = 0;
            startTime = SDL_GetTicks();
            (void)printf("FPS: %f\n", fps);
        }
    }

    for (int i = 0; i < 4; i++)
    {
        SDL_DestroyTexture(gamestate.tile_textures[i]);
    }
    SDL_DestroyRenderer(gamestate.renderer);
    SDL_DestroyWindow(gamestate.window);
    SDL_Quit();

    return 0;
}