#include "player.h"

void player_move(const uint8_t *keystate, PlayerStates curr_state, GameState *gamestate) {
    if(curr_state != idle) {
        printf("Nicht idle\n");
        return;
    }

    Scene *curr_scene = &gamestate->scenes[gamestate->player.curr_scene];

    if (keystate[SDLK_RIGHT & 0xFFFF]){
        gamestate->player.x += MOVE_SPEED;
        if(gamestate->player.x >= COLS * TILE_SIZE) {
            if(curr_scene->sectors[curr_scene->curr_sector].neighbors[1] != 0) {
                curr_scene->curr_sector = curr_scene->sectors[curr_scene->curr_sector].neighbors[1] - 1;
                gamestate->player.x = 0;
            } else {
            gamestate->player.x = COLS * TILE_SIZE - 1;
            }
        }
    }

    if (keystate[SDLK_LEFT & 0xFFFF]) {
        gamestate->player.x -= MOVE_SPEED;
        if(gamestate->player.x < 0) {
            if(curr_scene->sectors[curr_scene->curr_sector].neighbors[3] != 0) {
                curr_scene->curr_sector = curr_scene->sectors[curr_scene->curr_sector].neighbors[3] - 1;
                gamestate->player.x = COLS * TILE_SIZE - 1;
            } else {
            gamestate->player.x = 0;
            }
        }
    }

    if (keystate[SDLK_UP & 0xFFFF]) {
        gamestate->player.y -= MOVE_SPEED;
        if(gamestate->player.y < 0) {
            if(curr_scene->sectors[curr_scene->curr_sector].neighbors[0] != 0) {
                curr_scene->curr_sector = curr_scene->sectors[curr_scene->curr_sector].neighbors[0] - 1;
                gamestate->player.y = ROWS * TILE_SIZE - 1;
            } else {
            gamestate->player.y = 0;
            }
        }
    }

    if (keystate[SDLK_DOWN & 0xFFFF])  {
        gamestate->player.y += MOVE_SPEED;
        if(gamestate->player.y >= ROWS * TILE_SIZE) {
            if(curr_scene->sectors[curr_scene->curr_sector].neighbors[2] != 0) {
                curr_scene->curr_sector = curr_scene->sectors[curr_scene->curr_sector].neighbors[2] - 1;
                gamestate->player.y = 0;
            } else {
            gamestate->player.y = ROWS * TILE_SIZE - 1;
            }
        }
    }
}

void player_attack(const uint8_t *keystate, PlayerStates curr_state, GameState *gamestate) {
    if(curr_state != fighting) {
        return;
    }

    (void)keystate;
    (void)gamestate;
}

void player_change_state(PlayerStates new_state, GameState *gamestate) {
    (void)new_state;
    (void)gamestate;
}