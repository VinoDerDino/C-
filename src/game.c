#include <stdio.h>
#include <SDL2/SDL.h>
#include "tile.h"
#include "map.h"

typedef struct
{
    int state;
    int x, y;
} Player;

typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;

    World world;
    Player player;
} State;

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    State state;
    state.player.x = 120;
    state.player.y = 120;

    state.world.n = 0;
    int retval = load_world("res/map.txt", &state.world);

    if (retval != 0)
    {
        printf("terminated, error: %d", retval);
        return 1;
    }

    printf("test\n");
    state.window = SDL_CreateWindow("Tile Map", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ROWS * TILE_SIZE, COLS * TILE_SIZE, SDL_WINDOW_SHOWN);
    if (!state.window)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    state.renderer = SDL_CreateRenderer(state.window, -1, SDL_RENDERER_ACCELERATED);
    if (!state.renderer)
    {
        SDL_DestroyWindow(state.window);
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Texture *textures[TEXTURE_COUNT];
    textures[0] = load_texture("res/texture_missing.bmp", state.renderer);
    textures[1] = load_texture("res/grass.bmp", state.renderer);
    textures[2] = load_texture("res/stone.bmp", state.renderer);
    textures[3] = load_texture("res/water.bmp", state.renderer);

    if (textures[0] == NULL || textures[1] == NULL || textures[2] == NULL || textures[2] == NULL)
    {
        printf("Failed to load one or more textures!\n");
        return 1;
    }

    (void)printf("Map dimensions: rows = %d, cols = %d\n", state.world.height, state.world.width);

    int running = 1;
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }

            const uint8_t *keystate = SDL_GetKeyboardState(NULL);

            if (keystate[SDLK_RIGHT & 0xFFFF])
            {
                state.player.x += MOVE_SPEED;
                if (state.player.x >= state.world.width * TILE_SIZE)
                    state.player.x = state.world.width * TILE_SIZE - 1;
            }

            if (keystate[SDLK_LEFT & 0xFFFF])
            {
                state.player.x -= MOVE_SPEED;
                if (state.player.x < 0)
                    state.player.x = 0;
            }

            if (keystate[SDLK_UP & 0xFFFF])
            {
                state.player.y -= MOVE_SPEED;
                if (state.player.y < 0)
                    state.player.y = 0;
            }

            if (keystate[SDLK_DOWN & 0xFFFF])
            {
                state.player.y += MOVE_SPEED;
                if (state.player.y >= state.world.height * TILE_SIZE)
                    state.player.y = state.world.height * TILE_SIZE - 1;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    int x = (int)(event.button.x / TILE_SIZE), y = (int)(event.button.y / TILE_SIZE);

                    for (int i = 0; i < state.world.n; i++)
                    {
                        if (x == state.world.map[i].x && y == state.world.map[i].y)
                        {
                            printf("x = %d, y = %d, id = %d, type = %d\n", i % state.world.width, (int)(i / state.world.width), state.world.map[i].id, state.world.map[i].type);
                        }
                    }
                }
            }
        }

        SDL_RenderClear(state.renderer);
        drawMap(state.renderer, textures, &state.world, state.player.x, state.player.y);
        SDL_RenderPresent(state.renderer);
    }

    for (int i = 0; i < 3; i++)
    {
        SDL_DestroyTexture(textures[i]);
    }
    SDL_DestroyRenderer(state.renderer);
    SDL_DestroyWindow(state.window);
    SDL_Quit();

    return 0;
}