#ifndef BATTLE_H
#define BATTLE_H

#include "battle_states.h"
#include "turn.h"
#include "player.h"
#include "enemy.h"
#include <allegro5/allegro5.h>

typedef struct{
    Player* player;
    Enemy* enemy;
    Battle_state state;
    Turn_choice turn_choice;
    Turn_state turn_state;
} Battle;

Battle* start_battle(Player* player, Enemy* enemy);
void manage_battle(Battle* battle, ALLEGRO_EVENT event, ALLEGRO_TIMER* timer);
void destroy_battle(Battle* battle);

#endif