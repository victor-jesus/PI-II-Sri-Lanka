#include "game.h"
#include "battle.h"
#include "screen.h"
#include "allegro5/allegro5.h"

/*
    Animações do player, como são unicas decidi coloca-las aqui de maneira manual
*/
const char* path_idle = "assets/sprites/samurai/idle.png";
const char* path_run = "assets/sprites/samurai/run.png";
const char* path_attack = "assets/sprites/samurai/attack.png";
const char* path_hit = "assets/sprites/samurai/hurt.png";

Game* create_game(Game_state state, ALLEGRO_FONT* font, int pos_x_player, int pos_y_player, int vx_player, int hp_player){
    Game* game = malloc(sizeof(Game));
    game->state = state;
    game->player = malloc(sizeof(Player));    
    game->enemy = malloc(sizeof(Enemy));

    init_player(game->player, 100, pos_x_player, pos_y_player, vx_player, 5, 0, 0, 0, 0);

    set_player_sprite(game->player, path_idle, ANIM_IDLE, 10, 1, 0.1f);
    set_player_sprite(game->player, path_run, ANIM_RUN, 16, 1, 0.06f);
    set_player_sprite(game->player, path_attack, ANIM_ATTACK, 7, 1, 0.1f);
    set_player_sprite(game->player, path_hit, ANIM_HIT, 4, 1, 0.1f);

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

void update_game(Game* game, unsigned char* key, ALLEGRO_EVENT event, ALLEGRO_TIMER* timer_enemy, float dt){
    check_battle(game);

    if(game->state == GAME_EXPLORING){
        update_player(game->player, key, dt);
    }

    if(game->state == GAME_BATTLE && game->battle){
        if(game->battle->state == BATTLE_END) {
            game->state = GAME_EXPLORING;
            game->battle = NULL;
            destroy_battle(game->battle);
            return;
        }

        manage_battle(game->battle, event, timer_enemy);

    }
}

void draw_game(Game* game){
    if(game->state == GAME_BATTLE && game->battle){
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