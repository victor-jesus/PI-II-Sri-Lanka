#ifndef BATTLE_H
#define BATTLE_H

#include "battle_states.h"
#include "turn.h"
#include "player.h"
#include "enemy.h"
#include <allegro5/allegro5.h>
#include "allegro5/allegro_font.h"

typedef struct Battle{
    Player* player;
    Enemy* enemy;
    Battle_state state;
    Turn_choice turn_choice;
    Turn_state turn_state;
    ALLEGRO_TIMER* timer_dialogue;
    ALLEGRO_TIMER* timer_end;
} Battle;

void start_battle(Battle* battle, Player* player, Enemy* enemy);
void manage_battle(Battle* battle, ALLEGRO_EVENT event, ALLEGRO_TIMER* timer, ALLEGRO_FONT* font);
void destroy_battle(Battle* battle);

#endif