#ifndef GAME_H
#define GAME_H

#include <allegro5/allegro_font.h>
#include <stdbool.h>

#define MAX_WORLD_ENTITIES 100

#include "enemies.h"
#include "player.h"
#include "enemy.h"
#include "battle.h"
#include "map.h"
#include "door.h"
#include "game_state.h"

typedef enum{
    DIALOGUE_1,
    DIALOGUE_2,
    DIALOGUE_3,
    DIALOGUE_4,
    DIALOGUE_5,
    NEXT_LEVEL,
    DIALOGUE_NONE
} Init_dialogues;

typedef enum{
    GAMEPLAY_BATTLE,
    GAMEPLAY_PAUSE,
    GAMEPLAY_EXPLORING,
    GAMEPLAY_PUZZLE,
    GAMEPLAY_NONE
} Gameplay_state;

typedef struct{
    int x, y;
    bool left;
} Mouse;

typedef enum{
    BTN_INIT,
    BTN_OPTIONS,
    BTN_EXIT,
    BTN_NONE
} Btn_state;

typedef struct{
    int x, y, w, h;
} Button;

typedef struct {
    Game_state state;
    Player* player;
    Enemy* enemy; // BOSSES
    Enemy* mobs[MAX_ENEMIES]; // MOBS
    int world_enemies;
    Battle* battle;
    Mouse mouse;
    Button btn_init, btn_options, btn_exit;
    Map* map;
    ALLEGRO_FONT* game_font;
    ALLEGRO_FONT* title_font;
    ALLEGRO_FONT* subtitle_font;
    ALLEGRO_FONT* subtitle_8_font;
    ALLEGRO_FONT* subtitle_11_font;
    ALLEGRO_TRANSFORM camera_transform; 
    
    Init_dialogues init_dialogues;
    Gameplay_state gameplay_state;
    ALLEGRO_BITMAP* controls; // BITMAP PARA TECLAS DO TECLADO 

    Entity* world_entities[MAX_WORLD_ENTITIES];
    int num_world_entities;
    ALLEGRO_BITMAP* background;
    ALLEGRO_EVENT_QUEUE* queue;
    ALLEGRO_EVENT* event;

    ALLEGRO_TIMER* timer_game_logs;
    ALLEGRO_TIMER* timer_game_tips;
    char log_ln1[256];
    char log_ln2[256];
    char log_ln3[256];
    char log_ln4[256];
    char log_ln5[256];
    char log_ln_tip_1[256];
    char log_ln_tip_2[256];
    char log_ln_tip_3[256];

    Game_state previous_game_state;
    Gameplay_state previous_gameplay_state;

    Puzzle_id active_puzzle_id;

    ALLEGRO_BITMAP* puzzle_bhaskara_img;
    ALLEGRO_BITMAP* puzzle_pythagoras_img;
    ALLEGRO_BITMAP* puzzle_bhaskara_paper;
    ALLEGRO_BITMAP* puzzle_bhaskara_paper_2;
    ALLEGRO_BITMAP* puzzle_pitagoras_paper_3;

    ALLEGRO_FONT* log_font;
    ALLEGRO_FONT* log_font_20;
    bool can_draw;

    bool is_minotaur_dead, is_medusa_dead, is_arauto_dead;
    bool is_first_level_mobs_dead, is_second_level_mobs_dead, is_third_level_mobs_dead;
} Game;


void add_world_entity(Game* game, Entity* entity);
Game* create_game(Game_state state, ALLEGRO_FONT* font, ALLEGRO_FONT* title_font, ALLEGRO_FONT* subtitle_font, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_EVENT* event, int pos_x_player, int pos_y_player, int vx_player, int hp_player);
void update_game(Game* game, unsigned char* key, ALLEGRO_EVENT event, float dt);
void draw_map(Map *map);
void draw_menu(Game* game);
void draw_game(Game* game);
void destroy_game(Game* game);

#endif