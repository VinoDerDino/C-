#ifndef PLAYER_H_
#define PLAYER_H_

#include <SDL2/SDL.h>

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
    int state;
    int x, y;
    PlayerStates states;
} Player;

/********************************************************************************/
/*                                                                              */
/*                                Functions                                     */
/*                                                                              */
/********************************************************************************/

void player_move(const uint8_t keystate, PlayerStates curr_state, State state);
void player_attack(const uint8_t keystate, PlayerStates curr_state, State state);

#endif