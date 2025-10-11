#include "game.h"
#include "entity.h"
#include "battle.h"
#include "screen.h"
#include "allegro5/allegro5.h"
#include "allegro5/allegro_primitives.h"
#include <stdio.h>
#include <stdlib.h> 

/*
    Animações do player, como são unicas decidi coloca-las aqui de maneira manual
*/
const char* path_idle = "assets/sprites/warrior_2/IDLE.png";
const char* path_run = "assets/sprites/warrior_2/RUN.png";
const char* path_attack = "assets/sprites/warrior_2/ATTACK_1.png";
const char* path_hit = "assets/sprites/warrior_2/HURT.png";

// Mesma coisa para o enemy, por enquanto
const char* path_skeleton_idle = "assets/sprites/skeleton/idle.png";
const char* path_skeleton_run = "assets/sprites/skeleton/run.png";
const char* path_skeleton_attack = "assets/sprites/skeleton/attack.png";
const char* path_skeleton_hit = "assets/sprites/skeleton/hit.png";

const char* path_door = "assets/sprites/enviroment/door_01.png";

Game* create_game(Game_state state, ALLEGRO_FONT* font, ALLEGRO_FONT* title_font, ALLEGRO_FONT* subtitle_font, int pos_x_player, int pos_y_player, int vx_player, int hp_player){
    Game* game = malloc(sizeof(Game));
    game->state = state;
    game->player = malloc(sizeof(Player));    
    game->enemy = malloc(sizeof(Enemy));
    game->map = malloc(sizeof(Map));
    game->num_world_entities = 0;

    for(int y = 0; y < MAP_HEIGHT; y++){
        for(int x = 0; x < MAP_WIDTH; x++){
            if (y == 0 || y == MAP_HEIGHT - 1 || x == 0 || x == MAP_WIDTH - 1)
                game->map->tiles[y][x] = 1;
            else
                game->map->tiles[y][x] = 0;

            if(x == 5 && y == 5){
                game->map->tiles[y][x] = 1;
            }
            if(x == 6 && y == 6){
                game->map->tiles[y][x] = 1;
            }
            if(x == 6 && y == 5){
                game->map->tiles[y][x] = 1;
            }
            if(x == 5 && y == 6){
                game->map->tiles[y][x] = 1;
            }
        }
    }

    Entity* door = malloc(sizeof(Entity));
    init_entity(door, 1500, (SCREEN_H / 2) + 25, 0, 0, 1);
    set_entity_anim(door, path_door, ANIM_IDLE, 1, 1, 0.1f);
    set_entity_scale(door, 0.5);
    set_hit_box(door, 0, 0, 0, 0);

    add_world_entity(game, door); 

    init_player(game->player, 100, pos_x_player, pos_y_player, vx_player, 5, 20, 20, 30, 30);

    set_entity_anim(&game->player->entity, path_idle, ANIM_IDLE, 7, 1, 0.1f);
    set_entity_anim(&game->player->entity, path_run, ANIM_RUN, 8, 1, 0.06f);
    set_entity_anim(&game->player->entity, path_attack, ANIM_ATTACK, 6, 1, 0.1f);
    set_entity_anim(&game->player->entity, path_hit, ANIM_HIT, 4, 1, 0.1f);
    set_entity_scale(&game->player->entity, 2.5);

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

    game->menu_background = al_load_bitmap("assets/Menu_Design.png");

    al_identity_transform(&game->camera_transform);
   
    return game;
}

void add_world_entity(Game* game, Entity* entity) {
    if (game->num_world_entities < MAX_WORLD_ENTITIES) {
        game->world_entities[game->num_world_entities] = entity;
        game->num_world_entities++;
    } else {
        // Opcional: imprimir um aviso ou liberar a memória da entidade não adicionada
        fprintf(stderr, "Aviso: Nao foi possivel adicionar mais entidades. Limite atingido.\n");
    }
}

void read_mouse(Game* game){
    ALLEGRO_MOUSE_STATE state;
    al_get_mouse_state(&state);

    game->mouse.x = state.x;
    game->mouse.y = state.y;

    game->mouse.left = state.buttons & 1;
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

void menu_options(Game* game){
   Btn_state btn_state = is_mouse_in_btn(game);

    if (game->mouse.left && btn_state == BTN_INIT)
    {
        game->state = GAME_EXPLORING;
    } else if(game->mouse.left && btn_state == BTN_EXIT){
        game->state = GAME_OVER;
    }
}

void update_camera(Game* game) {
    float player_x = game->player->entity.x;
    float player_y = game->player->entity.y;

    float screen_half_w = SCREEN_W / 2.0f;
    float screen_half_h = SCREEN_H / 2.0f;

    float camera_x = player_x - screen_half_w + 200;
    float camera_y = player_y - screen_half_h;

    float map_max_x = (float)(MAP_WIDTH * TILE_W) - SCREEN_W;
    float map_max_y = (float)(MAP_HEIGHT * TILE_H) - SCREEN_H;

    if (MAP_WIDTH * TILE_W > SCREEN_W) {
        if (camera_x < 0) camera_x = 0;
        if (camera_x > map_max_x) camera_x = map_max_x;
    } else {
        camera_x = -((SCREEN_W - (float)(MAP_WIDTH * TILE_W)) / 2.0f);
    }
    
    if (MAP_HEIGHT * TILE_H > SCREEN_H) {
        if (camera_y < 0) camera_y = 0;
        if (camera_y > map_max_y) camera_y = map_max_y;
    } else {
        camera_y = -((SCREEN_H - (float)(MAP_HEIGHT * TILE_H)) / 2.0f);
    }

    al_identity_transform(&game->camera_transform);

    al_translate_transform(&game->camera_transform, -camera_x, -camera_y);
}

void update_game(Game* game, unsigned char* key, ALLEGRO_EVENT event, ALLEGRO_TIMER* timer_enemy, float dt){
    read_mouse(game);   
    
    if(game->state == GAME_MENU) menu_options(game);

    check_battle(game);

    if(game->state == GAME_EXPLORING || game->state == GAME_BATTLE){
        update_camera(game); 
    }

    if(game->state == GAME_EXPLORING){
        update_player(game->player, key, dt);

        for(int i = 0; i < game->num_world_entities; i++){
            Entity* current_entity = game->world_entities[i];
            if(current_entity && current_entity->isActive){
                update_entity(current_entity, dt);
            }
        }

        if(game->enemy->entity.isActive)
        // update_enemy(&game->enemy->entity, dt);
        update_entity(&game->enemy->entity, dt);
        return;
    } 

    if(game->state == GAME_BATTLE && game->battle){
        if(game->battle->state == BATTLE_END) {
            game->state = GAME_EXPLORING;
            game->battle = NULL; 
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

void create_button(Button* btn, int x, int y, int w, int h){
    btn->x = x - (w / 2);
    btn->y = y;
    btn->w = w;
    btn->h = h;
}
    
void draw_menu(Game* game){
    int btn_w = 200;
    int btn_h = 50;
    int gap = 20;

    int btn1x = SCREEN_W / 2;
    int btn1y = (SCREEN_H / 2) + gap;
    int btn2x = SCREEN_W / 2;
    int btn2y = btn1y + btn_h + gap;

    int btn3x = SCREEN_W /2;
    int btn3y = btn2y + btn_h + gap;

    create_button(&game->btn_init, btn1x, btn1y, btn_w, btn_h);
    create_button(&game->btn_options, btn2x, btn2y, btn_w, btn_h);
    create_button(&game->btn_exit, btn3x, btn3y, btn_w, btn_h);

    al_draw_scaled_bitmap(
        game->menu_background,
        0, 0, 1300, 736,
        0, 0, SCREEN_W, SCREEN_H,    
        0
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

void draw_map(Map *map) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            float draw_x = x * TILE_W;
            float draw_y = y * TILE_H;

            if (map->tiles[y][x] == 1) {
                al_draw_filled_rectangle(draw_x, draw_y, 
                                         draw_x + TILE_W, draw_y + TILE_H,
                                         al_map_rgb(255, 255, 255));
            } else {
                al_draw_filled_rectangle(draw_x, draw_y, 
                                         draw_x + TILE_W, draw_y + TILE_H,
                                         al_map_rgb(50, 100, 50));
            }
                al_draw_rectangle(draw_x, draw_y, 
                                draw_x + TILE_W, draw_y + TILE_H,
                                al_map_rgb(0, 0, 0), 1);

        }
    }
}


void draw_game(Game* game){
    ALLEGRO_TRANSFORM identity_transform;

    switch(game->state){
        case GAME_MENU:
            draw_menu(game);
            break;

        case GAME_EXPLORING:
            

        case GAME_BATTLE:
            al_use_transform(&game->camera_transform);

            draw_map(game->map);

            for(int i = 0; i < game->num_world_entities; i++){
                Entity* current_entity = game->world_entities[i];
                if(current_entity && current_entity->isActive){
                    draw_entity(current_entity);
                }
            }
            
            draw_entity(&game->player->entity);

            if(game->enemy->entity.isActive){
                draw_entity(&game->enemy->entity);
            }

            al_identity_transform(&identity_transform);
            al_use_transform(&identity_transform);
            
            if(game->state == GAME_BATTLE){
                al_draw_text(game->game_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, 50, ALLEGRO_ALIGN_CENTER, "BATALHA!");

                if(game->battle->turn_state == TURN_PLAYER)
                    al_draw_text(game->game_font, al_map_rgb(255, 255, 0), SCREEN_W / 2, 150, ALLEGRO_ALIGN_CENTER, "Turno do player");
                else 
                    al_draw_text(game->game_font, al_map_rgb(255, 255, 0), SCREEN_W / 2, 150, ALLEGRO_ALIGN_CENTER, "Turno do inimigo");

                al_draw_text(game->game_font, al_map_rgb(255, 255, 0), 40, 50, ALLEGRO_ALIGN_LEFT, "Player");
                al_draw_textf(game->game_font, al_map_rgb(255, 255, 0), 40, 70, ALLEGRO_ALIGN_LEFT, "HP: %d", game->player->entity.hp);

                al_draw_text(game->game_font, al_map_rgb(255, 255, 0), SCREEN_W - 40, 50, ALLEGRO_ALIGN_RIGHT, "Enemy");
                al_draw_textf(game->game_font, al_map_rgb(255, 255, 0), SCREEN_W - 40, 70, ALLEGRO_ALIGN_RIGHT, "HP: %d", game->enemy->entity.hp);
            }
            break;
            
        case GAME_OVER:

            break;
        case GAME_INIT:

            break;
    }
}

void destroy_game(Game* game) {
    if (!game) return;

    for(int i = 0; i < game->num_world_entities; i++){
        if (game->world_entities[i]) free(game->world_entities[i]); 
    }

    if (game->battle) {
        destroy_battle(game->battle);
        game->battle = NULL;
    }

    if (game->enemy) {
        destroy_enemy(game->enemy);
        game->enemy = NULL;
    }

    if (game->player) {
        destroy_player(game->player);
        game->player = NULL;
    }

    if (game->map) {
        free(game->map);
        game->map = NULL;
    }

    if (game->menu_background) {
        al_destroy_bitmap(game->menu_background);
        game->menu_background = NULL;
    }
    free(game);
}
