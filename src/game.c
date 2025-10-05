#include "game.h"
#include "entity.h"
#include "battle.h"
#include "screen.h"
#include "allegro5/allegro5.h"
#include "allegro5/allegro_primitives.h"
#include <stdio.h>

/*
    Animações do player, como são unicas decidi coloca-las aqui de maneira manual
*/
const char* path_idle = "assets/sprites/samurai/idle.png";
const char* path_run = "assets/sprites/samurai/run.png";
const char* path_attack = "assets/sprites/samurai/attack.png";
const char* path_hit = "assets/sprites/samurai/hurt.png";

// Mesma coisa para o enemy, por enquanto
const char* path_skeleton_idle = "assets/sprites/skeleton/idle.png";
const char* path_skeleton_run = "assets/sprites/skeleton/run.png";
const char* path_skeleton_attack = "assets/sprites/skeleton/attack.png";
const char* path_skeleton_hit = "assets/sprites/skeleton/hit.png";


Game* create_game(Game_state state, ALLEGRO_FONT* font, ALLEGRO_FONT* title_font, ALLEGRO_FONT* subtitle_font, int pos_x_player, int pos_y_player, int vx_player, int hp_player){
    Game* game = malloc(sizeof(Game));
    game->state = state;
    game->player = malloc(sizeof(Player));    
    game->enemy = malloc(sizeof(Enemy));

    init_player(game->player, 100, pos_x_player, pos_y_player, vx_player, 5, 50, 14, 30, 30);

    set_entity_anim(&game->player->entity, path_idle, ANIM_IDLE, 10, 1, 0.1f);
    set_entity_anim(&game->player->entity, path_run, ANIM_RUN, 16, 1, 0.06f);
    set_entity_anim(&game->player->entity, path_attack, ANIM_ATTACK, 7, 1, 0.1f);
    set_entity_anim(&game->player->entity, path_hit, ANIM_HIT, 4, 1, 0.1f);

    init_enemy(game->enemy, 600, 720/2, 5, 100);
    set_entity_anim(&game->enemy->entity, path_skeleton_idle, ANIM_IDLE, 11, 1, 0.1f);
    set_entity_anim(&game->enemy->entity, path_skeleton_run, ANIM_RUN, 13, 1, 0.06f);
    set_entity_anim(&game->enemy->entity, path_skeleton_attack, ANIM_ATTACK, 18, 1, 0.1f);
    set_entity_anim(&game->enemy->entity, path_skeleton_hit, ANIM_HIT, 8, 1, 0.1f);
    game->enemy->entity.isActive = false;

    game->battle = NULL;

    game->game_font = font;
    game->title_font = title_font;
    game->subtitle_font = subtitle_font;
   


  

    return game;
}

void read_mouse(Game* game){
    ALLEGRO_MOUSE_STATE state;
    al_get_mouse_state(&state);

    game->mouse.x = state.x;
    game->mouse.y = state.y;

    game->mouse.left   = state.buttons & 1;
}

Btn_state is_mouse_in_btn(Game* game){
    if (game->mouse.x >= game->btn_init.x && game->mouse.x <= game->btn_init.x + game->btn_init.w &&
            game->mouse.y >= game->btn_init.y && game->mouse.y <= game->btn_init.y + game->btn_init.h){
        return BTN_INIT;
    }else if(game->mouse.x >= game->btn_options.x && game->mouse.x <= game->btn_options.x + game->btn_options.w &&
                game->mouse.y >= game->btn_options.y && game->mouse.y <= game->btn_options.y + game->btn_options.h){
        return BTN_OPTIONS;
    }else if(game->mouse.x >= game->btn_exit.x && game->mouse.x <= game->btn_exit.x + game->btn_exit.w &&
            game->mouse.y >= game->btn_exit.y && game->mouse.y <= game->btn_exit.y + game->btn_exit.h){
        return BTN_EXIT;
    }else{
        return NONE;
    }

    
} 

void check_battle(Game* game){
    if(!game->enemy->entity.isActive) return;
    if(game->state == GAME_BATTLE && game->battle) return;

    int dist = game->player->entity.x - game->enemy->entity.x;

    if(dist >= -300 && dist <= 300){
        if(dist < 0) game->enemy->entity.flip = ALLEGRO_FLIP_HORIZONTAL;
        game->state = GAME_BATTLE;
        game->battle = start_battle(game->player, game->enemy);
    }
}

void update_game(Game* game, unsigned char* key, ALLEGRO_EVENT event, ALLEGRO_TIMER* timer_enemy, float dt){
    read_mouse(game);
    //printf("mouse: %d, %d\n", game->mouse.x, game->mouse.y);

    Btn_state btn_state = is_mouse_in_btn(game);
    if (game->mouse.left && btn_state == BTN_INIT)
    {
        game->state = GAME_EXPLORING;
    }

    if(game->mouse.left && btn_state == BTN_EXIT){
        game->state = GAME_OVER;
    }
    
    



    check_battle(game);

    if(game->state == GAME_EXPLORING){
        update_player(game->player, key, dt);
        if(game->enemy->entity.isActive)
        // update_enemy(&game->enemy->entity, dt);
        update_entity(&game->enemy->entity, dt);
        return;
    } 

    if(game->state == GAME_BATTLE && game->battle){
        if(game->battle->state == BATTLE_END) {
            game->state = GAME_EXPLORING;
            game->battle = NULL;
            destroy_battle(game->battle);
            return;
        }
        
        if(game->enemy->entity.isActive){
            update_entity(&game->enemy->entity, dt);
            // update_enemy(&game->enemy->entity, dt);
        }
        manage_battle(game->battle, event, timer_enemy);
        update_player_battle(game->player, dt);
    }
}

void create_button(Game* game, int btn1x, int btn1y, int btn2x, int btn2y, int btn3x, int btn3y, int btn_h, int btn_w){
   
    game->btn_init.w = btn_w;
    game->btn_options.w = btn_w;
    game->btn_exit.w = btn_w;

    game->btn_init.h = btn_h;
    game->btn_options.h = btn_h;
    game->btn_exit.h = btn_h;

    game->btn_init.x = btn1x;
    game->btn_init.y = btn1y;

    game->btn_options.x = btn2x;
    game->btn_options.y = btn2y;

    game->btn_exit.x = btn3x;
    game->btn_exit.y = btn3y;

}


    
void draw_menu(Game* game){
    ALLEGRO_BITMAP* menu = al_load_bitmap("assets/Menu_Design.png");
    int btn_w = 200;
    int btn_h = 50;
    int gap = 20;

    int btn1x = SCREEN_W / 2;
    int btn1y = (SCREEN_H / 2) + gap;
    int btn2x = SCREEN_W / 2;
    int btn2y = btn1y + btn_h + gap;

    int btn3x = SCREEN_W /2;
    int btn3y = btn2y + btn_h + gap;

    create_button(game, btn1x, btn1y, btn2x, btn2y, btn3x, btn3y, btn_h, btn_w);

    al_draw_scaled_bitmap(
        menu,
        0, 0, 1312, 736, // origem e tamanho original
        0, 0, SCREEN_W, SCREEN_H,    // destino e tamanho novo
        0                   // flags
    );

    al_draw_text(game->title_font, al_map_rgb(255, 0, 0), (SCREEN_W / 2), (SCREEN_H / 2) - 250, ALLEGRO_ALIGN_CENTER, "CRÔNICAS DO CÁLCULO");
    al_draw_text(game->subtitle_font, al_map_rgb(255, 255, 0), (SCREEN_W / 2), (SCREEN_H / 2) - 210, ALLEGRO_ALIGN_CENTER, "O DESPERTAR DO ARAUTO");

    al_draw_rectangle(btn1x - (btn_w / 2), btn1y, btn1x + (btn_w / 2), btn1y + btn_h, al_map_rgb(255, 255, 0), 2);
    al_draw_text(game->subtitle_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, (btn1y + btn_h) - (btn_h / 2) - 10, ALLEGRO_ALIGN_CENTER, "INICIAR");

    al_draw_rectangle(btn2x - (btn_w / 2), btn2y, btn2x + (btn_w / 2), btn2y + btn_h, al_map_rgb(255, 255, 0), 2);
    al_draw_text(game->subtitle_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, (btn2y + btn_h) - (btn_h / 2) - 10, ALLEGRO_ALIGN_CENTER, "OPÇÕES");

    al_draw_rectangle(btn3x - (btn_w / 2), btn3y, btn3x + (btn_w / 2), btn3y + btn_h, al_map_rgb(255, 255, 0), 2);
    al_draw_text(game->subtitle_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, (btn3y + btn_h) - (btn_h / 2) - 10, ALLEGRO_ALIGN_CENTER, "SAIR");

}

void draw_game(Game* game){

    if(game->state == GAME_MENU){
         draw_menu(game);
         return;
    }

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

    al_draw_text(game->game_font, al_map_rgb(255, 255, 0), game->player->entity.box.x + game->player->entity.box.w, game->player->entity.box.y, ALLEGRO_ALIGN_CENTER, "Player");
    draw_entity(&game->player->entity);

    if(game->enemy->entity.isActive){
        al_draw_text(game->game_font, al_map_rgb(255, 255, 0), game->enemy->entity.x, game->enemy->entity.y - 30, ALLEGRO_ALIGN_CENTER, "Enemy");
        draw_entity(&game->enemy->entity);
    }
}