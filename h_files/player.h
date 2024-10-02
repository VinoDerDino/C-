#ifndef PLAYER_H_
#define PLAYER_H_

#include <SDL2/SDL.h>
#include "map.h"

/********************************************************************************/
/*                                                                              */
/*                               Playerstats                                    */
/*                                                                              */
/********************************************************************************/

#define MOVE_SPEED 2

/********************************************************************************/
/*                                                                              */
/*                                 Structs                                      */
/*                                                                              */
/********************************************************************************/

typedef enum
{
    idle,
    fighting
} PlayerStates;

typedef struct
{
    int x, y;
    int curr_scene;
    PlayerStates states;
} Player;

typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_Texture *tile_textures[TILE_TEXTURE_COUNT];
    SDL_Texture *player_textures[PLAYER_TEXTURE_COUNT];

    Scene scenes[SCENE_COUNT];
    Player player;
} GameState;

/********************************************************************************/
/*                                                                              */
/*                                Functions                                     */
/*                                                                              */
/********************************************************************************/

void player_move(const uint8_t *keystate, PlayerStates curr_state, GameState *gameState);
void player_attack(const uint8_t *keystate, PlayerStates curr_state, GameState *gameState);
void player_change_state(PlayerStates new_state, GameState *gameState);

#endif