#include "game.h"
#include "entity.h"
#include "battle.h"
#include "screen.h"
#include "allegro5/allegro5.h"
#include "allegro5/allegro_primitives.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "key_types.h"
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

const char* path_map_tile = "assets/sprites/map/background-wall.png";
const char* path_map_tile_floor = "assets/sprites/map/chao-4.png";

const char* path_banner = "assets/sprites/map/banners.png";
const char* path_banner_e = "assets/sprites/enviroment/banner-e.png";
const char* path_banner_pi = "assets/sprites/enviroment/banner-pi.png";
const char* path_door = "assets/sprites/enviroment/door_01.png";
const char* path_torch = "assets/sprites/map/torch.png";
const char* path_window_1 = "assets/sprites/map/window_1.png";
const char* path_window_big = "assets/sprites/map/big_window.png";
const char* path_bau = "assets/sprites/enviroment/bau.png";

const char* path_initial_background = "assets/background_level_01.jpg";
const char* path_dialogue_box = "assets/sprites/ui/gui/dialogue_box.png";

const char* path_subtitle_8_font = "assets/fonts/pressStart2p.ttf";

const char* path_key_e = "assets/sprites/ui/controls/KEYBOARD/KEYS/E.png";


#define TILE_FLOOR 1
#define TILE_WALL 0

Game* create_game(Game_state state, ALLEGRO_FONT* font, ALLEGRO_FONT* title_font, ALLEGRO_FONT* subtitle_font, int pos_x_player, int pos_y_player, int vx_player, int hp_player){
    Game* game = malloc(sizeof(Game));
    game->state = state;
    game->gameplay_state = GAMEPLAY_NONE;
    game->player = malloc(sizeof(Player));    
    game->enemy = malloc(sizeof(Enemy));
    game->map = malloc(sizeof(Map));
    game->num_world_entities = 0;
    game->init_dialogues = DIALOGUE_NONE;

    Entity* key_e = malloc(sizeof(Entity));
    init_entity(key_e, 0, 0, 0, 0, 1, KEY);
    set_entity_anim(key_e, path_key_e, ANIM_IDLE, 1, 1, 0.1f);
    set_entity_scale(key_e, 1);
    set_hit_box(key_e, 0, 0, 0, 0);

    init_player(game->player, 100, pos_x_player, (SCREEN_H / 2) + 140, vx_player, 5, 20, 20, 30, 30);

    set_entity_anim(&game->player->entity, path_idle, ANIM_IDLE, 7, 1, 0.12f);
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
    game->subtitle_8_font = al_load_ttf_font(path_subtitle_8_font, 8, 0);

    game->background = al_load_bitmap("assets/Menu_Design.png");

    game->controls = al_load_bitmap(path_key_e);


    al_identity_transform(&game->camera_transform);
   
    return game;
}

void render_initial_level(Game* game){
    if (game->state != GAME_INIT) {
        if (game->background) {
            al_destroy_bitmap(game->background);
            game->background = NULL;
        }

        game->state = GAME_INIT;
        game->background = al_load_bitmap(path_initial_background);
        if (!game->background) {
            printf("ERRO: Nao foi possivel carregar background_level_01.jpg\n");
        }
    }

    Entity* dialogue = malloc(sizeof(Entity));
    init_entity(dialogue, 0, 0, 0, 0, 1, DIALOGUE);
    set_entity_anim(dialogue, path_dialogue_box, ANIM_IDLE, 1, 1, 0.1f);
    set_entity_scale(dialogue, 1.5);
    set_hit_box(dialogue, 0, 0, 0, 0);
    set_entity_pos(dialogue, SCREEN_W - (dialogue->box.w) - 20, 10);

    add_world_entity(game, dialogue);

    game->init_dialogues = DIALOGUE_1;
}

void render_first_map(Game* game){
    game->state = GAME_FIRST_MISSION;
    game->gameplay_state = GAMEPLAY_EXPLORING;

    for(int y = 0; y < MAP_HEIGHT; y++){
        for(int x = 0; x < MAP_WIDTH; x++){
            if(y == MAP_HEIGHT - 1)
                game->map->tiles[y][x] = TILE_FLOOR;
            else
                game->map->tiles[y][x] = TILE_WALL;
        }
    }

    init_map(game->map, path_map_tile, path_map_tile_floor); 

    Entity* door = malloc(sizeof(Entity));
    init_entity(door, 1500, (SCREEN_H / 2) + 125, 0, 0, 1, DOOR);
    set_entity_anim(door, path_door, ANIM_IDLE, 1, 1, 0.1f);
    set_entity_scale(door, 0.5);
    set_hit_box(door, 0, 0, 0, 0);

    Entity* banner = malloc(sizeof(Entity));
    init_entity(banner, 1425, (SCREEN_H / 2) + 200, 0, 0, 1, ENVIRONMENT_NO_MOVE);
    set_entity_anim(banner, path_banner, ANIM_IDLE, 6, 1, 0.1f);
    set_entity_scale(banner, 0.5);
    set_hit_box(banner, 0, 0, 0, 0);
    set_entity_pos(banner, door->x + (door->box.w / 2) - (banner->box.w / 2) + 100, door->y + door->box.w + 10);

    Entity* banner2 = malloc(sizeof(Entity));
    init_entity(banner2, 0, 0, 0, 0, 1, ENVIRONMENT_NO_MOVE);
    set_entity_anim(banner2, path_banner, ANIM_IDLE, 6, 1, 0.1f);
    set_entity_scale(banner2, 0.5);
    set_hit_box(banner2, 0, 0, 0, 0);
    set_entity_pos(banner2, door->x + (door->box.w / 2) - (banner2->box.w / 2) - 100, door->y + door->box.w + 10);


    Entity* torch = malloc(sizeof(Entity));
    init_entity(torch, 0, 0, 0, 0, 1, ENVIRONMENT_MOVE);
    set_entity_anim(torch, path_torch, ANIM_IDLE, 4, 2, 0.2f);
    set_entity_scale(torch, 1.2);
    set_hit_box(torch, 0, 0, 0, 0);
    set_entity_pos(torch, door->x + (door->box.w / 2) - (torch->box.w / 2) - 100, door->y);

    Entity* torch2 = malloc(sizeof(Entity));
    init_entity(torch2, 0, 0, 0, 0, 1, ENVIRONMENT_MOVE);
    set_entity_anim(torch2, path_torch, ANIM_IDLE, 4, 2, 0.2f);
    set_entity_scale(torch2, 1.2);
    set_hit_box(torch2, 0, 0, 0, 0);
    set_entity_pos(torch2, door->x + (door->box.w / 2) - (torch2->box.w / 2) + 100, door->y);

    Entity* window_1 = malloc(sizeof(Entity));
    init_entity(window_1, 1000, (SCREEN_H / 2), 0, 0, 1, ENVIRONMENT_NO_MOVE);
    set_entity_anim(window_1, path_window_1, ANIM_IDLE, 1, 1, 0.1f);
    set_entity_scale(window_1, 2);
    set_hit_box(window_1, 0, 0, 0, 0);

    Entity* window_big = malloc(sizeof(Entity));
    init_entity(window_big, 700, 80, 0, 0, 1, ENVIRONMENT_NO_MOVE);
    set_entity_anim(window_big, path_window_big, ANIM_IDLE, 1, 1, 0.1f);
    set_entity_scale(window_big, 1.2);
    set_hit_box(window_big, 0, 0, 0, 0);

    Entity* window_2 = malloc(sizeof(Entity));
    init_entity(window_2, 2000, (SCREEN_H / 2), 0, 0, 1, ENVIRONMENT_NO_MOVE);
    set_entity_anim(window_2, path_window_1, ANIM_IDLE, 1, 1, 0.1f);
    set_entity_scale(window_2, 2);
    set_hit_box(window_2, 0, 0, 0, 0);

    Entity* bau = malloc(sizeof(Entity));
    init_entity(bau, 400, (SCREEN_H / 2) + 180, 0, 0, 1, ENVIRONMENT_NO_MOVE);
    set_entity_anim(bau, path_bau, ANIM_IDLE, 1, 1, 0.1f);
    set_entity_scale(bau, 1.2);
    set_hit_box(bau, 0, 0, 0, 0);

    add_world_entity(game, torch);
    add_world_entity(game, bau);
    add_world_entity(game, window_big);
    add_world_entity(game, window_1);
    add_world_entity(game, window_2);
    add_world_entity(game, torch2);
    add_world_entity(game, banner);
    add_world_entity(game, banner2);
    add_world_entity(game, door); 

}

void render_second_map(Game* game){
    reset_world_entities(game);
    game->state = GAME_SECOND_MISSION;
    game->gameplay_state = GAMEPLAY_EXPLORING;

    for(int y = 0; y < MAP_HEIGHT; y++){
        for(int x = 0; x < MAP_WIDTH; x++){
            if(y == MAP_HEIGHT - 1)
                game->map->tiles[y][x] = TILE_FLOOR;
            else
                game->map->tiles[y][x] = TILE_WALL;
        }
    }

    init_map(game->map, path_map_tile, path_map_tile_floor); 

    Entity* door = malloc(sizeof(Entity));
    init_entity(door, 1500, (SCREEN_H / 2) + 125, 0, 0, 1, DOOR);
    set_entity_anim(door, path_door, ANIM_IDLE, 1, 1, 0.1f);
    set_entity_scale(door, 0.5);
    set_hit_box(door, 0, 0, 0, 0);

    Entity* banner = malloc(sizeof(Entity));
    init_entity(banner, 0, 0, 0, 0, 1, ENVIRONMENT_NO_MOVE);
    set_entity_anim(banner, path_banner_e, ANIM_IDLE, 1, 1, 0.1f);
    set_entity_scale(banner, 0.2);
    set_hit_box(banner, 0, 0, 0, 0);
    set_entity_pos(banner, door->x + (door->box.w / 2) - (banner->box.w / 2) + 200, door->y + door->box.w - 100);

    Entity* banner2 = malloc(sizeof(Entity));
    init_entity(banner2, 0, 0, 0, 0, 1, ENVIRONMENT_NO_MOVE);
    set_entity_anim(banner2, path_banner_pi, ANIM_IDLE, 1, 1, 0.1f);
    set_entity_scale(banner2, 0.2);
    set_hit_box(banner2, 0, 0, 0, 0);
    set_entity_pos(banner2, door->x + (door->box.w / 2) - (banner2->box.w / 2) - 200, door->y + door->box.w - 100);


    Entity* torch = malloc(sizeof(Entity));
    init_entity(torch, 0, 0, 0, 0, 1, ENVIRONMENT_MOVE);
    set_entity_anim(torch, path_torch, ANIM_IDLE, 4, 2, 0.2f);
    set_entity_scale(torch, 1.2);
    set_hit_box(torch, 0, 0, 0, 0);
    set_entity_pos(torch, door->x + (door->box.w / 2) - (torch->box.w / 2) - 100, door->y);

    Entity* torch2 = malloc(sizeof(Entity));
    init_entity(torch2, 0, 0, 0, 0, 1, ENVIRONMENT_MOVE);
    set_entity_anim(torch2, path_torch, ANIM_IDLE, 4, 2, 0.2f);
    set_entity_scale(torch2, 1.2);
    set_hit_box(torch2, 0, 0, 0, 0);
    set_entity_pos(torch2, door->x + (door->box.w / 2) - (torch2->box.w / 2) + 100, door->y);

    Entity* window_1 = malloc(sizeof(Entity));
    init_entity(window_1, 1000, (SCREEN_H / 2), 0, 0, 1, ENVIRONMENT_NO_MOVE);
    set_entity_anim(window_1, path_window_1, ANIM_IDLE, 1, 1, 0.1f);
    set_entity_scale(window_1, 2);
    set_hit_box(window_1, 0, 0, 0, 0);

    Entity* window_big = malloc(sizeof(Entity));
    init_entity(window_big, 700, 80, 0, 0, 1, ENVIRONMENT_NO_MOVE);
    set_entity_anim(window_big, path_window_big, ANIM_IDLE, 1, 1, 0.1f);
    set_entity_scale(window_big, 1.2);
    set_hit_box(window_big, 0, 0, 0, 0);

    Entity* window_2 = malloc(sizeof(Entity));
    init_entity(window_2, 2000, (SCREEN_H / 2), 0, 0, 1, ENVIRONMENT_NO_MOVE);
    set_entity_anim(window_2, path_window_1, ANIM_IDLE, 1, 1, 0.1f);
    set_entity_scale(window_2, 2);
    set_hit_box(window_2, 0, 0, 0, 0);

    Entity* bau = malloc(sizeof(Entity));
    init_entity(bau, 400, (SCREEN_H / 2) + 180, 0, 0, 1, ENVIRONMENT_NO_MOVE);
    set_entity_anim(bau, path_bau, ANIM_IDLE, 1, 1, 0.1f);
    set_entity_scale(bau, 1.2);
    set_hit_box(bau, 0, 0, 0, 0);

    add_world_entity(game, torch);
    add_world_entity(game, bau);
    add_world_entity(game, window_big);
    add_world_entity(game, window_1);
    add_world_entity(game, window_2);
    add_world_entity(game, torch2);
    add_world_entity(game, banner);
    add_world_entity(game, banner2);
    add_world_entity(game, door); 

}

void add_world_entity(Game* game, Entity* entity) {
    if (game->num_world_entities < MAX_WORLD_ENTITIES) {
        game->world_entities[game->num_world_entities] = entity;
        game->num_world_entities++;
    } else {
        printf("Não foi possível adicionar mais entidades. Limite atingido.\n");
    }
}

void reset_world_entities(Game* game){
    for(int i = 0; i < MAX_WORLD_ENTITIES; i++){
        game->world_entities[i] = NULL;
    }
    game->num_world_entities = 0;
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
        return BTN_NONE;
    }
} 

void check_battle(Game* game){
    if(!game->enemy->entity.isActive) return;
    // Com mudança no game_state precisaremos refatorar todo o código de state do game
    //if(game->state == GAMEPLAY_BATTLE && game->battle) return;

    int dist = game->player->entity.x - game->enemy->entity.x;

    if(dist >= -300 && dist <= 300){
        if(dist < 0) game->enemy->entity.flip = ALLEGRO_FLIP_HORIZONTAL;
        //game->state = GAMEPLAY_BATTLE;
        game->battle = start_battle(game->player, game->enemy);
    }
}

void menu_options(Game* game){
   Btn_state btn_state = is_mouse_in_btn(game);

    if (game->mouse.left && btn_state == BTN_INIT)
    {
        if(game->background){
            al_destroy_bitmap(game->background);
            game->background = NULL;         
        }

        reset_world_entities(game);
        render_initial_level(game);
    } else if(game->mouse.left && btn_state == BTN_EXIT){
        game->state = GAME_OVER;
    }
}

bool check_interaction(ALLEGRO_BITMAP* control, Entity* entity_1, Entity* entity_2){
    if(entity_1->box.x + entity_1->box.w >= entity_2->box.x && entity_1->box.x <= entity_2->box.x + entity_2->box.w 
        && entity_1->box.y + entity_1->box.h >= entity_2->box.y && entity_1->box.y <= entity_2->box.y + entity_2->box.h){
            if(entity_2->entity_type == DOOR){
                render_control(control, entity_2, INTERACT_E);
            }
            return true;
    }

    return false;
}

void render_control(ALLEGRO_BITMAP* control, Entity* entity, Key_code key){
    switch (key){
        case INTERACT_E:
            float draw_x = entity->x + (entity->box.w / 2) - (32 / 2); 
            float draw_y = entity->y - 30; 

            al_draw_scaled_bitmap(
                control,
                0, 0,
                16, 16,
                draw_x, draw_y,
                32, 32,
                0 
            );
            break;
    }

}

void resolve_interaction_with_door(Game* game, Entity* entity_1, Entity* entity_2, unsigned char* key){
    if(key[ALLEGRO_KEY_E]){
        game->state = GAME_SECOND_MISSION;
        render_second_map(game);

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
    
    if(game->state == GAME_MENU) {
        menu_options(game);
        return;
    }

    check_battle(game);

    if(game->gameplay_state == GAMEPLAY_EXPLORING || game->gameplay_state == GAMEPLAY_BATTLE){
        update_camera(game); 
    }

    if(game->gameplay_state == GAMEPLAY_EXPLORING){
        
        update_player(game->player, key, dt);

        for(int i = 0; i < game->num_world_entities; i++){
            Entity* current_entity = game->world_entities[i];
            if(current_entity && current_entity->isActive){
                update_entity(current_entity, dt);
            }

            if(current_entity && current_entity->entity_type == DOOR){
                bool check = check_interaction(game->controls, &game->player->entity, current_entity);

                if(check) resolve_interaction_with_door(game, &game->player->entity, current_entity, key);
            }

        }

        if(game->enemy->entity.isActive)
        // update_enemy(&game->enemy->entity, dt);
        update_entity(&game->enemy->entity, dt);
        return;
    } 

    if(game->gameplay_state == GAMEPLAY_BATTLE && game->battle){
        if(game->battle->state == BATTLE_END) {
            game->gameplay_state = GAMEPLAY_EXPLORING;
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

void draw_map(Map *map) {
    if (!map || !map->wall) return; 
    if(!map || !map->floor) return;

    const int TILES_PER_ROW = 1; 
    
    const float SOURCE_W = 32; 
    const float SOURCE_H = 32; 
    
    const float DEST_W = TILE_W; 
    const float DEST_H = TILE_H; 

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            float draw_x = x * DEST_W;
            float draw_y = y * DEST_H;

            int tile_index = map->tiles[y][x];

            int tile_col = tile_index % TILES_PER_ROW; 
            int tile_row = tile_index / TILES_PER_ROW; 

            float sx = (float)tile_col * SOURCE_W;
            float sy = (float)tile_row * SOURCE_H;


            if(map->tiles[y][x] == TILE_WALL){
                al_draw_scaled_bitmap(
                    map->wall,
                    sx, sy,
                    SOURCE_W, SOURCE_H,
                    draw_x, draw_y,
                    DEST_W, DEST_H,      
                    0                    
                );
            }

            if(map->tiles[y][x] == TILE_FLOOR){
                al_draw_scaled_bitmap(
                    map->floor,
                    sx, sy,
                    1024, 1024,
                    draw_x, draw_y,
                    128, 128,
                    0 
                );
            }
        }
    }
}

void draw_level_01(Game* game){
    al_draw_scaled_bitmap(
        game->background,
        0, 0, 2624, 1472,
        0, 0, 1280, 720,    
        0
    );

    for(int i = 0; i < game->num_world_entities; i++){
        Entity* current_entity = game->world_entities[i];
        if(current_entity && current_entity->isActive && current_entity->entity_type == DIALOGUE){
            draw_entity(current_entity);

            int mid_x = current_entity->x + (current_entity->box.w / 2);
            int mid_y = current_entity->y + (current_entity->box.h / 2);

            switch(game->init_dialogues){
                case DIALOGUE_1:
                    al_draw_text(game->subtitle_font, al_map_rgb(149, 98, 57), mid_x + 10, mid_y - 16, ALLEGRO_ALIGN_CENTER, "Seu nome é...");
                    al_draw_text(game->subtitle_font, al_map_rgb(149, 98, 57), mid_x + 10, mid_y + 8, ALLEGRO_ALIGN_CENTER, "Arthur IV"); 
                    break;    
                case DIALOGUE_2:    
                    al_draw_text(game->subtitle_font, al_map_rgb(149, 98, 57), mid_x + 10, mid_y - 16, ALLEGRO_ALIGN_CENTER, "Filho de...");
                    al_draw_text(game->subtitle_font, al_map_rgb(149, 98, 57), mid_x + 10, mid_y + 8, ALLEGRO_ALIGN_CENTER, "Arthur III"); 
                    break;
                case DIALOGUE_3:
                    al_draw_text(game->subtitle_font, al_map_rgb(149, 98, 57), mid_x + 10, mid_y - 16, ALLEGRO_ALIGN_CENTER, "Em busca de...");
                    al_draw_text(game->subtitle_font, al_map_rgb(149, 98, 57), mid_x + 10, mid_y + 8, ALLEGRO_ALIGN_CENTER, "conhecimento"); 
                    break;
                case DIALOGUE_4:
                    al_draw_text(game->subtitle_font, al_map_rgb(149, 98, 57), mid_x + 10, mid_y - 16, ALLEGRO_ALIGN_CENTER, "Você entra numa");
                    al_draw_text(game->subtitle_font, al_map_rgb(149, 98, 57), mid_x + 10, mid_y + 8, ALLEGRO_ALIGN_CENTER, "jornada!"); 
                    break;
                case DIALOGUE_5:
                    al_draw_text(game->subtitle_font, al_map_rgb(149, 98, 57), mid_x + 10, mid_y, ALLEGRO_ALIGN_CENTER, "Boa Sorte!");
                    break;
                case NEXT_LEVEL:
                    current_entity->isActive = false;
                    render_first_map(game);
                    game->gameplay_state = GAMEPLAY_EXPLORING;
                    game->state = GAME_FIRST_MISSION;
                    break;
            }
          
            if(game->init_dialogues != DIALOGUE_5){
            al_draw_text(game->subtitle_8_font, al_map_rgb(128, 78, 37), (current_entity->x + current_entity->box.w) - 160, (current_entity->y + current_entity->box.h) - 50, ALLEGRO_ALIGN_CENTER, "Clique para continuar");
                al_draw_scaled_bitmap(
                    game->controls,
                    0, 0,
                    16, 16,
                    (current_entity->x + current_entity->box.w) - 70, (current_entity->y + current_entity->box.h) - 52,
                    16, 16,
                    0 
                );        
            } else {
                al_draw_text(game->subtitle_8_font, al_map_rgb(128, 78, 37), (current_entity->x + current_entity->box.w) - 160, (current_entity->y + current_entity->box.h) - 50, ALLEGRO_ALIGN_CENTER, "Clique para começar");
                al_draw_scaled_bitmap(
                    game->controls,
                    0, 0,
                    16, 16,
                    (current_entity->x + current_entity->box.w) - 70, (current_entity->y + current_entity->box.h) - 52,
                    16, 16,
                    0 
                );        
            }
        }
    }
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
        game->background,
        0, 0, 1300, 736,
        0, 0, SCREEN_W, SCREEN_H,    
        0
    );

    al_draw_text(game->title_font, al_map_rgb(255, 0, 0), (SCREEN_W / 2), (SCREEN_H / 2) - 250, ALLEGRO_ALIGN_CENTER, "CRÔNICAS DO CÁLCULO");
    al_draw_text(game->subtitle_font, al_map_rgb(255, 255, 0), (SCREEN_W / 2), (SCREEN_H / 2) - 210, ALLEGRO_ALIGN_CENTER, "O DESPERTAR DO ARAUTO");

    al_draw_rectangle(btn1x - (btn_w / 2), btn1y, btn1x + (btn_w / 2), btn1y + btn_h, al_map_rgb(255, 255, 0), 2);
    al_draw_text(game->subtitle_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, (btn1y + btn_h) - (btn_h / 2) - 8, ALLEGRO_ALIGN_CENTER, "INICIAR");

    al_draw_rectangle(btn2x - (btn_w / 2), btn2y, btn2x + (btn_w / 2), btn2y + btn_h, al_map_rgb(255, 255, 0), 2);
    al_draw_text(game->subtitle_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, (btn2y + btn_h) - (btn_h / 2) - 8, ALLEGRO_ALIGN_CENTER, "OPÇÕES");

    al_draw_rectangle(btn3x - (btn_w / 2), btn3y, btn3x + (btn_w / 2), btn3y + btn_h, al_map_rgb(255, 255, 0), 2);
    al_draw_text(game->subtitle_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, (btn3y + btn_h) - (btn_h / 2) - 8, ALLEGRO_ALIGN_CENTER, "SAIR");

}


void draw_game(Game* game){
    ALLEGRO_TRANSFORM identity_transform;

    switch(game->state){
        case GAME_MENU:
            draw_menu(game);
            break;
        case GAME_INIT:
            draw_level_01(game);
            break;
        case GAME_FIRST_MISSION:
            al_use_transform(&game->camera_transform);

            draw_map(game->map);

            for(int i = 0; i < game->num_world_entities; i++){
                Entity* current_entity = game->world_entities[i];
                if(current_entity && current_entity->isActive){
                    draw_entity(current_entity);
                }

                
                if(current_entity && current_entity->entity_type == DOOR){
                    bool check = check_interaction(game->controls, &game->player->entity, current_entity);
                }
            }
            
            draw_entity(&game->player->entity);
            
            if(game->enemy->entity.isActive){
                draw_entity(&game->enemy->entity);
            }

            al_identity_transform(&identity_transform);
            al_use_transform(&identity_transform);
            
            // if(game->state == GAMEPLAY_BATTLE){
            //     al_draw_text(game->game_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, 50, ALLEGRO_ALIGN_CENTER, "BATALHA!");

            //     if(game->battle->turn_state == TURN_PLAYER)
            //         al_draw_text(game->game_font, al_map_rgb(255, 255, 0), SCREEN_W / 2, 150, ALLEGRO_ALIGN_CENTER, "Turno do player");
            //     else 
            //         al_draw_text(game->game_font, al_map_rgb(255, 255, 0), SCREEN_W / 2, 150, ALLEGRO_ALIGN_CENTER, "Turno do inimigo");


            //     al_draw_textf(game->game_font, al_map_rgb(255, 255, 0), 40, 70, ALLEGRO_ALIGN_LEFT, "HP: %d", game->player->entity.hp);

            //     al_draw_text(game->game_font, al_map_rgb(255, 255, 0), SCREEN_W - 40, 50, ALLEGRO_ALIGN_RIGHT, "Enemy");
            //     al_draw_textf(game->game_font, al_map_rgb(255, 255, 0), SCREEN_W - 40, 70, ALLEGRO_ALIGN_RIGHT, "HP: %d", game->enemy->entity.hp);
            // }
            break;
        case GAME_SECOND_MISSION:
            al_use_transform(&game->camera_transform);

            draw_map(game->map);

            for(int i = 0; i < game->num_world_entities; i++){
                Entity* current_entity = game->world_entities[i];
                if(current_entity && current_entity->isActive){
                    draw_entity(current_entity);
                }

                
                if(current_entity && current_entity->entity_type == DOOR){
                    bool check = check_interaction(game->controls, &game->player->entity, current_entity);
                }
            }
            
            draw_entity(&game->player->entity);
            
            if(game->enemy->entity.isActive){
                draw_entity(&game->enemy->entity);
            }

            al_identity_transform(&identity_transform);
            al_use_transform(&identity_transform);

        case GAME_OVER:

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

    if (game->background) {
        al_destroy_bitmap(game->background);
        game->background = NULL;
    }
    free(game);
}
