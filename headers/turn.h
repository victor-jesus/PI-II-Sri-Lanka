#ifndef TURN_STATE_H
#define TURN_STATE_H

typedef enum{
    TURN_PLAYER,
    TURN_ENEMY,
    TURN_EMPTY
} Turn_state;

typedef enum{
    TURN_ATTACK,
    TURN_BIG_POTION,
    TURN_SMALL_POTION,
    TURN_WATER,
    TURN_DEFEND,
    TURN_NONE
} Turn_choice;
#endif