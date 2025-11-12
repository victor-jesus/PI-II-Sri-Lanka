#ifndef BATTLE_H
#define BATTLE_H

#include "battle_states.h"
#include "turn.h"
#include "player.h"
#include "enemy.h"
#include <allegro5/allegro5.h>
#include <stdbool.h>
#include "allegro5/allegro_font.h"
#include "game_state.h"

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
    char log_ln3[256];
    char log_ln4[256];
    char log_ln5[256];
    char log_ln6[256];
    char log_ln7[256];
    char log_ln8[256];
    char log_ln9[256];
    char log_ln10[256];
    ALLEGRO_TIMER* log_timer;
    ALLEGRO_BITMAP* dialogue_sprite;

    bool dropped_itens;
} Battle;

void start_battle(Battle* battle, Player* player, Enemy* enemy);
void manage_battle(Battle* battle, ALLEGRO_EVENT event, Game_state game_state, unsigned char* key, ALLEGRO_FONT* font, int* world_enemies);
void destroy_battle(Battle* battle);

#endif