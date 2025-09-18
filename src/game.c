#include "game.h"
#include "battle.h"
#include "screen.h"
#include "allegro5/allegro5.h"

Game* create_game(Game_state state, ALLEGRO_FONT* font, int pos_x_player, int pos_y_player, int vx_player, int hp_player){
    Game* game = malloc(sizeof(Game));
    game->state = state;
    game->player = malloc(sizeof(Player));
    init_player(game->player, pos_x_player, pos_y_player, vx_player, hp_player);

    game->enemy = malloc(sizeof(Enemy));
    init_enemy(game->enemy, 600, 720/2, 5, 100);

    game->battle = NULL;

    game->game_font = font;

    return game;
}

void check_battle(Game* game){
    if(!game->enemy->entity.isActive) return;

    int dist = game->player->entity.x - game->enemy->entity.x;

    if(dist >= -300){
        game->state = GAME_BATTLE;
        game->battle = start_battle(game->player, game->enemy);
    }
}

void update_game(Game* game, const char* key, ALLEGRO_EVENT event, ALLEGRO_TIMER* timer_enemy){
    check_battle(game);

    if(game->state == GAME_EXPLORING){
        update_player(game->player, key);
    }

    if(game->state == GAME_BATTLE){
        if(game->battle->state == BATTLE_END) {
            game->state = GAME_EXPLORING;
            destroy_battle(game->battle);
            return;
        }

        manage_battle(game->battle, event, timer_enemy);

    }
}

void draw_game(Game* game){
    if(game->state == GAME_BATTLE){
        al_draw_text(game->game_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, 50, ALLEGRO_ALIGN_CENTER, "BATALHA!");

        if(game->battle->turn_state == TURN_PLAYER)
            al_draw_text(game->game_font, al_map_rgb(255, 255, 0), SCREEN_W / 2, 150, ALLEGRO_ALIGN_CENTER, "Turno do player");
        else 
            al_draw_text(game->game_font, al_map_rgb(255, 255, 0), SCREEN_W / 2, 150, ALLEGRO_ALIGN_CENTER, "Turno do inimigo");

        al_draw_text(game->game_font, al_map_rgb(255, 255, 0), 40, (SCREEN_H / 2) - 300, ALLEGRO_ALIGN_LEFT, "Player");
        al_draw_textf(game->game_font, al_map_rgb(255, 255, 0), 40, (SCREEN_H / 2) - 290, ALLEGRO_ALIGN_LEFT, "HP: %d", game->player->entity.hp);

        al_draw_text(game->game_font, al_map_rgb(255, 255, 0), SCREEN_W - 40, SCREEN_H / 2 - 300, ALLEGRO_ALIGN_RIGHT, "Enemy");
        al_draw_textf(game->game_font, al_map_rgb(255, 255, 0), SCREEN_W - 40, SCREEN_H / 2 - 290, ALLEGRO_ALIGN_RIGHT, "HP: %d", game->enemy->entity.hp);
    }

    al_draw_text(game->game_font, al_map_rgb(255, 255, 0), game->player->entity.x, game->player->entity.y - 30, ALLEGRO_ALIGN_CENTER, "Player");
    draw_entity(&game->player->entity);

    if(game->enemy->entity.isActive){
        al_draw_text(game->game_font, al_map_rgb(255, 255, 0), game->enemy->entity.x, game->enemy->entity.y - 30, ALLEGRO_ALIGN_CENTER, "Enemy");
        draw_entity(&game->enemy->entity);
    }
}