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
    Dialogues dialogues;
    Battle_state state;
    Turn_choice turn_choice;
    Turn_state turn_state;
    ALLEGRO_FONT* battle_font;
    ALLEGRO_TIMER* timer_dialogue;
    ALLEGRO_TIMER* timer_enemy;
    ALLEGRO_TIMER* timer_end;
    char log_ln1[256];
    char log_ln2[256];
    ALLEGRO_TIMER* log_timer;
} Battle;

void start_battle(Battle* battle, Player* player, Enemy* enemy);
void manage_battle(Battle* battle, ALLEGRO_EVENT event, ALLEGRO_FONT* font);
void destroy_battle(Battle* battle);

#endif