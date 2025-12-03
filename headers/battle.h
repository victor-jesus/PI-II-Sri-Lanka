#ifndef BATTLE_H
#define BATTLE_H

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <stdbool.h>

#include "battle_states.h"
#include "turn.h"
#include "player.h"
#include "enemy.h"
#include "game_state.h"

typedef struct Battle{
    // Entidades
    Player* player;
    Enemy* enemy;
    
    // Estados e Controle
    Battle_state state;
    Dialogues dialogues;
    Turn_choice turn_choice;
    Turn_state turn_state;
    
    // Controle de Menu
    int opt_player;            // Opção selecionada no menu principal (1-4)
    bool is_selecting_item;    // Se true, desenha/controla o menu de itens
    int item_selection_index;  // Índice selecionado no submenu de itens

    // Flags de Ação (AQUI ESTÃO ELAS)
    bool can_act;       // Se o player pode agir no geral (ex: não está stunado)
    bool can_use_item;  // Se o player pode usar item neste turno
    bool is_defending;
    bool dropped_itens;

    // Recursos Visuais e Timers
    ALLEGRO_FONT* battle_font;
    ALLEGRO_TIMER* timer_dialogue;
    ALLEGRO_TIMER* timer_enemy;
    ALLEGRO_TIMER* timer_end;
    ALLEGRO_TIMER* log_timer;
    ALLEGRO_BITMAP* dialogue_sprite;
    ALLEGRO_TIMER* error_timer;

    // Logs de Batalha
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
    char log_damage[256];

} Battle;

void start_battle(Battle* battle, Player* player, Enemy* enemy);
void manage_battle(Battle* battle, ALLEGRO_EVENT event, ALLEGRO_TIMER* timer_death, Game_state game_state, unsigned char* key, ALLEGRO_FONT* font, int* world_enemies);
void destroy_battle(Battle* battle);

#endif