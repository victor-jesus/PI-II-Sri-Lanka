#ifndef GAME_H
#define GAME_H

#include <allegro5/allegro_font.h>
#include <stdbool.h>

#include "player.h"
#include "enemy.h"
#include "battle.h"
#include "map.h"

typedef enum{
    GAME_MENU,
    GAME_INIT,
    GAME_EXPLORING,
    GAME_BATTLE,
    GAME_OVER
} Game_state;

typedef struct{
    int x, y;
    bool left;
} Mouse;

typedef enum{
    BTN_INIT,
    BTN_OPTIONS,
    BTN_EXIT,
    NONE
} Btn_state;

typedef struct{
    int x, y, w, h;

} Button;

typedef struct {
    ALLEGRO_BITMAP* menu_background;
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
} Game;

Game* create_game(Game_state state, ALLEGRO_FONT* font, ALLEGRO_FONT* title_font, ALLEGRO_FONT* subtitle_font, int pos_x_player, int pos_y_player, int vx_player, int hp_player);
void update_game(Game* game, unsigned char* key, ALLEGRO_EVENT event, ALLEGRO_TIMER* timer_enemy, float dt);
void draw_menu(Game* game);
void draw_game(Game* game);
void destroy_game(Game* game);

#endif