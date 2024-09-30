#include "player.h"

void player_move(const uint8_t keystate, PlayerStates curr_state, State state)
{
    if (curr_state != idle)
    {
        return;
    }

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
}

void player_attack(const uint8_t keystate, PlayerStates curr_state)
{
    if (curr_state != fighting)
    {
        return;
    }
}