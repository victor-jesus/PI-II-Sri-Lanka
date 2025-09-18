#ifndef GAME_H
#define GAME_H

#include <allegro5/allegro_font.h>

#include "player.h"
#include "enemy.h"
#include "battle.h"

typedef enum{
    GAME_MENU,
    GAME_INIT,
    GAME_EXPLORING,
    GAME_BATTLE,
    GAME_OVER
} Game_state;

typedef struct {
    Game_state state;
    Player* player;
    Enemy* enemy;
    Battle* battle;
    ALLEGRO_FONT* game_font;
} Game;

Game* create_game(Game_state state, ALLEGRO_FONT* font, int pos_x_player, int pos_y_player, int vx_player, int hp_player);
void update_game(Game* game, const char* key, ALLEGRO_EVENT event, ALLEGRO_TIMER* timer_enemy);
void draw_game(Game* game);

#endif