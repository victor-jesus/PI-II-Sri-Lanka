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
#include "door.h"

/*
    Animações do player, como são unicas decidi coloca-las aqui de maneira manual
*/
const char* path_idle = "assets/sprites/warrior_2/IDLE.png";
const char* path_run = "assets/sprites/warrior_2/RUN.png";
const char* path_attack = "assets/sprites/warrior_2/ATTACK_1.png";
const char* path_hit = "assets/sprites/warrior_2/HURT.png";

// Mesma coisa para o enemy, por enquanto
const char* path_minotaur_idle = "assets/sprites/minotaur_1/idle.png";
const char* path_minotaur_run = "assets/sprites/minotaur_1/walk.png";
const char* path_minotaur_attack = "assets/sprites/minotaur_1/attack.png";
const char* path_minotaur_hit = "assets/sprites/minotaur_1/hurt.png";


const char* path_medusa_idle = "assets/sprites/medusa_1/idle.png";
const char* path_medusa_walk = "assets/sprites/medusa_1/walk.png";
const char* path_medusa_attack = "assets/sprites/medusa_1/attack_1.png";
const char* path_medusa_hit = "assets/sprites/medusa_1/hurt.png";

const char* path_arauto_idle = "assets/sprites/arauto_1/idle.png";
const char* path_arauto_walk = "assets/sprites/arauto_1/walk.png";
const char* path_arauto_attack = "assets/sprites/arauto_1/attack_1.png";
const char* path_arauto_hit = "assets/sprites/arauto_1/hurt.png";

const char* path_map_tile = "assets/sprites/map/background-wall.png";
const char* path_map_tile_floor = "assets/sprites/map/chao-4.png";
const char* path_map_tile_floor_rock = "assets/sprites/map/teste-chao.png";

const char* path_map_tile_cave_floor = "assets/sprites/map/cave-chao.png";
const char* path_map_tile_cave_wall = "assets/sprites/map/textura-teste.png";

const char* path_banner = "assets/sprites/map/banners.png";
const char* path_banner_e = "assets/sprites/enviroment/banner-e.png";
const char* path_banner_pi = "assets/sprites/enviroment/banner-pi.png";
const char* path_door = "assets/sprites/enviroment/door_01.png";
const char* path_torch = "assets/sprites/map/torch.png";
const char* path_window_1 = "assets/sprites/map/window_1.png";
const char* path_window_big = "assets/sprites/map/big_window.png";
const char* path_bau = "assets/sprites/enviroment/bau.png";
const char* path_quadro_bhakara = "assets/sprites/enviroment/quadro-bhaskara.png";
const char* path_armadura_env = "assets/sprites/enviroment/armadura_1.png";

const char* path_initial_background = "assets/background_level_01.jpg";
const char* path_dialogue_box = "assets/sprites/ui/gui/dialogue_box.png";

const char* path_subtitle_8_font = "assets/fonts/pressStart2p.ttf";

const char* path_key_e = "assets/sprites/ui/controls/KEYBOARD/KEYS/E.png";


#define TILE_FLOOR_2 2 
#define TILE_FLOOR 1
#define TILE_WALL 0

void add_world_entity(Game* game, Entity* entity) {
    if (game->num_world_entities < MAX_WORLD_ENTITIES) {
        game->world_entities[game->num_world_entities] = entity;
        game->num_world_entities++;
    } else {
        printf("Não foi possível adicionar mais entidades. Limite atingido.\n");
    }
}

void reset_world_entities(Game* game){
    for(int i = 0; i < game->num_world_entities; i++){
        if(game->world_entities[i] != NULL){
            free(game->world_entities[i]);
            game->world_entities[i] = NULL; // Opcional, mas boa prática
        }
    }
    game->num_world_entities = 0;
}

Game* create_game(Game_state state, ALLEGRO_FONT* font, ALLEGRO_FONT* title_font, ALLEGRO_FONT* subtitle_font, int pos_x_player, int pos_y_player, int vx_player, int hp_player){
    Game* game = malloc(sizeof(Game));
    game->state = state;
    game->gameplay_state = GAMEPLAY_NONE;
    game->player = malloc(sizeof(Player));   

    game->map = malloc(sizeof(Map));
    game->map->wall = NULL;
    game->map->floor = NULL;
    game->map->floor_2 = NULL;

    game->num_world_entities = 0;
    game->init_dialogues = DIALOGUE_NONE;



    init_player(game->player, 100, pos_x_player, (SCREEN_H / 2) + 30, vx_player, 5, 25, 22, 30, 30);

    set_entity_anim(&game->player->entity, path_idle, ANIM_IDLE, 7, 1, 0.12f);
    set_entity_anim(&game->player->entity, path_run, ANIM_RUN, 8, 1, 0.07f);
    set_entity_anim(&game->player->entity, path_attack, ANIM_ATTACK, 6, 1, 0.1f);
    set_entity_anim(&game->player->entity, path_hit, ANIM_HIT, 4, 1, 0.1f);
    set_entity_scale(&game->player->entity, 2.5);

    game->enemy = NULL;
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
    reset_world_entities(game);

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

Entity* create_entity(const char* path, float x, float y, float scale, int frame, int cols, int rows, float dt, Entity_type entity_type){
    Entity* entity = malloc(sizeof(Entity));
    init_entity(entity, x, y, 0, 0, 1, entity_type);
    set_entity_anim(entity, path, ANIM_IDLE, cols, rows, dt);
    set_entity_scale(entity, scale);
    set_hit_box(entity, 0, 0, 0, 0);
    return entity;
}

Entity* create_torch(float x, float y) {
    Entity* torch = malloc(sizeof(Entity));
    init_entity(torch, x, y, 0, 0, 1, ENVIRONMENT_MOVE);
    set_entity_anim(torch, path_torch, ANIM_IDLE, 4, 2, 0.2f);
    set_entity_scale(torch, 1.2);
    set_hit_box(torch, 0, 0, 0, 0);
    return torch;
}

Entity* create_window(const char* path, float x, float y, float scale) {
    Entity* window = malloc(sizeof(Entity));
    init_entity(window, x, y, 0, 0, 1, ENVIRONMENT_NO_MOVE);
    set_entity_anim(window, path, ANIM_IDLE, 1, 1, 0.1f);
    set_entity_scale(window, scale);
    set_hit_box(window, 0, 0, 0, 0);
    return window;
}

Door* create_door(float x, float y, Door_type type) {
    Door* door = malloc(sizeof(Door));
    init_entity(&door->entity, x, y, 0, 0, 1, DOOR);
    set_entity_anim(&door->entity, path_door, ANIM_IDLE, 1, 1, 0.1f);
    set_entity_scale(&door->entity, 0.5);
    set_hit_box(&door->entity, 0, 0, 0, 0);
    set_door_type(door, type);
    return door;
}

Entity* create_banner(const char* path, float x, float y, float scale, int frames, int rows) {
    Entity* banner = malloc(sizeof(Entity));
    init_entity(banner, x, y, 0, 0, 1, ENVIRONMENT_NO_MOVE);
    set_entity_anim(banner, path, ANIM_IDLE, frames, rows, 0.1f);
    set_entity_scale(banner, scale);
    set_hit_box(banner, 0, 0, 0, 0);
    return banner;
}
    
/*
    Refatorar as funções de criar entidades e implementar posição relacionada ao eixo X (LEVEL_WIDTH * porcentagem)
*/
void load_first_map(Game* game) {
    reset_world_entities(game);
    game->state = GAME_FIRST_MISSION;
    game->gameplay_state = GAMEPLAY_EXPLORING;

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (y == MAP_HEIGHT - 2)
                game->map->tiles[y][x] = TILE_FLOOR;
            else if (y == MAP_HEIGHT - 1)
                game->map->tiles[y][x] = TILE_FLOOR_2;
            else
                game->map->tiles[y][x] = TILE_WALL;
        }
    }
    init_map(game->map, path_map_tile, path_map_tile_floor, path_map_tile_floor_rock);

    const float LEVEL_WIDTH = 6000.0f;
    const float GROUND_Y = (SCREEN_H / 2.0f) - 96.0f;
    const float WALL_Y = (SCREEN_H / 2.0f) - 250.0f;

    Entity* bhaskara_frame = malloc(sizeof(Entity));
    init_entity(bhaskara_frame, LEVEL_WIDTH * 0.05f, WALL_Y, 0, 0, 1, ENVIRONMENT_NO_MOVE);
    set_entity_anim(bhaskara_frame, path_quadro_bhakara, ANIM_IDLE, 1, 1, 0.1f);
    set_entity_scale(bhaskara_frame, 1.5);
    set_hit_box(bhaskara_frame, 0, 0, 0, 0);

    Entity* first_small_window = create_window(path_window_1, LEVEL_WIDTH * 0.13f, WALL_Y + 50, 2.0f);

    Door* minotaur_door = create_door(LEVEL_WIDTH * 0.22f, GROUND_Y, DOOR_MINOTAUR);
    Entity* central_large_window = create_window(path_window_big, LEVEL_WIDTH * 0.30f, -15, 1.0f);
    Entity* pi_banner_1 = create_banner(path_banner_pi, LEVEL_WIDTH * 0.26f, WALL_Y - 50, 0.5f, 1, 1);
    Entity* pi_banner_2 = create_banner(path_banner_pi, LEVEL_WIDTH * 0.34f, WALL_Y - 50, 0.5f, 1, 1);
    Entity* central_torch = create_torch(LEVEL_WIDTH * 0.40f, GROUND_Y);
    Entity* knight_armor = create_entity(path_armadura_env, LEVEL_WIDTH * 0.19, WALL_Y + 50, 0.2f, 1, 1, 1, 0.1f, ENVIRONMENT_NO_MOVE);

    Entity* euler_banner_1 = create_banner(path_banner_e, LEVEL_WIDTH * 0.47f, WALL_Y, 0.3f, 1, 1);
    Entity* second_small_window = create_window(path_window_1, LEVEL_WIDTH * 0.57f, WALL_Y + 50, 2.0f);
    Entity* euler_banner_2 = create_banner(path_banner_e, LEVEL_WIDTH * 0.67f, WALL_Y, 0.3f, 1, 1);
    Entity* corridor_torch = create_torch(LEVEL_WIDTH * 0.73f, GROUND_Y);

    Door* final_door = create_door(LEVEL_WIDTH * 0.97f, GROUND_Y, DOOR_NEXT_LEVEL);
    
    Entity* final_torch_left = create_torch(final_door->entity.x - 120, final_door->entity.y);
    Entity* final_torch_right = create_torch(final_door->entity.x + 120, final_door->entity.y);
    Entity* final_banner_left = create_banner(path_banner, final_door->entity.x - 70, final_door->entity.y, 0.5f, 6, 1);
    Entity* final_banner_right = create_banner(path_banner, final_door->entity.x + 70, final_door->entity.y, 0.5f, 6, 1);
    
    add_world_entity(game, bhaskara_frame);
    add_world_entity(game, knight_armor);
    add_world_entity(game, first_small_window);
    add_world_entity(game, &minotaur_door->entity);
    add_world_entity(game, central_large_window);
    add_world_entity(game, pi_banner_1);
    add_world_entity(game, pi_banner_2);
    add_world_entity(game, central_torch);
    add_world_entity(game, euler_banner_1);
    add_world_entity(game, second_small_window);
    add_world_entity(game, euler_banner_2);
    add_world_entity(game, corridor_torch);
    add_world_entity(game, &final_door->entity);
    add_world_entity(game, final_torch_left);
    add_world_entity(game, final_torch_right);
    add_world_entity(game, final_banner_left);
    add_world_entity(game, final_banner_right);
}

void load_second_map(Game* game) {
    reset_world_entities(game);
    game->state = GAME_SECOND_MISSION;
    game->gameplay_state = GAMEPLAY_EXPLORING;

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            game->map->tiles[y][x] = (y >= MAP_HEIGHT - 2) ? TILE_FLOOR : TILE_WALL;
        }
    }
    init_map(game->map, path_map_tile_cave_wall, path_map_tile_cave_floor, path_map_tile_cave_floor);

    const float LEVEL_WIDTH = 6000.0f;
    const float GROUND_Y = (SCREEN_H / 2.0f) - 98.0f; // Posição Y base para objetos no chão
    const float WALL_Y = (SCREEN_H / 2.0f) - 200.0f; // Posição Y base para objetos na parede

    Door* start_door = create_door(LEVEL_WIDTH * 0.1f, GROUND_Y, DOOR_RETURN);
    Door* medusa_door = create_door(LEVEL_WIDTH * 0.4f, GROUND_Y, DOOR_MEDUSA);
    Door* end_door = create_door(LEVEL_WIDTH * 0.9f, GROUND_Y, DOOR_NEXT_LEVEL);

    game->player->entity.x = start_door->entity.x;

    Entity* left_torch = create_torch(start_door->entity.x - 120, start_door->entity.y);
    Entity* right_torch = create_torch(start_door->entity.x + 120, start_door->entity.y);
    
    Entity* window_1 = create_window(path_window_1, LEVEL_WIDTH * 0.25f, WALL_Y, 2.0f);
    Entity* center_window = create_window(path_window_big, LEVEL_WIDTH * 0.5f, WALL_Y - 170, 1.0f);
    Entity* window_2 = create_window(path_window_1, LEVEL_WIDTH * 0.75f, WALL_Y, 2.0f);

    add_world_entity(game, &start_door->entity);
    add_world_entity(game, &medusa_door->entity);
    add_world_entity(game, &end_door->entity);
    add_world_entity(game, left_torch);
    add_world_entity(game, right_torch);
    add_world_entity(game, window_1);
    add_world_entity(game, center_window);
    add_world_entity(game, window_2);
}

void load_third_map(Game* game) {
    reset_world_entities(game);
    game->state = GAME_THIRD_MISSION;
    game->gameplay_state = GAMEPLAY_EXPLORING;

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (y == MAP_HEIGHT - 2)
                game->map->tiles[y][x] = TILE_FLOOR;
            else if (y == MAP_HEIGHT - 1)
                game->map->tiles[y][x] = TILE_FLOOR_2;
            else
                game->map->tiles[y][x] = TILE_WALL;
        }
    }
    init_map(game->map, path_map_tile, path_map_tile_floor, path_map_tile_floor_rock);

    const float LEVEL_WIDTH = 6200.0f;
    const float GROUND_Y = (SCREEN_H / 2.0f) - 96.0f;
    const float WALL_Y = (SCREEN_H / 2.0f) - 250.0f;

    Door* start_door = create_door(LEVEL_WIDTH * 0.03f, GROUND_Y, DOOR_RETURN);
    Door* arauto_door = create_door(LEVEL_WIDTH * 0.07f, GROUND_Y, DOOR_ARAUTO);
    game->player->entity.x = start_door->entity.x;

    Entity* entry_torch = create_torch(LEVEL_WIDTH * 0.06f, GROUND_Y);
    
    Entity* bhaskara_frame = malloc(sizeof(Entity));
    init_entity(bhaskara_frame, LEVEL_WIDTH * 0.09f, WALL_Y, 0, 0, 1, ENVIRONMENT_NO_MOVE);
    set_entity_anim(bhaskara_frame, path_quadro_bhakara, ANIM_IDLE, 1, 1, 0.1f);
    set_entity_scale(bhaskara_frame, 1.5);
    set_hit_box(bhaskara_frame, 0, 0, 0, 0);

    Entity* corridor_window_1 = create_window(path_window_1, LEVEL_WIDTH * 0.15f, WALL_Y + 50, 2.0f);
    Entity* corridor_banner_1 = create_banner(path_banner_e, LEVEL_WIDTH * 0.25f, WALL_Y, 0.3f, 1, 1);
    Entity* corridor_window_2 = create_window(path_window_1, LEVEL_WIDTH * 0.35f, WALL_Y + 50, 2.0f);
    Entity* corridor_banner_2 = create_banner(path_banner_e, LEVEL_WIDTH * 0.45f, WALL_Y, 0.3f, 1, 1);

    Entity* hall_banner_1 = create_banner(path_banner_pi, LEVEL_WIDTH * 0.55f, WALL_Y - 50, 0.5f, 1, 1);
    Entity* hall_large_window = create_window(path_window_big, LEVEL_WIDTH * 0.65f, -15, 1.0f);
    Entity* hall_banner_2 = create_banner(path_banner_pi, LEVEL_WIDTH * 0.75f, WALL_Y - 50, 0.5f, 1, 1);
    Door* minotaur_door = create_door(LEVEL_WIDTH * 0.80f, GROUND_Y, DOOR_MINOTAUR);
    Entity* hall_torch = create_torch(minotaur_door->entity.x - 70, GROUND_Y);

    Door* final_door = create_door(LEVEL_WIDTH * 0.95f, GROUND_Y, DOOR_NEXT_LEVEL);

    Entity* final_banner_left = create_banner(path_banner, final_door->entity.x - 200, final_door->entity.y - 100, 0.5f, 6, 1);
    Entity* final_torch_left = create_torch(final_door->entity.x - 120, final_door->entity.y);
    Entity* final_torch_right = create_torch(final_door->entity.x + 120, final_door->entity.y);
    Entity* final_banner_right = create_banner(path_banner, final_door->entity.x + 200, final_door->entity.y - 100, 0.5f, 6, 1);

    add_world_entity(game, &start_door->entity);
    add_world_entity(game, &arauto_door->entity);
    add_world_entity(game, entry_torch);
    add_world_entity(game, bhaskara_frame);
    add_world_entity(game, corridor_window_1);
    add_world_entity(game, corridor_banner_1);
    add_world_entity(game, corridor_window_2);
    add_world_entity(game, corridor_banner_2);
    add_world_entity(game, hall_banner_1);
    add_world_entity(game, hall_large_window);
    add_world_entity(game, hall_banner_2);
    add_world_entity(game, &minotaur_door->entity);
    add_world_entity(game, hall_torch);
    add_world_entity(game, &final_door->entity);
    add_world_entity(game, final_banner_left);
    add_world_entity(game, final_torch_left);
    add_world_entity(game, final_torch_right);
    add_world_entity(game, final_banner_right);
}

void render_minotaur_level(Game* game){
    game->background = al_load_bitmap("assets/sprites/map/minotaur/background-minotaur.png");
    reset_world_entities(game);

    game->enemy = malloc(sizeof(Enemy));

    init_enemy(game->enemy, 800, 400, 5, 100);
    set_entity_anim(&game->enemy->entity, path_minotaur_idle, ANIM_IDLE, 10, 1, 0.1f);
    set_entity_anim(&game->enemy->entity, path_minotaur_run, ANIM_RUN, 12, 1, 0.06f);
    set_entity_anim(&game->enemy->entity, path_minotaur_attack, ANIM_ATTACK, 5, 1, 0.1f);
    set_entity_anim(&game->enemy->entity, path_minotaur_hit, ANIM_HIT, 3, 1, 0.1f);
    game->enemy->entity.flip = ALLEGRO_FLIP_HORIZONTAL;
    set_entity_scale(&game->enemy->entity, 2.0);
    set_hit_box(&game->enemy->entity, 0, 0, 0, 0);
    
    game->gameplay_state = GAMEPLAY_EXPLORING; 

    game->player->entity.x = 50; 

    game->player->entity.y = 505; 
}

void render_medusa_level(Game* game){
    game->background = al_load_bitmap("assets/sprites/map/medusa/background-medusa.png");
    reset_world_entities(game);

    game->enemy = malloc(sizeof(Enemy));

    init_enemy(game->enemy, 800, 400, 5, 100);
    set_entity_anim(&game->enemy->entity, path_medusa_idle, ANIM_IDLE, 7, 1, 0.1f);
    set_entity_anim(&game->enemy->entity, path_medusa_walk, ANIM_RUN, 13, 1, 0.06f);
    set_entity_anim(&game->enemy->entity, path_medusa_attack, ANIM_ATTACK, 16, 1, 0.1f);
    set_entity_anim(&game->enemy->entity, path_medusa_hit, ANIM_HIT, 3, 1, 0.1f);
    game->enemy->entity.flip = ALLEGRO_FLIP_HORIZONTAL;
    set_entity_scale(&game->enemy->entity, 2.0);
    set_hit_box(&game->enemy->entity, 0, 0, 0, 0);
    
    game->gameplay_state = GAMEPLAY_EXPLORING; 

    game->player->entity.x = 50; 

    game->player->entity.y = 505; 
}

void render_arauto_level(Game* game){
    game->background = al_load_bitmap("assets/sprites/map/arauto/background-arauto.png");
    reset_world_entities(game);

    game->enemy = malloc(sizeof(Enemy));

    init_enemy(game->enemy, 800, 400, 5, 100);
    set_entity_anim(&game->enemy->entity, path_arauto_idle, ANIM_IDLE, 6, 1, 0.1f);
    set_entity_anim(&game->enemy->entity, path_arauto_walk, ANIM_RUN, 8, 1, 0.06f);
    set_entity_anim(&game->enemy->entity, path_arauto_attack, ANIM_ATTACK, 6, 1, 0.1f);
    set_entity_anim(&game->enemy->entity, path_arauto_hit, ANIM_HIT, 3, 1, 0.1f);
    game->enemy->entity.flip = ALLEGRO_FLIP_HORIZONTAL;
    set_entity_scale(&game->enemy->entity, 2.0);
    set_hit_box(&game->enemy->entity, 0, 0, 0, 0);
    
    game->gameplay_state = GAMEPLAY_EXPLORING; 

    game->player->entity.x = 50; 

    game->player->entity.y = 505; 
}

void draw_minotaur_level(Game* game){
    al_draw_scaled_bitmap(
        game->background,
        0, 0,
        576, 324,
        0, 0,
        SCREEN_W, SCREEN_H,
        0
    );
}

void draw_medusa_level(Game* game){
    al_draw_scaled_bitmap(
        game->background,
        0, 0,
        1920, 1080,
        0, 0,
        SCREEN_W, SCREEN_H,
        0
    );
}

void draw_arauto_level(Game* game){
    al_draw_scaled_bitmap(
        game->background,
        0, 0,
        1920, 1080,
        0, 0,
        SCREEN_W, SCREEN_H,
        0
    );
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
    if (!game->enemy) {
        return; 
    }
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

void return_level(Game* game, Door* door_1){
    if(door_1->door_type != DOOR_RETURN) return;

    if(game->state == GAME_SECOND_MISSION){
        game->state = GAME_FIRST_MISSION;
        load_first_map(game);

        for(int i = 0; i < MAX_WORLD_ENTITIES; i++){
            Entity* current_entity = game->world_entities[i];

            if(current_entity->entity_type == DOOR){
                Door* door = (Door*) current_entity;

                if(door->door_type == DOOR_NEXT_LEVEL){
                    game->player->entity.x = door->entity.x;
                    break;
                }
            }
        }
    } else if(game->state == GAME_THIRD_MISSION){
        game->state = GAME_SECOND_MISSION;
        load_second_map(game);

        for(int i = 0; i < MAX_WORLD_ENTITIES; i++){
            Entity* current_entity = game->world_entities[i];

            if(current_entity->entity_type == DOOR){
                Door* door = (Door*) current_entity;

                if(door->door_type == DOOR_NEXT_LEVEL){
                    game->player->entity.x = door->entity.x;
                    break;
                }
            }
        }
    }
}

void resolve_interaction_with_door(Game* game, Entity* entity_1, Entity* entity_2, unsigned char* key){
    if (entity_2->entity_type != DOOR) return;

    Door* door = (Door*) entity_2;

    if(key[ALLEGRO_KEY_E]){
        switch(door->door_type){
            case DOOR_RETURN:
                return_level(game, door);
                break;
            case DOOR_MINOTAUR:
                game->state = GAME_MINOTAUR_LEVEL;
                render_minotaur_level(game);
                break;
            case DOOR_MEDUSA:
                game->state = GAME_MEDUSA_LEVEL;
                render_medusa_level(game);
                break;
            case DOOR_ARAUTO:
                game->state = GAME_ARAUTO_LEVEL;
                render_arauto_level(game);
                break;
            case DOOR_NEXT_LEVEL:
                if(game->state == GAME_FIRST_MISSION){
                    game->state = GAME_SECOND_MISSION;
                    load_second_map(game);
                } else if(game->state == GAME_SECOND_MISSION){
                    game->state = GAME_THIRD_MISSION;
                    load_third_map(game);
                }
        }
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

static void update_minotaur_level(Game* game, unsigned char* key, float dt) {
    update_player(game->player, key, dt);

    if (game->player->entity.x > SCREEN_W / 2) {
        game->player->entity.x = SCREEN_W / 2;
    }

    if (game->enemy && game->enemy->entity.isActive) {
        update_entity(&game->enemy->entity, dt);
    }

    if (key[ALLEGRO_KEY_K]) {
        if (game->enemy) {
            destroy_enemy(game->enemy);
            game->enemy = NULL;
        }


        load_first_map(game); 
        game->state = GAME_FIRST_MISSION;
            
        for(int i = 0; i < game->num_world_entities; i++) {
            Entity* current_entity = game->world_entities[i];
            if(current_entity && current_entity->entity_type == DOOR) {
                Door* door = (Door*) current_entity;
                if(door->door_type == DOOR_MINOTAUR) {
                    set_entity_pos(&game->player->entity, door->entity.x, door->entity.y);
                }
            }
        }
    }
}

static void update_medusa_level(Game* game, unsigned char* key, float dt) {
    update_player(game->player, key, dt);

    if (game->player->entity.x > SCREEN_W / 2) {
        game->player->entity.x = SCREEN_W / 2;
    }

    if (game->enemy && game->enemy->entity.isActive) {
        update_entity(&game->enemy->entity, dt);
    }

    if (key[ALLEGRO_KEY_K]) {
        if (game->enemy) {
            destroy_enemy(game->enemy);
            game->enemy = NULL; 
        }

        load_second_map(game); 
        game->state = GAME_SECOND_MISSION;
            
        for(int i = 0; i < game->num_world_entities; i++) {
            Entity* current_entity = game->world_entities[i];
            if(current_entity && current_entity->entity_type == DOOR) {
                Door* door = (Door*) current_entity;
                if(door->door_type == DOOR_MEDUSA) {
                    set_entity_pos(&game->player->entity, door->entity.x, door->entity.y);
                }
            }
        }
    }
}

static void update_arauto_level(Game* game, unsigned char* key, float dt) {
    update_player(game->player, key, dt);

    if (game->player->entity.x > SCREEN_W / 2) {
        game->player->entity.x = SCREEN_W / 2;
    }

    if (game->enemy && game->enemy->entity.isActive) {
        update_entity(&game->enemy->entity, dt);
    }

    if (key[ALLEGRO_KEY_K]) {
        if (game->enemy) {
            destroy_enemy(game->enemy);
            game->enemy = NULL; 
        }

        load_third_map(game); 
        game->state = GAME_THIRD_MISSION;
            
        for(int i = 0; i < game->num_world_entities; i++) {
            Entity* current_entity = game->world_entities[i];
            if(current_entity && current_entity->entity_type == DOOR) {
                Door* door = (Door*) current_entity;
                if(door->door_type == DOOR_ARAUTO) {
                    set_entity_pos(&game->player->entity, door->entity.x, door->entity.y);
                }
            }
        }
    }
}

static void update_exploring_state(Game* game, unsigned char* key, float dt) {
    update_camera(game);
    update_player(game->player, key, dt);

    for(int i = 0; i < game->num_world_entities; i++) {
        Entity* current_entity = game->world_entities[i];
        if (current_entity && current_entity->isActive) {
            update_entity(current_entity, dt);
        }

        if (current_entity && current_entity->entity_type == DOOR) {
            if (check_interaction(game->controls, &game->player->entity, current_entity)) {
                resolve_interaction_with_door(game, &game->player->entity, current_entity, key);
            }
        }
    }

    if (game->enemy && game->enemy->entity.isActive) {
        update_entity(&game->enemy->entity, dt);
    }
}

static void update_battle_state(Game* game, ALLEGRO_EVENT event, ALLEGRO_TIMER* timer_enemy, float dt) {
    if (!game->battle) {
        game->gameplay_state = GAMEPLAY_EXPLORING;
        return;
    }

    if (game->battle->state == BATTLE_END) {
        game->gameplay_state = GAMEPLAY_EXPLORING;
        game->battle = NULL;
        return;
    }

    update_camera(game);

    if (game->enemy && game->enemy->entity.isActive) {
        update_entity(&game->enemy->entity, dt);
    }
    
    manage_battle(game->battle, event, timer_enemy);
    update_player_battle(game->player, dt);
}

void update_game(Game* game, unsigned char* key, ALLEGRO_EVENT event, ALLEGRO_TIMER* timer_enemy, float dt) {
    read_mouse(game);

    switch (game->state) {
        case GAME_MENU:
            menu_options(game);
            break;

        case GAME_MINOTAUR_LEVEL:
            update_minotaur_level(game, key, dt);
            break;
        case GAME_MEDUSA_LEVEL:
            update_medusa_level(game, key, dt);
            break;
        case GAME_ARAUTO_LEVEL:
            update_arauto_level(game, key, dt);
            break;
        case GAME_FIRST_MISSION:
            check_battle(game);
            
            switch (game->gameplay_state) {
                case GAMEPLAY_EXPLORING:
                    update_exploring_state(game, key, dt);
                    break;
                case GAMEPLAY_BATTLE:
                    update_battle_state(game, event, timer_enemy, dt);
                    break;
                case GAMEPLAY_NONE:
                default:
                    break;
            }
            break;
        case GAME_SECOND_MISSION:
            check_battle(game);
            
            switch (game->gameplay_state) {
                case GAMEPLAY_EXPLORING:
                    update_exploring_state(game, key, dt);
                    break;
                case GAMEPLAY_BATTLE:
                    update_battle_state(game, event, timer_enemy, dt);
                    break;
                case GAMEPLAY_NONE:
                default:
                    break;
            }
            break;
        case GAME_THIRD_MISSION:
            check_battle(game);
            
            switch (game->gameplay_state) {
                case GAMEPLAY_EXPLORING:
                    update_exploring_state(game, key, dt);
                    break;
                case GAMEPLAY_BATTLE:
                    update_battle_state(game, event, timer_enemy, dt);
                    break;
                case GAMEPLAY_NONE:
                default:
                    break;
            }
            break;
        
        case GAME_INIT:
        case GAME_OVER:
        default:
            break;
    }
}

void create_button(Button* btn, int x, int y, int w, int h){
    btn->x = x - (w / 2);
    btn->y = y;
    btn->w = w;
    btn->h = h;
}

void draw_map(Map *map) {
    if (!map || !map->wall || !map->floor) return; 
    
    const float DEST_W = TILE_W; 
    const float DEST_H = TILE_H; 
    
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            float draw_x = x * DEST_W;
            float draw_y = y * DEST_H;

            ALLEGRO_BITMAP *tile_to_draw = NULL;
            
            if(map->tiles[y][x] == TILE_WALL){
                tile_to_draw = map->wall;
            } else if(map->tiles[y][x] == TILE_FLOOR){
                tile_to_draw = map->floor;
            } else if(map->tiles[y][x] == TILE_FLOOR_2){
                tile_to_draw = map->floor_2;
            }
            
            if (tile_to_draw) {
                float SOURCE_W = al_get_bitmap_width(tile_to_draw);
                float SOURCE_H = al_get_bitmap_height(tile_to_draw);
                
                al_draw_scaled_bitmap(
                    tile_to_draw,
                    0, 0, 
                    SOURCE_W, SOURCE_H,
                    draw_x, draw_y,
                    DEST_W, DEST_H,      
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
                    load_first_map(game);
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
        case GAME_MINOTAUR_LEVEL:   

            draw_minotaur_level(game);
            
            draw_entity(&game->player->entity);

            if(game->enemy->entity.isActive){
                draw_entity(&game->enemy->entity);
            }

            al_identity_transform(&identity_transform);
            al_use_transform(&identity_transform);
            
            break;
        case GAME_MEDUSA_LEVEL:   

            draw_medusa_level(game);
            
            draw_entity(&game->player->entity);

            if(game->enemy->entity.isActive){
                draw_entity(&game->enemy->entity);
            }

            al_identity_transform(&identity_transform);
            al_use_transform(&identity_transform);
            
            break;
        case GAME_ARAUTO_LEVEL:   

            draw_arauto_level(game);
            
            draw_entity(&game->player->entity);

            if(game->enemy->entity.isActive){
                draw_entity(&game->enemy->entity);
            }

            al_identity_transform(&identity_transform);
            al_use_transform(&identity_transform);
            
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
            
            if(game->enemy && game->enemy->entity.isActive){
                draw_entity(&game->enemy->entity);
            }

            al_identity_transform(&identity_transform);
            al_use_transform(&identity_transform);
            
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
            
            if(game->enemy && game->enemy->entity.isActive){
                draw_entity(&game->enemy->entity);
            }

            al_identity_transform(&identity_transform);
            al_use_transform(&identity_transform);
            break;
        case GAME_THIRD_MISSION:
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
            
            if(game->enemy && game->enemy->entity.isActive){
                draw_entity(&game->enemy->entity);
            }

            al_identity_transform(&identity_transform);
            al_use_transform(&identity_transform);
            break;
        case GAME_OVER:
            reset_world_entities(game);

            al_draw_text(game->title_font, al_map_rgb(255, 0, 0), SCREEN_W / 2, SCREEN_H / 2, ALLEGRO_ALIGN_CENTER, "Você morreu!");
            break;
    }
}

void destroy_game(Game* game) {
     if (!game) return;

    reset_world_entities(game);

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
        destroy_map(game->map); 
        game->map = NULL;
    }

    if (game->background) {
        al_destroy_bitmap(game->background);
        game->background = NULL;
    }
    
    if (game->controls) {
        al_destroy_bitmap(game->controls);
        game->controls = NULL;
    }

    if (game->subtitle_8_font) {
        al_destroy_font(game->subtitle_8_font);
        game->subtitle_8_font = NULL;
    }

    free(game);
}

