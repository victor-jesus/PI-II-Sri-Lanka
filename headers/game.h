#ifndef GAME_H
#define GAME_H

#include <allegro5/allegro_font.h>
#include <stdbool.h>

#define SCREEN_W 1280
#define SCREEN_H 720
#define MAX_WORLD_ENTITIES 100

#include "player.h"
#include "enemy.h"
#include "battle.h"
#include "map.h"

typedef enum{
    GAME_MENU,
    GAME_INIT,
    GAME_FIRST_MISSION,
    GAME_SECOND_MISSION,
    GAME_BATTLE,
    GAME_OVER
} Game_state;

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
    GAMEPLAY_EXPLORING,
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
    Enemy* enemy;
    Battle* battle;
    Mouse mouse;
    Button btn_init, btn_options, btn_exit;
    Map* map;
    ALLEGRO_FONT* game_font;
    ALLEGRO_FONT* title_font;
    ALLEGRO_FONT* subtitle_font;
    ALLEGRO_FONT* subtitle_8_font;
    ALLEGRO_TRANSFORM camera_transform; 

    Init_dialogues init_dialogues;
    Gameplay_state gameplay_state;
    ALLEGRO_BITMAP* controls;
    Entity* world_entities[MAX_WORLD_ENTITIES];
    int num_world_entities;
    ALLEGRO_BITMAP* background;

} Game;


void add_world_entity(Game* game, Entity* entity);
Game* create_game(Game_state state, ALLEGRO_FONT* font, ALLEGRO_FONT* title_font, ALLEGRO_FONT* subtitle_font, int pos_x_player, int pos_y_player, int vx_player, int hp_player);
void update_game(Game* game, unsigned char* key, ALLEGRO_EVENT event, ALLEGRO_TIMER* timer_enemy, float dt);
void draw_map(Map *map);
void draw_menu(Game* game);
void draw_game(Game* game);
void destroy_game(Game* game);

#endif