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
#include "box.h"
#include <string.h>
#include "game_state.h"

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
const char* path_minotaur_death = "assets/sprites/minotaur_1/dead.png";

const char* path_skeleton_idle = "assets/sprites/skeleton/idle.png";
const char* path_skeleton_run = "assets/sprites/skeleton/walk.png";
const char* path_skeleton_attack = "assets/sprites/skeleton/attack.png";
const char* path_skeleton_hit = "assets/sprites/skeleton/hit.png";
const char* path_skeleton_death = "assets/sprites/skeleton/dead.png";

const char* path_medusa_idle = "assets/sprites/medusa_1/idle.png";
const char* path_medusa_walk = "assets/sprites/medusa_1/walk.png";
const char* path_medusa_attack = "assets/sprites/medusa_1/attack_1.png";
const char* path_medusa_hit = "assets/sprites/medusa_1/hurt.png";
const char* path_medusa_dead = "assets/sprites/medusa_1/dead.png";

const char* path_arauto_idle = "assets/sprites/arauto_1/Idle_2.png";
const char* path_arauto_walk = "assets/sprites/arauto_1/walk.png";
const char* path_arauto_attack = "assets/sprites/arauto_1/Attack_2.png";
const char* path_arauto_hit = "assets/sprites/arauto_1/hurt.png";
const char* path_arauto_dead = "assets/sprites/arauto_1/dead.png";

const char* path_portrait_arauto = "assets/sprites/arauto_1/dead.png";
const char* path_hp_canva_arauto = "assets/sprites/arauto_1/dead.png";

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

const char* path_quadro_bhaskara = "assets/sprites/enviroment/quadro-bhaskara.png";
const char* path_tabuleta_bhaskara = "assets/sprites/enviroment/tabuleta-bhaskara.png";
const char* path_tabuleta_bhaskara_2 = "assets/sprites/enviroment/tabulete-bhaskara_2.png";

const char* path_roda = "assets/sprites/enviroment/roda.png";
const char* path_armadura_env = "assets/sprites/enviroment/armadura_1.png";

const char* path_heart = "assets/sprites/transparent/heart.png";
const char* path_shield = "assets/sprites/transparent/shield.png";
const char* path_shield_enemy = "assets/sprites/transparent/shield_enemy.png";
const char* path_sword_ui = "assets/sprites/transparent/sword.png";

const char* path_hp_canva_minotaur = "assets/sprites/minotaur_1/minotaur_hp.png";
const char* path_portrait_minotaur = "assets/sprites/minotaur_1/portrait_minotaur.jpeg";

const char* path_hp_canva_medusa = "assets/sprites/minotaur_1/minotaur_hp.png";
const char* path_portrait_medusa = "assets/sprites/medusa_1/portrait_medusa.png";

const char* path_portrait_player = "assets/sprites/warrior_2/portrait_player.png";
const char* path_player_sprite_hp = "assets/sprites/warrior_2/hp_player.png";

const char* path_initial_background = "assets/background_level_01.jpg";
const char* path_dialogue_box = "assets/sprites/ui/gui/dialogue_box.png";
const char* path_dialogue_box_battle = "assets/sprites/ui/gui/dialogue_box_battle.png";

const char* path_subtitle_8_font = "assets/fonts/pressStart2p.ttf";
const char* path_log_font = "assets/fonts/fonte_titulo.ttf";

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
            game->world_entities[i] = NULL;
        }
    }
    game->num_world_entities = 0;
}

void initial_inventory(Game* game){
    inventory_add_item(&game->player->inventory, BIG_POTION, 2);
    inventory_add_item(&game->player->inventory, SMALL_POTION, 3);
    inventory_add_item(&game->player->inventory, WATER, 1);
}

void change_game_state(Game* game, Game_state game_state){
    game->state = game_state;
}

Game* create_game(Game_state state, ALLEGRO_FONT* font, ALLEGRO_FONT* title_font, ALLEGRO_FONT* subtitle_font, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_EVENT* event, int pos_x_player, int pos_y_player, int vx_player, int hp_player){
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

    game->world_enemies = 0;

    for(int i = 0; i < MAX_ENEMIES; i++){
        game->mobs[i] = NULL;
    }
    
    init_player(game->player, "Estudante", 100, pos_x_player, (SCREEN_H / 2) + 60, vx_player, 5, 10, 10, 25, 22, 30, 30);
    game->player->portrait = al_load_bitmap(path_portrait_player);
    game->player->player_hp = al_load_bitmap(path_player_sprite_hp);

    Entity* hp = malloc(sizeof(Entity));
    init_entity(hp, 0, 0, 0, 0, 1, UI);
    set_entity_anim(hp, path_heart, ANIM_IDLE, 1, 1, 0.1f);
    set_entity_scale(hp, 1.5);
    set_hit_box(hp, 0, 0, 0, 0);
    set_entity_pos(hp, SCREEN_W - (hp->box.w) - 20, 10);
    add_world_entity(game, hp);

    game->queue = queue;
    game->event = event;

    set_entity_anim(&game->player->entity, path_idle, ANIM_IDLE, 7, 1, 0.12f);
    set_entity_anim(&game->player->entity, path_run, ANIM_RUN, 8, 1, 0.07f);
    set_entity_anim(&game->player->entity, path_attack, ANIM_ATTACK, 6, 1, 0.1f);
    set_entity_anim(&game->player->entity, path_hit, ANIM_HIT, 4, 1, 0.1f);
    set_entity_scale(&game->player->entity, 2.5);
    
    game->enemy = NULL;

    game->battle = malloc(sizeof(Battle));

    game->battle->timer_end = al_create_timer(5.0);
    game->battle->log_timer = al_create_timer(5.0);
    game->battle->timer_enemy = al_create_timer(3.5);
    game->timer_game_logs = al_create_timer(5.0);
    game->timer_game_tips = al_create_timer(10.0);
    game->battle->battle_font = subtitle_font;
    al_register_event_source(game->queue, al_get_timer_event_source(game->battle->timer_end));
    al_register_event_source(game->queue, al_get_timer_event_source(game->battle->log_timer));
    al_register_event_source(game->queue, al_get_timer_event_source(game->battle->timer_enemy));
    al_register_event_source(game->queue, al_get_timer_event_source(game->timer_game_logs));
    al_register_event_source(game->queue, al_get_timer_event_source(game->timer_game_tips));

    game->game_font = font;
    game->title_font = title_font;
    game->subtitle_font = subtitle_font;
    game->log_font = al_load_ttf_font(path_log_font, 32, 0);
    game->log_font_20 = al_load_ttf_font(path_log_font, 20, 0);
    game->subtitle_8_font = al_load_ttf_font(path_subtitle_8_font, 8, 0);
    game->subtitle_11_font = al_load_ttf_font(path_subtitle_8_font, 11, 0);

    game->player->hp_heart = al_load_bitmap(path_heart);
    game->player->shield = al_load_bitmap(path_shield);
    game->player->sword_ui = al_load_bitmap(path_sword_ui);

    game->background = al_load_bitmap("assets/Menu_Design.png");
    
    game->controls = al_load_bitmap(path_key_e);
    game->puzzle_bhaskara_img = al_load_bitmap(path_quadro_bhaskara);
    game->puzzle_bhaskara_paper = al_load_bitmap(path_tabuleta_bhaskara);
    game->puzzle_bhaskara_paper_2 = al_load_bitmap(path_tabuleta_bhaskara_2);
    
    al_identity_transform(&game->camera_transform);
    
    
    game->log_ln1[0] = '\0';
    game->log_ln2[0] = '\0';
    game->log_ln3[0] = '\0';
    game->log_ln4[0] = '\0';

    load_item_database();
    initial_inventory(game); 

    // Item* HEAL_SMALL_POTION = create_item(2, "Poção Pequena", "Cura 10 HP.", 10, true, 3, ITEM_SMALL_HEAL);
    // add_item(&game->player->inventory, HEAL_SMALL_POTION, 3);

    // Item* HEAL_WATER = create_item(2, "Garrafa D'água", "Cura 5 HP.", 5, true, 5, ITEM_WATER);
    // add_item(&game->player->inventory, HEAL_WATER, 5);

    // player_equip_item(game->player, KEY_TO_MINOTAUR);
    // player_equip_item(game->player, KEY_TO_MEDUSA);
    player_equip_item(game->player, KEY_TO_ARAUTO);
    // player_equip_item(game->player, KEY_TO_SECOND_MAP);
    // player_equip_item(game->player, KEY_TO_THIRD_MAP);


    return game;
}

void resolve_collision_between_boxes(Entity* b1, Box* b2){
    float overlapLeft   = (b1->box.x + b1->box.w) - b2->x;
    float overlapRight  = (b2->x + b2->w) - b1->box.x;
    float overlapTop    = (b1->box.y + b1->box.h) - b2->y;
    float overlapBottom = (b2->y + b2->h) - b1->box.y;

    float minOverlapX = (overlapLeft < overlapRight) ? overlapLeft : overlapRight;
    float minOverlapY = (overlapTop < overlapBottom) ? overlapTop : overlapBottom;

    if (minOverlapX < minOverlapY) {
        if (overlapLeft < overlapRight) {
            b1->x -= overlapLeft;
        } else {
            b1->x += overlapRight;
        }
        b1->vx = 0;
    } else {
        if(overlapTop < overlapBottom){
            b1->y -= overlapTop;
            b1->vy = 0;
        } else {
            b1->y += overlapBottom;
            b1->vy = 0;
        }
    }

    update_hit_box(b1);
}

void check_map_collision(Entity* entity, Map* map) {
    for (int row = 0; row < MAP_HEIGHT; row++) {
        for (int col = 0; col < MAP_WIDTH; col++) {
            if (map->tiles[row][col] == TILE_WALL) {
                Box tile_box = get_tile_box(row, col);

                if (entity->box.x < tile_box.x + tile_box.w &&
                    entity->box.x + entity->box.w > tile_box.x &&
                    entity->box.y < tile_box.y + tile_box.h &&
                    entity->box.y + entity->box.h > tile_box.y)
                {
                    resolve_collision_between_boxes(entity, &tile_box);
                }
            }
        }
    }
}

void resolve_map_collision(Entity* entity, Map* map){
    if(entity->x > (MAP_WIDTH * TILE_W) - 50) entity->x = (MAP_WIDTH * TILE_W) - 50;
    if(entity->x < 0) entity->x = 0;
    if(entity->y > 490) entity->y = 490;
    else if(entity->y < 290) entity->y = 290;
}

void resolve_map_collision_battle(Entity* entity){
    if(entity->x > 800) entity->x = 800;
}

Entity* create_entity(const char* path, float x, float y, float scale, float frame, int cols, int rows, float dt, Entity_type entity_type){
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

void render_initial_level(Game* game){
    reset_world_entities(game);

    if (game->state != GAME_INIT) {
        if (game->background) {
            al_destroy_bitmap(game->background);
            game->background = NULL;
        }

        change_game_state(game, GAME_INIT);
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

void add_skeleton(Game* game, int x, int y, int attack, int defense){
    for(int i = 0; i < MAX_ENEMIES; i++){
        if(game->mobs[i] == NULL){
            game->mobs[i] = malloc(sizeof(Enemy));

            if (game->mobs[i] == NULL) {
                printf("ERRO: Falha ao alocar memória para o esqueleto!\n");
                return;
            }
            
            init_enemy(game->mobs[i], "Esqueleto", MOB, x, y, 5, attack, defense, 100, 0,0,0,0);
            set_entity_anim(&game->mobs[i]->entity, path_skeleton_idle, ANIM_IDLE, 11, 1, 0.1f);
            set_entity_anim(&game->mobs[i]->entity, path_skeleton_run, ANIM_RUN, 12, 1, 0.06f);
            set_entity_anim(&game->mobs[i]->entity, path_skeleton_attack, ANIM_ATTACK, 18, 1, 0.1f);
            set_entity_anim(&game->mobs[i]->entity, path_skeleton_hit, ANIM_HIT, 8, 1, 0.14f);
            set_entity_anim(&game->mobs[i]->entity, path_skeleton_death, ANIM_DEATH, 15, 1, 0.25f);
    
            game->mobs[i]->entity.flip = ALLEGRO_FLIP_HORIZONTAL;
            set_entity_scale(&game->mobs[i]->entity, 3.0);
    
            game->mobs[i]->hp_heart = al_load_bitmap(path_heart);
            game->mobs[i]->shield_enemy = al_load_bitmap(path_shield_enemy);
            
            game->world_enemies++;
            return;
        }
    }
}

void remove_all_mobs(Game* game){
    for(int i = 0; i < MAX_ENEMIES; i++){
        free(game->mobs[i]);
        game->mobs[i] = NULL;
    }
}

void log_tips(Game* game){
    if(game->can_draw == true) return;

    sprintf(game->log_ln1, "Você pode interagir com alguns itens do cenário");
    al_start_timer(game->timer_game_tips);

    game->can_draw = false;
}

/*
    Refatorar as funções de criar entidades e implementar posição relacionada ao eixo X (LEVEL_WIDTH * porcentagem)
*/
void load_first_map(Game* game) {
    game->world_enemies = 0;

    reset_world_entities(game);
    change_game_state(game, GAME_FIRST_MISSION);
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
    init_entity(bhaskara_frame, LEVEL_WIDTH * 0.05f, WALL_Y + 50, 0, 0, 1, EDUCATIONAL);
    set_entity_anim(bhaskara_frame, path_quadro_bhaskara, ANIM_IDLE, 1, 1, 0.1f);
    set_entity_scale(bhaskara_frame, 1.5);
    set_hit_box(bhaskara_frame, 0, 0, 0, 0);
    bhaskara_frame->puzzle_id = PUZZLE_BHASKARA;

    Entity* first_small_window = create_window(path_window_1, LEVEL_WIDTH * 0.13f, WALL_Y + 50, 2.0f);

    Door* minotaur_door = create_door(LEVEL_WIDTH * 0.22f, GROUND_Y, DOOR_MINOTAUR);
    minotaur_door->entity.is_locked_key = true;
    minotaur_door->entity.is_locked_puzzle = true;

    Entity* dial_1 = malloc(sizeof(Entity));
    init_entity(dial_1, minotaur_door->entity.x + 120, minotaur_door->entity.y, 0,0, 1, PUZZLE_DIAL);
    set_entity_anim(dial_1, path_roda, ANIM_IDLE, 1, 1, 0.1f);
    set_entity_scale(dial_1, 1.5);
    set_hit_box(dial_1, 0,0,0,0);
    dial_1->puzzle_id = PUZZLE_DIAL_1;

    Entity* dial_2 = malloc(sizeof(Entity));
    init_entity(dial_2, minotaur_door->entity.x + 240, minotaur_door->entity.y, 0,0, 1, PUZZLE_DIAL);
    set_entity_anim(dial_2, path_roda, ANIM_IDLE, 1, 1, 0.1f);
    set_entity_scale(dial_2, 1.5);
    set_hit_box(dial_2, 0,0,0,0);
    dial_2->puzzle_id = PUZZLE_DIAL_2;

    Entity* dial_3 = malloc(sizeof(Entity));
    init_entity(dial_3, minotaur_door->entity.x + 360, minotaur_door->entity.y, 0,0, 1, PUZZLE_DIAL);
    set_entity_anim(dial_3, path_roda, ANIM_IDLE, 1, 1, 0.1f);
    set_entity_scale(dial_3, 1.5);
    set_hit_box(dial_3, 0,0,0,0);
    dial_3->puzzle_id = PUZZLE_DIAL_3;

    // Entity* central_large_window = create_window(path_window_big, LEVEL_WIDTH * 0.30f, -15, 1.0f);
    // Entity* pi_banner_1 = create_banner(path_banner_pi, LEVEL_WIDTH * 0.26f, WALL_Y - 50, 0.5f, 1, 1);
    Entity* pi_banner_2 = create_banner(path_banner_pi, LEVEL_WIDTH * 0.34f, WALL_Y - 50, 0.5f, 1, 1);
    Entity* central_torch = create_torch(LEVEL_WIDTH * 0.40f, GROUND_Y);
    Entity* knight_armor_1 = create_entity(path_armadura_env, LEVEL_WIDTH * 0.42, WALL_Y + 50, 0.2f, 1, 1, 1, 0.1f, EDUCATIONAL);
    knight_armor_1->puzzle_id = PUZZLE_EQUATION_BHASKARA;
    
    Entity* knight_armor_2 = create_entity(path_armadura_env, LEVEL_WIDTH * 0.15, WALL_Y + 50, 0.2f, 1, 1, 1, 0.1f, EDUCATIONAL);

    Entity* knight_armor_3 = create_entity(path_armadura_env, LEVEL_WIDTH * 0.81, WALL_Y + 50, 0.2f, 1, 1, 1, 0.1f, EDUCATIONAL);

    Entity* knight_armor_4 = create_entity(path_armadura_env, LEVEL_WIDTH * 0.11, WALL_Y + 50, 0.2f, 1, 1, 1, 0.1f, EDUCATIONAL);

    minotaur_door->entity.puzzle_id = PUZZLE_DOOR_BHASKARA;
    
    add_skeleton(game, 1000, 500, 5, 5);
    add_skeleton(game, 2000, 400, 5, 5);
    add_skeleton(game, 3200, 450, 5, 5);

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
    add_world_entity(game, knight_armor_1);
    add_world_entity(game, knight_armor_2);
    add_world_entity(game, knight_armor_3);
    add_world_entity(game, knight_armor_4);
    add_world_entity(game, first_small_window);
    add_world_entity(game, &minotaur_door->entity);
    // add_world_entity(game, central_large_window);
    // add_world_entity(game, pi_banner_1);
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

    add_world_entity(game, dial_1);
    add_world_entity(game, dial_2);
    add_world_entity(game, dial_3);
}

void load_second_map(Game* game) {
    game->world_enemies = 0;

    for(int i = 0; i < MAX_ENEMIES; i++){
        game->mobs[i] = NULL;
    }

    reset_world_entities(game);
    change_game_state(game, GAME_SECOND_MISSION);
    game->gameplay_state = GAMEPLAY_EXPLORING;

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            game->map->tiles[y][x] = (y >= MAP_HEIGHT - 2) ? TILE_FLOOR : TILE_WALL;
        }
    }
    init_map(game->map, path_map_tile_cave_wall, path_map_tile_cave_floor, path_map_tile_cave_floor);

    const float LEVEL_WIDTH = 6000.0f;
    const float GROUND_Y = (SCREEN_H / 2.0f) - 98.0f;
    const float WALL_Y = (SCREEN_H / 2.0f) - 200.0f;

    Door* start_door = create_door(LEVEL_WIDTH * 0.1f, GROUND_Y, DOOR_RETURN);
    Door* medusa_door = create_door(LEVEL_WIDTH * 0.4f, GROUND_Y, DOOR_MEDUSA);
    medusa_door->entity.is_locked_key = true;
    medusa_door->entity.is_locked_puzzle = true;

    Entity* dial_1 = malloc(sizeof(Entity));
    init_entity(dial_1, medusa_door->entity.x + 120, medusa_door->entity.y, 0,0, 1, PUZZLE_DIAL);
    set_entity_anim(dial_1, path_roda, ANIM_IDLE, 1, 1, 0.1f);
    set_entity_scale(dial_1, 1.5);
    set_hit_box(dial_1, 0,0,0,0);
    dial_1->puzzle_id = PUZZLE_DIAL_1;

    Entity* dial_2 = malloc(sizeof(Entity));
    init_entity(dial_2, medusa_door->entity.x + 240, medusa_door->entity.y, 0,0, 1, PUZZLE_DIAL);
    set_entity_anim(dial_2, path_roda, ANIM_IDLE, 1, 1, 0.1f);
    set_entity_scale(dial_2, 1.5);
    set_hit_box(dial_2, 0,0,0,0);
    dial_2->puzzle_id = PUZZLE_DIAL_2;

    Entity* dial_3 = malloc(sizeof(Entity));
    init_entity(dial_3, medusa_door->entity.x + 360, medusa_door->entity.y, 0,0, 1, PUZZLE_DIAL);
    set_entity_anim(dial_3, path_roda, ANIM_IDLE, 1, 1, 0.1f);
    set_entity_scale(dial_3, 1.5);
    set_hit_box(dial_3, 0,0,0,0);
    dial_3->puzzle_id = PUZZLE_DIAL_3;

    Door* end_door = create_door(LEVEL_WIDTH * 0.9f, GROUND_Y, DOOR_NEXT_LEVEL);

    Entity* knight_armor_1 = create_entity(path_armadura_env, LEVEL_WIDTH * 0.22, WALL_Y + 50, 0.2f, 1, 1, 1, 0.1f, EDUCATIONAL);
    
    Entity* knight_armor_2 = create_entity(path_armadura_env, LEVEL_WIDTH * 0.15, WALL_Y + 50, 0.2f, 1, 1, 1, 0.1f, EDUCATIONAL);

    Entity* knight_armor_3 = create_entity(path_armadura_env, LEVEL_WIDTH * 0.81, WALL_Y + 50, 0.2f, 1, 1, 1, 0.1f, EDUCATIONAL);
    knight_armor_3->puzzle_id = PUZZLE_EQUATION_BHASKARA;

    Entity* knight_armor_4 = create_entity(path_armadura_env, LEVEL_WIDTH * 0.11, WALL_Y + 50, 0.2f, 1, 1, 1, 0.1f, EDUCATIONAL);

    game->player->entity.x = start_door->entity.x;

    add_skeleton(game, 1300, 500, 20, 20);
    add_skeleton(game, 2400, 500, 20, 20);
    add_skeleton(game, 3200, 500, 20, 20);

    Entity* left_torch = create_torch(start_door->entity.x - 120, start_door->entity.y);
    Entity* right_torch = create_torch(start_door->entity.x + 120, start_door->entity.y);
    
    Entity* window_1 = create_window(path_window_1, LEVEL_WIDTH * 0.25f, WALL_Y, 2.0f);
    Entity* center_window = create_window(path_window_big, LEVEL_WIDTH * 0.5f, WALL_Y - 170, 1.0f);
    Entity* window_2 = create_window(path_window_1, LEVEL_WIDTH * 0.75f, WALL_Y, 2.0f);

    add_world_entity(game, &start_door->entity);
    add_world_entity(game, knight_armor_1);
    add_world_entity(game, knight_armor_2);
    add_world_entity(game, knight_armor_3);
    add_world_entity(game, knight_armor_4);
    add_world_entity(game, &medusa_door->entity);
    add_world_entity(game, &end_door->entity);
    add_world_entity(game, left_torch);
    add_world_entity(game, right_torch);
    add_world_entity(game, window_1);
    add_world_entity(game, center_window);
    add_world_entity(game, window_2);
    add_world_entity(game, dial_1);
    add_world_entity(game, dial_2);
    add_world_entity(game, dial_3);
}

void load_third_map(Game* game) {
    game->world_enemies = 0;

    for(int i = 0; i < MAX_ENEMIES; i++){
        game->mobs[i] = NULL;
    }

    reset_world_entities(game);
    change_game_state(game, GAME_THIRD_MISSION);
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
    Door* easter_egg_door = create_door(LEVEL_WIDTH * 0.4f, GROUND_Y, DOOR_EASTER_EGG);
    game->player->entity.x = start_door->entity.x;

    Entity* entry_torch = create_torch(LEVEL_WIDTH * 0.06f, GROUND_Y);

    add_skeleton(game, 1220, 500, 50, 50);
    add_skeleton(game, 1220, 500, 50, 50);
    add_skeleton(game, 2400, 500, 50, 50);
    
    Entity* bhaskara_frame = malloc(sizeof(Entity));
    init_entity(bhaskara_frame, LEVEL_WIDTH * 0.09f, WALL_Y, 0, 0, 1, ENVIRONMENT_NO_MOVE);
    set_entity_anim(bhaskara_frame, path_quadro_bhaskara, ANIM_IDLE, 1, 1, 0.1f);
    set_entity_scale(bhaskara_frame, 1.5);
    set_hit_box(bhaskara_frame, 0, 0, 0, 0);

    Entity* corridor_window_1 = create_window(path_window_1, LEVEL_WIDTH * 0.15f, WALL_Y + 50, 2.0f);
    Entity* corridor_banner_1 = create_banner(path_banner_e, LEVEL_WIDTH * 0.25f, WALL_Y, 0.3f, 1, 1);
    Entity* corridor_window_2 = create_window(path_window_1, LEVEL_WIDTH * 0.35f, WALL_Y + 50, 2.0f);
    Entity* corridor_banner_2 = create_banner(path_banner_e, LEVEL_WIDTH * 0.45f, WALL_Y, 0.3f, 1, 1);

    Entity* hall_banner_1 = create_banner(path_banner_pi, LEVEL_WIDTH * 0.55f, WALL_Y - 50, 0.5f, 1, 1);
    Entity* hall_large_window = create_window(path_window_big, LEVEL_WIDTH * 0.65f, -15, 1.0f);
    Entity* hall_banner_2 = create_banner(path_banner_pi, LEVEL_WIDTH * 0.75f, WALL_Y - 50, 0.5f, 1, 1);

    Door* final_door = create_door(LEVEL_WIDTH * 0.95f, GROUND_Y, DOOR_ARAUTO);
    final_door->entity.is_locked_key = true;
    final_door->entity.is_locked_puzzle = true;

    Entity* dial_1 = malloc(sizeof(Entity));
    init_entity(dial_1, final_door->entity.x + 120, final_door->entity.y, 0,0, 1, PUZZLE_DIAL);
    set_entity_anim(dial_1, path_roda, ANIM_IDLE, 1, 1, 0.1f);
    set_entity_scale(dial_1, 1.5);
    set_hit_box(dial_1, 0,0,0,0);
    dial_1->puzzle_id = PUZZLE_DIAL_1;

    Entity* dial_2 = malloc(sizeof(Entity));
    init_entity(dial_2, final_door->entity.x + 240, final_door->entity.y, 0,0, 1, PUZZLE_DIAL);
    set_entity_anim(dial_2, path_roda, ANIM_IDLE, 1, 1, 0.1f);
    set_entity_scale(dial_2, 1.5);
    set_hit_box(dial_2, 0,0,0,0);
    dial_2->puzzle_id = PUZZLE_DIAL_2;

    Entity* dial_3 = malloc(sizeof(Entity));
    init_entity(dial_3, final_door->entity.x + 360, final_door->entity.y, 0,0, 1, PUZZLE_DIAL);
    set_entity_anim(dial_3, path_roda, ANIM_IDLE, 1, 1, 0.1f);
    set_entity_scale(dial_3, 1.5);
    set_hit_box(dial_3, 0,0,0,0);
    dial_3->puzzle_id = PUZZLE_DIAL_3;

    Entity* final_banner_left = create_banner(path_banner, final_door->entity.x - 200, final_door->entity.y - 100, 0.5f, 6, 1);
    Entity* final_torch_left = create_torch(final_door->entity.x - 120, final_door->entity.y);
    Entity* final_torch_right = create_torch(final_door->entity.x + 120, final_door->entity.y);
    Entity* final_banner_right = create_banner(path_banner, final_door->entity.x + 200, final_door->entity.y - 100, 0.5f, 6, 1);

    Entity* knight_armor_1 = create_entity(path_armadura_env, LEVEL_WIDTH * 0.42, WALL_Y + 50, 0.2f, 1, 1, 1, 0.1f, EDUCATIONAL);
    
    Entity* knight_armor_2 = create_entity(path_armadura_env, LEVEL_WIDTH * 0.15, WALL_Y + 50, 0.2f, 1, 1, 1, 0.1f, EDUCATIONAL);

    Entity* knight_armor_3 = create_entity(path_armadura_env, LEVEL_WIDTH * 0.81, WALL_Y + 50, 0.2f, 1, 1, 1, 0.1f, EDUCATIONAL);
    knight_armor_3->puzzle_id = PUZZLE_EQUATION_BHASKARA;

    Entity* knight_armor_4 = create_entity(path_armadura_env, LEVEL_WIDTH * 0.11, WALL_Y + 50, 0.2f, 1, 1, 1, 0.1f, EDUCATIONAL);

    add_world_entity(game, &start_door->entity);
    add_world_entity(game, &easter_egg_door->entity);
    add_world_entity(game, entry_torch);
    add_world_entity(game, bhaskara_frame);
    add_world_entity(game, corridor_window_1);
    add_world_entity(game, corridor_banner_1);
    add_world_entity(game, corridor_window_2);
    add_world_entity(game, corridor_banner_2);
    add_world_entity(game, hall_banner_1);
    add_world_entity(game, hall_large_window);
    add_world_entity(game, hall_banner_2);
    add_world_entity(game, &final_door->entity);
    add_world_entity(game, final_banner_left);
    add_world_entity(game, final_torch_left);
    add_world_entity(game, final_torch_right);
    add_world_entity(game, final_banner_right);
    add_world_entity(game, dial_1);
    add_world_entity(game, dial_2);
    add_world_entity(game, dial_3);
    add_world_entity(game, knight_armor_1);
    add_world_entity(game, knight_armor_2);
    add_world_entity(game, knight_armor_3);
    add_world_entity(game, knight_armor_4);
}

void render_minotaur_level(Game* game){
    reset_world_entities(game);

    game->background = al_load_bitmap("assets/sprites/map/minotaur/background-minotaur.png");

    game->enemy = malloc(sizeof(Enemy));

    init_enemy(game->enemy, "Minotauro de arquimedes", MINOTAUR, 700, 405, 5, 10, 25, 150, 30,0,0,0);
    set_entity_anim(&game->enemy->entity, path_minotaur_idle, ANIM_IDLE, 10, 1, 0.1f);
    set_entity_anim(&game->enemy->entity, path_minotaur_run, ANIM_RUN, 12, 1, 0.06f);
    set_entity_anim(&game->enemy->entity, path_minotaur_attack, ANIM_ATTACK, 5, 1, 0.1f);
    set_entity_anim(&game->enemy->entity, path_minotaur_hit, ANIM_HIT, 3, 1, 0.14f);
    set_entity_anim(&game->enemy->entity, path_minotaur_death, ANIM_DEATH, 5, 1, 0.25f);

    game->enemy->entity.flip = ALLEGRO_FLIP_HORIZONTAL;
    game->enemy->portrait = al_load_bitmap(path_portrait_minotaur);
    set_entity_scale(&game->enemy->entity, 2.0);

    game->enemy->hp_heart = al_load_bitmap(path_heart);
    game->enemy->shield_enemy = al_load_bitmap(path_shield_enemy);
    game->enemy->hp_canva = al_load_bitmap(path_hp_canva_minotaur);
    game->battle->dialogue_sprite = al_load_bitmap(path_dialogue_box_battle);

    game->player->entity.x = 200; 
    game->player->entity.flip = 0;

    game->player->entity.y = 517; 

    start_battle(game->battle, game->player, game->enemy);

    game->enemy->entity.y = 405;

    game->battle->state = BATTLE_DIALOGUE;

    game->battle->dialogues = 0;

}

void render_medusa_level(Game* game){
    game->background = al_load_bitmap("assets/sprites/map/medusa/background-medusa.png");
    reset_world_entities(game);

    game->enemy = malloc(sizeof(Enemy));

    init_enemy(game->enemy, "Medusa de Hipátia", MEDUSA, 800, 400, 5, 50, 50, 200, 0,0,0,0);
    set_entity_anim(&game->enemy->entity, path_medusa_idle, ANIM_IDLE, 7, 1, 0.1f);
    set_entity_anim(&game->enemy->entity, path_medusa_walk, ANIM_RUN, 13, 1, 0.06f);
    set_entity_anim(&game->enemy->entity, path_medusa_attack, ANIM_ATTACK, 16, 1, 0.1f);
    set_entity_anim(&game->enemy->entity, path_medusa_hit, ANIM_HIT, 3, 1, 0.1f);
    set_entity_anim(&game->enemy->entity, path_medusa_dead, ANIM_DEATH, 3, 1, 0.25f);

    game->enemy->entity.flip = ALLEGRO_FLIP_HORIZONTAL;
    game->enemy->portrait = al_load_bitmap(path_portrait_medusa);
    set_entity_scale(&game->enemy->entity, 2.0);
    
    game->enemy->hp_heart = al_load_bitmap(path_heart);
    game->enemy->shield_enemy = al_load_bitmap(path_shield_enemy);
    game->enemy->hp_canva = al_load_bitmap(path_hp_canva_minotaur);
    game->battle->dialogue_sprite = al_load_bitmap(path_dialogue_box_battle);

    game->player->entity.x = 200; 
    game->player->entity.flip = 0;

    game->player->entity.y = 517; 

    start_battle(game->battle, game->player, game->enemy);

    game->enemy->entity.y = game->player->entity.y - 100;

    game->battle->state = BATTLE_DIALOGUE;

    game->battle->dialogues = DIALOGUE_MEDUSA_1;

}

void render_arauto_level(Game* game){
    game->background = al_load_bitmap("assets/sprites/map/arauto/background-arauto.png");
    reset_world_entities(game);

    game->enemy = malloc(sizeof(Enemy));

    init_enemy(game->enemy, "Arauto", ARAUTO, 800, 400, 5, 100, 100, 500, 0,0,0,0);
    set_entity_anim(&game->enemy->entity, path_arauto_idle, ANIM_IDLE, 5, 1, 0.1f);
    set_entity_anim(&game->enemy->entity, path_arauto_walk, ANIM_RUN, 8, 1, 0.06f);
    set_entity_anim(&game->enemy->entity, path_arauto_attack, ANIM_ATTACK, 6, 1, 0.1f);
    set_entity_anim(&game->enemy->entity, path_arauto_hit, ANIM_HIT, 3, 1, 0.1f);
    set_entity_anim(&game->enemy->entity, path_arauto_dead, ANIM_DEATH, 6, 1, 0.1f);

    game->enemy->entity.flip = ALLEGRO_FLIP_HORIZONTAL;
    game->enemy->portrait = al_load_bitmap(path_portrait_arauto);
    set_entity_scale(&game->enemy->entity, 2.0);
    
    game->enemy->hp_heart = al_load_bitmap(path_heart);
    game->enemy->shield_enemy = al_load_bitmap(path_shield_enemy);
    game->enemy->hp_canva = al_load_bitmap(path_hp_canva_arauto);
    game->battle->dialogue_sprite = al_load_bitmap(path_dialogue_box_battle);

    game->player->entity.x = 200; 
    game->player->entity.flip = 0;

    game->player->entity.y = 517; 

    start_battle(game->battle, game->player, game->enemy);

    game->enemy->entity.y = game->player->entity.y - 200;

    game->battle->state = BATTLE_DIALOGUE;

    game->battle->dialogues = DIALOGUE_ARAUTO_1;


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
    

    al_draw_filled_rectangle(0, 0, SCREEN_W, SCREEN_H, al_map_rgba(0, 0, 0, 100));

    if(game->battle->state == BATTLE_DIALOGUE){
        switch (game->battle->dialogues)
        {
        case DIALOGUE_BATTLE_0:
            al_draw_scaled_bitmap(
                game->battle->dialogue_sprite,
                0,0,
                177, 90,
                SCREEN_W - 700, 0,
                700, 350,
                0
            );
            
            al_draw_scaled_bitmap(
                game->enemy->portrait,
                0,0,
                520, 500,
                SCREEN_W - 660, 80,
                215, 212,
                0
            );

            al_draw_textf(game->subtitle_11_font, al_map_rgb(54, 16, 4),  SCREEN_W - 375, SCREEN_H / 2 - 300, ALLEGRO_ALIGN_LEFT, "%s:", game->enemy->name);
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 250, ALLEGRO_ALIGN_LEFT, "Você ousa adentrar");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W -  400, SCREEN_H / 2 - 225, ALLEGRO_ALIGN_LEFT, "o labirinto do conhecimento?");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54,16, 4), SCREEN_W - 400, SCREEN_H / 2 - 200, ALLEGRO_ALIGN_LEFT, "Sou o Minotauro,");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54,16, 4), SCREEN_W - 400, SCREEN_H / 2 - 175, ALLEGRO_ALIGN_LEFT, "forjado pelos cálculos");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54,16, 4), SCREEN_W - 400, SCREEN_H / 2 - 150, ALLEGRO_ALIGN_LEFT, "e sabedoria de Arquimedes.");
            
            
            break;
        case DIALOGUE_BATTLE_1:
            al_draw_scaled_bitmap(
                game->battle->dialogue_sprite,
                0,0,
                177, 90,
                SCREEN_W - 700, 0,
                700, 350,
                0
            );

            al_draw_scaled_bitmap(
                game->player->portrait,
                0,0,
                520, 596,
                SCREEN_W - 660, 50,
                215, 246,
                0
            );

            al_draw_textf(game->subtitle_11_font, al_map_rgb(54, 16, 4),  SCREEN_W - 375, SCREEN_H / 2 - 300, ALLEGRO_ALIGN_LEFT, "%s:", game->player->name);
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 250, ALLEGRO_ALIGN_LEFT, "Não busco confronto,");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W -  400, SCREEN_H / 2 - 225, ALLEGRO_ALIGN_LEFT, "mas sim entendimento.");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54,16, 4), SCREEN_W - 400, SCREEN_H / 2 - 200, ALLEGRO_ALIGN_LEFT, "Vim desvendar");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54,16, 4), SCREEN_W - 400, SCREEN_H / 2 - 175, ALLEGRO_ALIGN_LEFT, "os segredos da matemática.");
            break;
        case DIALOGUE_BATTLE_2:
            

            al_draw_scaled_bitmap(
                game->battle->dialogue_sprite,
                0,0,
                177, 90,
                SCREEN_W - 700, 0,
                700, 350,
                0
            );

            
            al_draw_scaled_bitmap(
                game->enemy->portrait,
                0,0,
                520, 500,
                SCREEN_W - 660, 80,
                215, 212,
                0
            );

            al_draw_textf(game->subtitle_11_font, al_map_rgb(54, 16, 4),  SCREEN_W - 375, SCREEN_H / 2 - 300, ALLEGRO_ALIGN_LEFT, "%s:", game->enemy->name);
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 250, ALLEGRO_ALIGN_LEFT, "Muitos tentaram o mesmo caminho");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W -  400, SCREEN_H / 2 - 225, ALLEGRO_ALIGN_LEFT, "e o final foi trágico,");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54,16, 4), SCREEN_W - 400, SCREEN_H / 2 - 200, ALLEGRO_ALIGN_LEFT, "não será você que vai conseguir.");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54,16, 4), SCREEN_W - 400, SCREEN_H / 2 - 175, ALLEGRO_ALIGN_LEFT, "Me responda, jovem! Está pronto");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54,16, 4), SCREEN_W - 400, SCREEN_H / 2 - 150, ALLEGRO_ALIGN_LEFT, "para raciocinar sob pressão,");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54,16, 4), SCREEN_W - 400, SCREEN_H / 2 - 125, ALLEGRO_ALIGN_LEFT, "ou será mais uma alma");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54,16, 4), SCREEN_W - 400, SCREEN_H / 2 - 100, ALLEGRO_ALIGN_LEFT, "perdida e insignificante?");

            break;
        case DIALOGUE_BATTLE_3:
            al_draw_scaled_bitmap(
                game->battle->dialogue_sprite,
                0,0,
                177, 90,
                SCREEN_W - 700, 0,
                700, 350,
                0
            );

            al_draw_scaled_bitmap(
                game->player->portrait,
                0,0,
                520, 596,
                SCREEN_W - 660, 50,
                215, 246,
                0
            );

            al_draw_textf(game->subtitle_11_font, al_map_rgb(54, 16, 4),  SCREEN_W - 375, SCREEN_H / 2 - 300, ALLEGRO_ALIGN_LEFT, "%s:", game->player->name);
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 250, ALLEGRO_ALIGN_LEFT, "Não temo desafios!");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W -  400, SCREEN_H / 2 - 225, ALLEGRO_ALIGN_LEFT, "Cada obstáculo que venço");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54,16, 4), SCREEN_W - 400, SCREEN_H / 2 - 200, ALLEGRO_ALIGN_LEFT, "me leva mais perto do que busco.");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54,16, 4), SCREEN_W - 400, SCREEN_H / 2 - 175, ALLEGRO_ALIGN_LEFT, "Se preciso provar meu valor,");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54,16, 4), SCREEN_W - 400, SCREEN_H / 2 - 150, ALLEGRO_ALIGN_LEFT, "que assim seja. Estou pronto");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54,16, 4), SCREEN_W - 400, SCREEN_H / 2 - 125, ALLEGRO_ALIGN_LEFT, "e será uma honra");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54,16, 4), SCREEN_W - 400, SCREEN_H / 2 - 100, ALLEGRO_ALIGN_LEFT, "enfrentar um grande gênio");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54,16, 4), SCREEN_W - 400, SCREEN_H / 2 - 75, ALLEGRO_ALIGN_LEFT, "como você Arquimedes.");
            
            break;
        case DIALOGUE_BATTLE_4:
            al_draw_scaled_bitmap(
                game->battle->dialogue_sprite,
                0,0,
                177, 90,
                SCREEN_W - 700, 0,
                700, 350,
                0
            );

            al_draw_scaled_bitmap(
                game->enemy->portrait,
                0,0,
                520, 500,
                SCREEN_W - 660, 80,
                215, 212,
                0
            );

            al_draw_textf(game->subtitle_11_font, al_map_rgb(54, 16, 4),  SCREEN_W - 375, SCREEN_H / 2 - 300, ALLEGRO_ALIGN_LEFT, "%s:", game->enemy->name);
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 250, ALLEGRO_ALIGN_LEFT, "Então prepare-se!");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W -  400, SCREEN_H / 2 - 225, ALLEGRO_ALIGN_LEFT, "Apenas quem domina");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54,16, 4), SCREEN_W - 400, SCREEN_H / 2 - 200, ALLEGRO_ALIGN_LEFT, "o equilíbrio entre força");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54,16, 4), SCREEN_W - 400, SCREEN_H / 2 - 175, ALLEGRO_ALIGN_LEFT, "e lógica merece avançar.");
            
            break;
        case DIALOGUE_BATTLE_FINAL_INTRO:
            al_draw_text(game->subtitle_11_font, al_map_rgb(255, 255, 0), 70, 20, ALLEGRO_ALIGN_CENTER, "Pressione");
            al_draw_scaled_bitmap(
                    game->controls,
                    0, 0,
                    16, 16,
                    130, 12,
                    32, 32,
                    0 
                );   
            al_draw_text(game->subtitle_11_font, al_map_rgb(255, 255, 0), 300, 20, ALLEGRO_ALIGN_CENTER, "para começar a batalha!");
   
            
            break;
        case DIALOGUE_AFTER_TRASH_TALK:
            al_draw_text(game->subtitle_11_font, al_map_rgb(255, 255, 0), 70, 20, ALLEGRO_ALIGN_CENTER, "Pressione");
            al_draw_scaled_bitmap(
                    game->controls,
                    0, 0,
                    16, 16,
                    130, 12,
                    32, 32,
                    0 
                );   
            al_draw_text(game->subtitle_11_font, al_map_rgb(255, 255, 0), 300, 20, ALLEGRO_ALIGN_CENTER, "para retomar a batalha!");
   
            
            break;
        case DIALOGUE_BATTLE_5:
            al_draw_scaled_bitmap(
                game->battle->dialogue_sprite,
                0,0,
                177, 90,
                SCREEN_W - 700, 0,
                700, 350,
                0
            );

            al_draw_scaled_bitmap(
                game->enemy->portrait,
                0,0,
                520, 500,
                SCREEN_W - 660, 80,
                215, 212,
                0
            );

            al_draw_textf(game->subtitle_11_font, al_map_rgb(54, 16, 4),  SCREEN_W - 375, SCREEN_H / 2 - 300, ALLEGRO_ALIGN_LEFT, "%s:", game->enemy->name);
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 175, ALLEGRO_ALIGN_LEFT, "Você é fraco e sem conhecimento");
            
            break;
        case DIALOGUE_BATTLE_6:
            al_draw_scaled_bitmap(
                game->battle->dialogue_sprite,
                0,0,
                177, 90,
                SCREEN_W - 700, 0,
                700, 350,
                0
            );

            al_draw_scaled_bitmap(
                game->enemy->portrait,
                0,0,
                520, 500,
                SCREEN_W - 660, 80,
                215, 212,
                0
            );

            al_draw_textf(game->subtitle_11_font, al_map_rgb(54, 16, 4),  SCREEN_W - 375, SCREEN_H / 2 - 300, ALLEGRO_ALIGN_LEFT, "%s:", game->enemy->name);
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 175, ALLEGRO_ALIGN_LEFT, "Sua lógica é tão pífia");
            
            break;

        case DIALOGUE_BATTLE_7:
            al_draw_scaled_bitmap(
                game->battle->dialogue_sprite,
                0,0,
                177, 90,
                SCREEN_W - 700, 0,
                700, 350,
                0
            );

            al_draw_scaled_bitmap(
                game->enemy->portrait,
                0,0,
                520, 500,
                SCREEN_W - 660, 80,
                215, 212,
                0
            );

            al_draw_textf(game->subtitle_11_font, al_map_rgb(54, 16, 4),  SCREEN_W - 375, SCREEN_H / 2 - 300, ALLEGRO_ALIGN_LEFT, "%s:", game->enemy->name);
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 175, ALLEGRO_ALIGN_LEFT, "Suas habilidades e seu conhecimento");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 150, ALLEGRO_ALIGN_LEFT, "é igual a zero!");

            break;
        default:
            break;
        }
    } else {
        al_draw_scaled_bitmap(
            game->enemy->hp_canva,
            0, 0,
            895, 470,
            SCREEN_W - 380, 20,
            324, 170,
            0
        );
    }

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

    al_draw_filled_rectangle(0, 0, SCREEN_W, SCREEN_H, al_map_rgba(0, 0, 0, 100));

    if(game->battle->state == BATTLE_DIALOGUE){
        switch (game->battle->dialogues)
        {

        case DIALOGUE_MEDUSA_1:

            al_draw_scaled_bitmap(game->battle->dialogue_sprite, 0,0, 177, 90, SCREEN_W - 700, 0, 700, 350, 0);

            al_draw_scaled_bitmap(
                game->enemy->portrait,
                0,0,
                520, 500,
                SCREEN_W - 660, 80,
                215, 212,
                0
            );

            al_draw_textf(game->subtitle_11_font, al_map_rgb(54, 16, 4),  SCREEN_W - 375, SCREEN_H / 2 - 300, ALLEGRO_ALIGN_LEFT, "%s:", game->enemy->name);
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 250, ALLEGRO_ALIGN_LEFT, "Bem-vindo, forasteiro. Não tema");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 225, ALLEGRO_ALIGN_LEFT, "meus olhos. Sou Medusa, a");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 200, ALLEGRO_ALIGN_LEFT, "manifestação do poder racional");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 175, ALLEGRO_ALIGN_LEFT, "de Hipátia. Aqui, a razão é tão");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 150, ALLEGRO_ALIGN_LEFT, "cortante quanto a lâmina de Atena.");

            break;

        case DIALOGUE_MEDUSA_2:

            al_draw_scaled_bitmap(game->battle->dialogue_sprite, 0,0, 177, 90, SCREEN_W - 700, 0, 700, 350, 0);

            al_draw_scaled_bitmap(game->player->portrait, 0,0, 520, 596, SCREEN_W - 660, 50, 215, 246, 0); 
            
            al_draw_textf(game->subtitle_11_font, al_map_rgb(54, 16, 4),  SCREEN_W - 375, SCREEN_H / 2 - 300, ALLEGRO_ALIGN_LEFT, "%s:", game->player->name);
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 250, ALLEGRO_ALIGN_LEFT, "Então você é minha próxima");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 225, ALLEGRO_ALIGN_LEFT, "adversária. Será uma honra");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 200, ALLEGRO_ALIGN_LEFT, "enfrentá-la, pois a batalha");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 175, ALLEGRO_ALIGN_LEFT, "contra o Minotauro de");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 150, ALLEGRO_ALIGN_LEFT, "Arquimedes me fez aprender muito.");

            break;

        case DIALOGUE_MEDUSA_3:

            al_draw_scaled_bitmap(game->battle->dialogue_sprite, 0,0, 177, 90, SCREEN_W - 700, 0, 700, 350, 0);

            al_draw_scaled_bitmap(
                game->enemy->portrait,
                0,0,
                520, 500,
                SCREEN_W - 660, 80,
                215, 212,
                0
            );

            al_draw_textf(game->subtitle_11_font, al_map_rgb(54, 16, 4),  SCREEN_W - 375, SCREEN_H / 2 - 300, ALLEGRO_ALIGN_LEFT, "%s:", game->enemy->name);
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 250, ALLEGRO_ALIGN_LEFT, "Hum... Então você o derrotou?");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 225, ALLEGRO_ALIGN_LEFT, "Mas comigo será diferente.");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 200, ALLEGRO_ALIGN_LEFT, "Jamais perderia para alguém que");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 175, ALLEGRO_ALIGN_LEFT, "não tem o mínimo de inteligência.");

            break;

        case DIALOGUE_MEDUSA_4:

            al_draw_scaled_bitmap(game->battle->dialogue_sprite, 0,0, 177, 90, SCREEN_W - 700, 0, 700, 350, 0);
            al_draw_scaled_bitmap(game->player->portrait, 0,0, 520, 596, SCREEN_W - 660, 50, 215, 246, 0); // Retrato do Player

            al_draw_textf(game->subtitle_11_font, al_map_rgb(54, 16, 4),  SCREEN_W - 375, SCREEN_H / 2 - 300, ALLEGRO_ALIGN_LEFT, "%s:", game->player->name);
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 250, ALLEGRO_ALIGN_LEFT, "Até agora, vocês se mostraram");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 225, ALLEGRO_ALIGN_LEFT, "muito arrogantes, mas vou");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 200, ALLEGRO_ALIGN_LEFT, "mostrar meu potencial e derrotá-la");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 175, ALLEGRO_ALIGN_LEFT, "assim como derrotei Arquimedes.");

            break;

        case DIALOGUE_MEDUSA_5:

            al_draw_scaled_bitmap(game->battle->dialogue_sprite, 0,0, 177, 90, SCREEN_W - 700, 0, 700, 350, 0);

            al_draw_scaled_bitmap(
                game->enemy->portrait,
                0,0,
                520, 500,
                SCREEN_W - 660, 80,
                215, 212,
                0
            );

            al_draw_textf(game->subtitle_11_font, al_map_rgb(54, 16, 4),  SCREEN_W - 375, SCREEN_H / 2 - 300, ALLEGRO_ALIGN_LEFT, "%s:", game->enemy->name);
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 250, ALLEGRO_ALIGN_LEFT, "Então prepare-se, jovem.");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 225, ALLEGRO_ALIGN_LEFT, "Além de derrotá-lo e provar que");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 200, ALLEGRO_ALIGN_LEFT, "você não é capaz de obter o");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 175, ALLEGRO_ALIGN_LEFT, "conhecimento absoluto da");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 150, ALLEGRO_ALIGN_LEFT, "matemática, irei transformá-lo");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 125, ALLEGRO_ALIGN_LEFT, "em pedra com o meu olhar");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 100, ALLEGRO_ALIGN_LEFT, "hipnotizante.");

            break;
        case DIALOGUE_MEDUSA_FINAL_1:

            al_draw_scaled_bitmap(game->battle->dialogue_sprite, 0,0, 177, 90, SCREEN_W - 700, 0, 700, 350, 0);

            al_draw_scaled_bitmap(
                game->enemy->portrait,
                0,0,
                520, 500,
                SCREEN_W - 660, 80,
                215, 212,
                0
            );

            al_draw_textf(game->subtitle_11_font, al_map_rgb(54, 16, 4),  SCREEN_W - 375, SCREEN_H / 2 - 300, ALLEGRO_ALIGN_LEFT, "%s:", game->enemy->name);
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 250, ALLEGRO_ALIGN_LEFT, "Inacreditável... você me superou.");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 225, ALLEGRO_ALIGN_LEFT, "Não pense, porém, que isso faz");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 200, ALLEGRO_ALIGN_LEFT, "de você alguém digno do saber");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 175, ALLEGRO_ALIGN_LEFT, "absoluto! A arrogância que percebo");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 150, ALLEGRO_ALIGN_LEFT, "em sua voz será sua ruína nos");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 125, ALLEGRO_ALIGN_LEFT, "próximos desafios.");

            break;

        case DIALOGUE_MEDUSA_FINAL_2:

            al_draw_scaled_bitmap(game->battle->dialogue_sprite, 0,0, 177, 90, SCREEN_W - 700, 0, 700, 350, 0);

            al_draw_scaled_bitmap(
                game->enemy->portrait,
                0,0,
                520, 500,
                SCREEN_W - 660, 80,
                215, 212,
                0
            );

            al_draw_textf(game->subtitle_11_font, al_map_rgb(54, 16, 4),  SCREEN_W - 375, SCREEN_H / 2 - 300, ALLEGRO_ALIGN_LEFT, "%s:", game->enemy->name);
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 250, ALLEGRO_ALIGN_LEFT, "Eu fui derrotada aqui, mas o Arauto");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 225, ALLEGRO_ALIGN_LEFT, "não tem piedade e nem hesitará");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 200, ALLEGRO_ALIGN_LEFT, "em esmagar quem subestima o seu");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 175, ALLEGRO_ALIGN_LEFT, "território.");

            break;

        case DIALOGUE_MEDUSA_FINAL_3:

            al_draw_scaled_bitmap(game->battle->dialogue_sprite, 0,0, 177, 90, SCREEN_W - 700, 0, 700, 350, 0);

            al_draw_scaled_bitmap(
                game->enemy->portrait,
                0,0,
                520, 500,
                SCREEN_W - 660, 80,
                215, 212,
                0
            );

            al_draw_textf(game->subtitle_11_font, al_map_rgb(54, 16, 4),  SCREEN_W - 375, SCREEN_H / 2 - 300, ALLEGRO_ALIGN_LEFT, "%s:", game->enemy->name);
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 250, ALLEGRO_ALIGN_LEFT, "Veremos o quanto essa determinação");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 225, ALLEGRO_ALIGN_LEFT, "resiste diante dos verdadeiros");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 200, ALLEGRO_ALIGN_LEFT, "horrores que o Arauto reserva.");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 175, ALLEGRO_ALIGN_LEFT, "Siga em frente, estudante ousado,");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 150, ALLEGRO_ALIGN_LEFT, "mas lembre-se: o próximo erro");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 125, ALLEGRO_ALIGN_LEFT, "poderá ser o último nesse reino");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 100, ALLEGRO_ALIGN_LEFT, "de enigmas devoradores de");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 75, ALLEGRO_ALIGN_LEFT, "arrogantes e ignorantes!.");

            break;
        case DIALOGUE_MEDUSA_RANDOM_1:

            al_draw_scaled_bitmap(
                game->enemy->portrait,
                0,0,
                520, 500,
                SCREEN_W - 660, 80,
                215, 212,
                0
            );

            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 175, ALLEGRO_ALIGN_LEFT, "Nem te olhei ainda e já está");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 150, ALLEGRO_ALIGN_LEFT, "paralisado... deve ser medo!");
            break;

        case DIALOGUE_MEDUSA_RANDOM_2:
            al_draw_scaled_bitmap(
                game->enemy->portrait,
                0,0,
                520, 500,
                SCREEN_W - 660, 80,
                215, 212,
                0
            );
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 175, ALLEGRO_ALIGN_LEFT, "Você me cansa com esse");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 150, ALLEGRO_ALIGN_LEFT, "conhecimento insignificante.");

            break;

        case DIALOGUE_MEDUSA_RANDOM_3:

            al_draw_scaled_bitmap(
                game->enemy->portrait,
                0,0,
                520, 500,
                SCREEN_W - 660, 80,
                215, 212,
                0
            );

            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 175, ALLEGRO_ALIGN_LEFT, "Se quer conquistar conhecimento");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 150, ALLEGRO_ALIGN_LEFT, "absoluto, enfrente a dúvida");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 125, ALLEGRO_ALIGN_LEFT, "sem medo!");

            break;
        case DIALOGUE_BATTLE_FINAL_INTRO:
            al_draw_text(game->subtitle_11_font, al_map_rgb(255, 255, 0), 70, 20, ALLEGRO_ALIGN_CENTER, "Pressione");
            al_draw_scaled_bitmap(
                    game->controls,
                    0, 0,
                    16, 16,
                    130, 12,
                    32, 32,
                    0 
                );   
            al_draw_text(game->subtitle_11_font, al_map_rgb(255, 255, 0), 300, 20, ALLEGRO_ALIGN_CENTER, "para começar a batalha!");
   
            
            break;
        case DIALOGUE_AFTER_TRASH_TALK:
            al_draw_text(game->subtitle_11_font, al_map_rgb(255, 255, 0), 70, 20, ALLEGRO_ALIGN_CENTER, "Pressione");
            al_draw_scaled_bitmap(
                    game->controls,
                    0, 0,
                    16, 16,
                    130, 12,
                    32, 32,
                    0 
                );   
            al_draw_text(game->subtitle_11_font, al_map_rgb(255, 255, 0), 300, 20, ALLEGRO_ALIGN_CENTER, "para retomar a batalha!");
   
            
            break;
        default:
            break;
        }

    } else {
        al_draw_scaled_bitmap(
            game->enemy->hp_canva,
            0, 0,
            895, 470,
            SCREEN_W - 380, 20,
            324, 170,
            0
        );
    }

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

    al_draw_filled_rectangle(0, 0, SCREEN_W, SCREEN_H, al_map_rgba(0, 0, 0, 100));

    if(game->battle->state == BATTLE_DIALOGUE){
        switch (game->battle->dialogues)
        {
        case DIALOGUE_ARAUTO_1:
            al_draw_scaled_bitmap(game->battle->dialogue_sprite, 0,0, 177, 90, SCREEN_W - 700, 0, 700, 350, 0);

            al_draw_scaled_bitmap(
                game->enemy->portrait,
                0,0,
                520, 500,
                SCREEN_W - 660, 80,
                215, 212,
                0
            );

            al_draw_textf(game->subtitle_11_font, al_map_rgb(54, 16, 4),  SCREEN_W - 375, SCREEN_H / 2 - 300, ALLEGRO_ALIGN_LEFT, "%s:", game->enemy->name);
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 250, ALLEGRO_ALIGN_LEFT, "Finalmente, você chegou ao centro");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 225, ALLEGRO_ALIGN_LEFT, "do conhecimento. Muitos tentaram");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 200, ALLEGRO_ALIGN_LEFT, "trilhar este caminho, mas poucos");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 175, ALLEGRO_ALIGN_LEFT, "possuem coragem e sabedoria");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 150, ALLEGRO_ALIGN_LEFT, "suficiente para encarar minha");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 125, ALLEGRO_ALIGN_LEFT, "essência. Está pronto para ser");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 100, ALLEGRO_ALIGN_LEFT, "testado como ninguém jamais foi?");
            break;

        case DIALOGUE_ARAUTO_2:
            al_draw_scaled_bitmap(game->battle->dialogue_sprite, 0,0, 177, 90, SCREEN_W - 700, 0, 700, 350, 0);
            al_draw_scaled_bitmap(game->player->portrait, 0,0, 520, 596, SCREEN_W - 660, 50, 215, 246, 0);
            al_draw_textf(game->subtitle_11_font, al_map_rgb(54, 16, 4),  SCREEN_W - 375, SCREEN_H / 2 - 300, ALLEGRO_ALIGN_LEFT, "%s:", game->player->name);
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 250, ALLEGRO_ALIGN_LEFT, "Cheguei até aqui porque não");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 225, ALLEGRO_ALIGN_LEFT, "temo desafios. Enfrentei mestres");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 200, ALLEGRO_ALIGN_LEFT, "lendários, resolvi enigmas, e");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 175, ALLEGRO_ALIGN_LEFT, "aprendi que a busca pelo saber");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 150, ALLEGRO_ALIGN_LEFT, "exige muito mais do que força,");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 125, ALLEGRO_ALIGN_LEFT, "exige humildade e persistência.");
            break;

        case DIALOGUE_ARAUTO_3:
            al_draw_scaled_bitmap(game->battle->dialogue_sprite, 0,0, 177, 90, SCREEN_W - 700, 0, 700, 350, 0);

            al_draw_scaled_bitmap(
                game->enemy->portrait,
                0,0,
                520, 500,
                SCREEN_W - 660, 80,
                215, 212,
                0
            );
            
            al_draw_textf(game->subtitle_11_font, al_map_rgb(54, 16, 4),  SCREEN_W - 375, SCREEN_H / 2 - 300, ALLEGRO_ALIGN_LEFT, "%s:", game->enemy->name);
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 250, ALLEGRO_ALIGN_LEFT, "Você fala como quem aprendeu, mas");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 225, ALLEGRO_ALIGN_LEFT, "apenas provará seu valor ao");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 200, ALLEGRO_ALIGN_LEFT, "superar meus desafios supremos.");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 175, ALLEGRO_ALIGN_LEFT, "O conhecimento absoluto não");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 150, ALLEGRO_ALIGN_LEFT, "pertence aos arrogantes ou");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 125, ALLEGRO_ALIGN_LEFT, "impulsivos. Cada resposta errada");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 100, ALLEGRO_ALIGN_LEFT, "terá consequências, cada acerto");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 75, ALLEGRO_ALIGN_LEFT, "te levará mais perto do verdadeiro");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 50, ALLEGRO_ALIGN_LEFT, "domínio da matemática.");
            break;

        case DIALOGUE_ARAUTO_4:
            al_draw_scaled_bitmap(game->battle->dialogue_sprite, 0,0, 177, 90, SCREEN_W - 700, 0, 700, 350, 0);
            al_draw_scaled_bitmap(game->player->portrait, 0,0, 520, 596, SCREEN_W - 660, 50, 215, 246, 0);
            al_draw_textf(game->subtitle_11_font, al_map_rgb(54, 16, 4),  SCREEN_W - 375, SCREEN_H / 2 - 300, ALLEGRO_ALIGN_LEFT, "%s:", game->player->name);
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 250, ALLEGRO_ALIGN_LEFT, "Como eu já disse, enfrentei");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 225, ALLEGRO_ALIGN_LEFT, "obstáculos inimagináveis e superei");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 200, ALLEGRO_ALIGN_LEFT, "cada desafio proposto por você e");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 175, ALLEGRO_ALIGN_LEFT, "os mestres matemáticos. Estou");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 150, ALLEGRO_ALIGN_LEFT, "preparado para enfrentar o maior");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 125, ALLEGRO_ALIGN_LEFT, "enigma de todos.");
            break;

        case DIALOGUE_ARAUTO_5:
            al_draw_scaled_bitmap(game->battle->dialogue_sprite, 0,0, 177, 90, SCREEN_W - 700, 0, 700, 350, 0);
            
            al_draw_scaled_bitmap(
                game->enemy->portrait,
                0,0,
                520, 500,
                SCREEN_W - 660, 80,
                215, 212,
                0
            );

            al_draw_textf(game->subtitle_11_font, al_map_rgb(54, 16, 4),  SCREEN_W - 375, SCREEN_H / 2 - 300, ALLEGRO_ALIGN_LEFT, "%s:", game->enemy->name);
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 250, ALLEGRO_ALIGN_LEFT, "Excelente. Que suas ações");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 225, ALLEGRO_ALIGN_LEFT, "revelem mais que palavras. No");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 200, ALLEGRO_ALIGN_LEFT, "território do conhecimento,");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 175, ALLEGRO_ALIGN_LEFT, "lógica, criatividade e razão");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 150, ALLEGRO_ALIGN_LEFT, "serão suas únicas armas. Vença,");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 125, ALLEGRO_ALIGN_LEFT, "e será digno de dominar a arte");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 100, ALLEGRO_ALIGN_LEFT, "matemática até o fim dos tempos.");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 75, ALLEGRO_ALIGN_LEFT, "Fraqueje, e suas dúvidas o");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 50, ALLEGRO_ALIGN_LEFT, "devorarão.");
            break;
        
        case DIALOGUE_ARAUTO_FINAL_1:
            al_draw_scaled_bitmap(game->battle->dialogue_sprite, 0,0, 177, 90, SCREEN_W - 700, 0, 700, 350, 0);
            
            al_draw_scaled_bitmap(
                game->enemy->portrait,
                0,0,
                520, 500,
                SCREEN_W - 660, 80,
                215, 212,
                0
            );

            al_draw_textf(game->subtitle_11_font, al_map_rgb(54, 16, 4),  SCREEN_W - 375, SCREEN_H / 2 - 300, ALLEGRO_ALIGN_LEFT, "%s:", game->enemy->name);
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 250, ALLEGRO_ALIGN_LEFT, "Impressionante... Você superou");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 225, ALLEGRO_ALIGN_LEFT, "não só meus enigmas, mas me");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 200, ALLEGRO_ALIGN_LEFT, "derrotou por completo.");
            break;
        case DIALOGUE_ARAUTO_FINAL_2:
            al_draw_scaled_bitmap(game->battle->dialogue_sprite, 0,0, 177, 90, SCREEN_W - 700, 0, 700, 350, 0);
            al_draw_scaled_bitmap(game->player->portrait, 0,0, 520, 596, SCREEN_W - 660, 50, 215, 246, 0);
            al_draw_textf(game->subtitle_11_font, al_map_rgb(54, 16, 4),  SCREEN_W - 375, SCREEN_H / 2 - 300, ALLEGRO_ALIGN_LEFT, "%s:", game->player->name);
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 250, ALLEGRO_ALIGN_LEFT, "A jornada foi árdua, com");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 225, ALLEGRO_ALIGN_LEFT, "intensos desafios. Mas entendi");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 200, ALLEGRO_ALIGN_LEFT, "que o saber absoluto não é um");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 175, ALLEGRO_ALIGN_LEFT, "destino, e sim um caminho que");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 150, ALLEGRO_ALIGN_LEFT, "se percorre com muita coragem");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 125, ALLEGRO_ALIGN_LEFT, "e persistência.");
            break;
        case DIALOGUE_ARAUTO_FINAL_3:
            al_draw_scaled_bitmap(game->battle->dialogue_sprite, 0,0, 177, 90, SCREEN_W - 700, 0, 700, 350, 0);
            
            al_draw_scaled_bitmap(
                game->enemy->portrait,
                0,0,
                520, 500,
                SCREEN_W - 660, 80,
                215, 212,
                0
            );

            al_draw_textf(game->subtitle_11_font, al_map_rgb(54, 16, 4),  SCREEN_W - 375, SCREEN_H / 2 - 300, ALLEGRO_ALIGN_LEFT, "%s:", game->enemy->name);
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 250, ALLEGRO_ALIGN_LEFT, "Está certo. Você aprendeu que");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 225, ALLEGRO_ALIGN_LEFT, "nem toda resposta é suficiente,");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 200, ALLEGRO_ALIGN_LEFT, "muitas vezes, a grandeza está");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 175, ALLEGRO_ALIGN_LEFT, "em questionar. Siga em frente,");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 150, ALLEGRO_ALIGN_LEFT, "jovem, levando não só o poder do");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 125, ALLEGRO_ALIGN_LEFT, "conhecimento, mas a sabedoria de");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 100, ALLEGRO_ALIGN_LEFT, "quem sabe que sempre há mais");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 75, ALLEGRO_ALIGN_LEFT, "a descobrir.");
            break;
        case DIALOGUE_ARAUTO_FINAL_4:
            al_draw_scaled_bitmap(game->battle->dialogue_sprite, 0,0, 177, 90, SCREEN_W - 700, 0, 700, 350, 0);
            al_draw_scaled_bitmap(game->player->portrait, 0,0, 520, 596, SCREEN_W - 660, 50, 215, 246, 0);
            al_draw_textf(game->subtitle_11_font, al_map_rgb(54, 16, 4),  SCREEN_W - 375, SCREEN_H / 2 - 300, ALLEGRO_ALIGN_LEFT, "%s:", game->player->name);
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 250, ALLEGRO_ALIGN_LEFT, "Obrigado, Arauto. Agora que irei");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 225, ALLEGRO_ALIGN_LEFT, "obter todo o conhecimento da");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 200, ALLEGRO_ALIGN_LEFT, "matemática, eu sinto orgulho da");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 175, ALLEGRO_ALIGN_LEFT, "minha jornada.");
            break;
        case DIALOGUE_ARAUTO_FINAL_5:
            al_draw_scaled_bitmap(game->battle->dialogue_sprite, 0,0, 177, 90, SCREEN_W - 700, 0, 700, 350, 0);
            
            al_draw_scaled_bitmap(
                game->enemy->portrait,
                0,0,
                520, 500,
                SCREEN_W - 660, 80,
                215, 212,
                0
            );

            al_draw_textf(game->subtitle_11_font, al_map_rgb(54, 16, 4),  SCREEN_W - 375, SCREEN_H / 2 - 300, ALLEGRO_ALIGN_LEFT, "%s:", game->enemy->name);
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 250, ALLEGRO_ALIGN_LEFT, "Você não tem que agradecer,");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 225, ALLEGRO_ALIGN_LEFT, "você passou nos testes e fez");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 200, ALLEGRO_ALIGN_LEFT, "por merecer. Parabéns jovem!.");
            break;
        
        case DIALOGUE_ARAUTO_RANDOM_1:
            al_draw_scaled_bitmap(game->battle->dialogue_sprite, 0,0, 177, 90, SCREEN_W - 700, 0, 700, 350, 0);
            
            al_draw_scaled_bitmap(
                game->enemy->portrait,
                0,0,
                520, 500,
                SCREEN_W - 660, 80,
                215, 212,
                0
            );

            al_draw_textf(game->subtitle_11_font, al_map_rgb(54, 16, 4),  SCREEN_W - 375, SCREEN_H / 2 - 300, ALLEGRO_ALIGN_LEFT, "%s:", game->enemy->name);
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 250, ALLEGRO_ALIGN_LEFT, "Você acha que chegou até aqui");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 225, ALLEGRO_ALIGN_LEFT, "por mérito, mas na verdade é");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 200, ALLEGRO_ALIGN_LEFT, "só sorte disfarçada de talento.");
            break;
        case DIALOGUE_ARAUTO_RANDOM_2:
            al_draw_scaled_bitmap(game->battle->dialogue_sprite, 0,0, 177, 90, SCREEN_W - 700, 0, 700, 350, 0);
            
            al_draw_scaled_bitmap(
                game->enemy->portrait,
                0,0,
                520, 500,
                SCREEN_W - 660, 80,
                215, 212,
                0
            );

            al_draw_textf(game->subtitle_11_font, al_map_rgb(54, 16, 4),  SCREEN_W - 375, SCREEN_H / 2 - 300, ALLEGRO_ALIGN_LEFT, "%s:", game->enemy->name);
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 250, ALLEGRO_ALIGN_LEFT, "Tanta confiança... mas e se");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 225, ALLEGRO_ALIGN_LEFT, "tudo o que sabe estiver errado?");
            break;
        case DIALOGUE_ARAUTO_RANDOM_3:
            al_draw_scaled_bitmap(game->battle->dialogue_sprite, 0,0, 177, 90, SCREEN_W - 700, 0, 700, 350, 0);
            
            al_draw_scaled_bitmap(
                game->enemy->portrait,
                0,0,
                520, 500,
                SCREEN_W - 660, 80,
                215, 212,
                0
            );

            al_draw_textf(game->subtitle_11_font, al_map_rgb(54, 16, 4),  SCREEN_W - 375, SCREEN_H / 2 - 300, ALLEGRO_ALIGN_LEFT, "%s:", game->enemy->name);
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 250, ALLEGRO_ALIGN_LEFT, "O conhecimento que você busca");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 225, ALLEGRO_ALIGN_LEFT, "pode ser a armadilha que");
            al_draw_text(game->subtitle_11_font, al_map_rgb(54, 16, 4), SCREEN_W - 400, SCREEN_H / 2 - 200, ALLEGRO_ALIGN_LEFT, "finalmente o destruirá.");
            break;

        case DIALOGUE_BATTLE_FINAL_INTRO:
            al_draw_text(game->subtitle_11_font, al_map_rgb(255, 255, 0), 70, 20, ALLEGRO_ALIGN_CENTER, "Pressione");
            al_draw_scaled_bitmap(
                    game->controls,
                    0, 0,
                    16, 16,
                    130, 12,
                    32, 32,
                    0 
                );   
            al_draw_text(game->subtitle_11_font, al_map_rgb(255, 255, 0), 300, 20, ALLEGRO_ALIGN_CENTER, "para começar a batalha!");
   
            
            break;
        case DIALOGUE_AFTER_TRASH_TALK:
            al_draw_text(game->subtitle_11_font, al_map_rgb(255, 255, 0), 70, 20, ALLEGRO_ALIGN_CENTER, "Pressione");
            al_draw_scaled_bitmap(
                    game->controls,
                    0, 0,
                    16, 16,
                    130, 12,
                    32, 32,
                    0 
                );   
            al_draw_text(game->subtitle_11_font, al_map_rgb(255, 255, 0), 300, 20, ALLEGRO_ALIGN_CENTER, "para retomar a batalha!");
   
            
            break;
        default:
            break;
        }
    } else {

        al_draw_scaled_bitmap(
            game->enemy->hp_canva,
            0, 0,
            895, 470,
            SCREEN_W - 380, 20,
            324, 170,
            0
        );
    }


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

    for(int i = 0; i < MAX_ENEMIES; i++){
        if (!game->mobs[i]) {
            continue; 
        }
    
        if(!game->mobs[i]->entity.isActive) {
            continue;
        }

        //Com mudança no game_state precisaremos refatorar todo o código de state do game
        if(game->gameplay_state == GAMEPLAY_BATTLE && game->battle) return;
    
        int dist = game->player->entity.x - game->mobs[i]->entity.x;
    
        if(dist >= -300 && dist <= 300){
            
            if(dist < 0) game->mobs[i]->entity.flip = ALLEGRO_FLIP_HORIZONTAL;
            game->gameplay_state = GAMEPLAY_BATTLE;
            start_battle(game->battle, game->player, game->mobs[i]);
        }
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
        change_game_state(game, GAME_OVER);
    }
}

bool check_interaction(ALLEGRO_BITMAP* control, Entity* entity_1, Entity* entity_2){
    if(entity_1->box.x + entity_1->box.w >= entity_2->box.x && entity_1->box.x <= entity_2->box.x + entity_2->box.w 
        && entity_1->box.y + entity_1->box.h >= entity_2->box.y && entity_1->box.y <= entity_2->box.y + entity_2->box.h){
            if(entity_2->entity_type == DOOR || entity_2->entity_type == EDUCATIONAL || entity_2->entity_type == PUZZLE_DIAL || entity_2->entity_type == PUZZLE_LEVER){
                render_control(control, entity_2, INTERACT_E);
            } 
            return true;
    }

    return false;
}

void return_level(Game* game, Door* door_1){
    if(door_1->door_type != DOOR_RETURN) return;

    if(game->state == GAME_SECOND_MISSION){
        change_game_state(game, GAME_FIRST_MISSION);
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
        change_game_state(game, GAME_SECOND_MISSION);
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

void resolve_interaction_with_puzzle(Game* game, Player* player, Entity* current_entity, unsigned char* key){
    if(key[ALLEGRO_KEY_E]){
        switch (current_entity->puzzle_id)
        {
        case PUZZLE_BHASKARA:
            game->previous_game_state = game->state; 
            game->active_puzzle_id = PUZZLE_BHASKARA; 
            game->gameplay_state = GAMEPLAY_PUZZLE;
            change_game_state(game, GAME_PUZZLE_SCREEN);
            break;
        case PUZZLE_EQUATION_BHASKARA:
            game->previous_game_state = game->state; 
            game->active_puzzle_id = PUZZLE_EQUATION_BHASKARA; 
            game->gameplay_state = GAMEPLAY_PUZZLE;
            change_game_state(game, GAME_PUZZLE_SCREEN);
            break;
        default:
            break;
        }


        key[ALLEGRO_KEY_E] = 0;
    }
}

void check_bhaskara_solution(Game* game, Door* door) {
    
    int values[3] = {0, 0, 0};
    int current_val = 0;

    for (int i = 0; i < game->num_world_entities; i++) {
        Entity* e = game->world_entities[i];
        if (e && e->isActive) {
            
            if (e->entity_type == PUZZLE_DIAL) {
                if (current_val < 3) {
                    values[current_val] = e->puzzle_value;
                    current_val++;
                }
            }
        }
    }


    switch(game->state){
        case GAME_FIRST_MISSION:

            for(int i = 0; i < 3; i++){
                printf("%d\n", values[i]);
            }

            bool found_16 = false;
            bool found_6 = false;
            bool found_2 = false;

            for (int i = 0; i < 3; i++) {
                if (values[i] == 16) found_16 = true;
                else if (values[i] == 6) found_6 = true;
                else if (values[i] == 2) found_2 = true;
            }

            if (found_16 && found_6 && found_2 && door != NULL) {
                door->entity.is_locked_puzzle = false; 
                sprintf(game->log_ln1, "Um mecanismo range e a porta se abre!");
            } else {
                sprintf(game->log_ln1, "Os números estão incorretos... Nada acontece.");
            }

            break;
        case GAME_SECOND_MISSION:
            for(int i = 0; i < 3; i++){
                printf("%d\n", values[i]);
            }

            bool found_4 = false;
            bool found_3 = false;
            bool found_5 = false;

            for (int i = 0; i < 3; i++) {
                if (values[i] == 4) found_4 = true;
                else if (values[i] == 3) found_3 = true;
                else if (values[i] == 5) found_5 = true;
            }

            if (found_4 && found_3 && found_5 && door != NULL) {
                door->entity.is_locked_puzzle = false; 
                sprintf(game->log_ln1, "Um mecanismo range e a porta se abre!");
            } else {
                sprintf(game->log_ln1, "Os números estão incorretos... Nada acontece.");
            }
            break;
        case GAME_THIRD_MISSION:
            for(int i = 0; i < 3; i++){
                printf("%d\n", values[i]);
            }

            bool found_4_a = false;
            bool found_3_a = false;
            bool found_5_a = false;

            for (int i = 0; i < 3; i++) {
                if (values[i] == 4) found_4_a = true;
                else if (values[i] == 3) found_3_a = true;
                else if (values[i] == 5) found_5_a = true;
            }

            if (found_4_a && found_3_a && found_5_a && door != NULL) {
                door->entity.is_locked_puzzle = false; 
                sprintf(game->log_ln1, "Um mecanismo range e a porta se abre!");
            } else {
                sprintf(game->log_ln1, "Os números estão incorretos... Nada acontece.");
            }
        default:
            break;
    }
    
 

}

void handle_player_interaction(Game* game, Player* player, Entity* entity, unsigned char* key){
    
    if(key[ALLEGRO_KEY_E]){
        switch (entity->entity_type)
        {
            case PUZZLE_DIAL:
                entity->puzzle_value++;
                if (entity->puzzle_value > 20) {
                    entity->puzzle_value = 0;
                }
                break;
                
            case PUZZLE_LEVER:
                //check_bhaskara_solution(game);
                break;
            break;
        
        default:
            break;
        }

        key[ALLEGRO_KEY_E] = 0;
    }

}

bool player_has_item(Player* player, const char* item_id) {
    for (int i = 0; i < MAX_EQUIP_SLOTS; i++) {
        if (player->equipment[i] != NULL) {
            if (player->equipment[i]->type == ITEM_EQUIPMENT && 
                strcmp(player->equipment[i]->id, item_id) == 0) 
            {
                return true;
            }
        }
    }
    return false;
}

void resolve_interaction_with_door(Game* game, Player* player, Entity* entity_2, unsigned char* key){
    if (entity_2->entity_type != DOOR) return;

    Door* door = (Door*) entity_2;

    if(key[ALLEGRO_KEY_E]){
        switch(door->door_type){
            case DOOR_RETURN:
                return_level(game, door);
                break;
            case DOOR_MINOTAUR:
               if (door->entity.is_locked_key) {
                    if (player_has_item(player, "minotauro_chave")) {

                        al_start_timer(game->timer_game_logs);
                        sprintf(game->log_ln1, "Você usa a chave do Minotauro.");
                        sprintf(game->log_ln2, "Onde jaz o para sempre imóvel, lá encontrará o que procura.");
                        sprintf(game->log_ln3, "(O mecanismo da chave destrancou, mas a porta continua selada.)");
                        
                        door->entity.is_locked_key = false; 
                    } else {

                        al_start_timer(game->timer_game_logs);
                        sprintf(game->log_ln1, "Você não possui a chave para abrir essa porta.");
                        sprintf(game->log_ln2, "Essa porta possui um emblema com um MINOTAURO.");
                        sprintf(game->log_ln3, "\"Já que está aqui. Avance para depois voltar.\"");
                    }
                    
                    return; 
                    }
                    if (door->entity.is_locked_puzzle) {
                        
                        al_start_timer(game->timer_game_logs);
                        check_bhaskara_solution(game, door); 
                        return;
                    }

                    if (door->entity.is_locked_key == false && door->entity.is_locked_puzzle == false) {
                        change_game_state(game, GAME_MINOTAUR_LEVEL);
                        game->gameplay_state = GAMEPLAY_BATTLE;
                        render_minotaur_level(game);
                        return;
                    }
                    
                    break;
            case DOOR_MEDUSA:
                if (door->entity.is_locked_key) {
                    if (player_has_item(player, "medusa_chave")) {

                        al_start_timer(game->timer_game_logs);
                        sprintf(game->log_ln1, "Você usa a chave da Medusa.");
                        sprintf(game->log_ln2, "Onde jaz o para sempre imóvel, lá encontrará o que procura.");
                        sprintf(game->log_ln3, "(O mecanismo da chave destrancou, mas a porta continua selada.)");
                        
                        door->entity.is_locked_key = false; 
                    } else {

                        al_start_timer(game->timer_game_logs);
                        sprintf(game->log_ln1, "Você não possui a chave para abrir essa porta.");
                        sprintf(game->log_ln2, "Essa porta possui um emblema com uma MEDUSA.");
                        sprintf(game->log_ln3, "\"Hmm, Interessante.\"");
                    }
                    
                    return; 
                    }
                    if (door->entity.is_locked_puzzle) {
                        
                        al_start_timer(game->timer_game_logs);
                        check_bhaskara_solution(game, door); 
                        return;
                    }

                    if (door->entity.is_locked_key == false && door->entity.is_locked_puzzle == false) {
                        change_game_state(game, GAME_MEDUSA_LEVEL);
                        game->gameplay_state = GAMEPLAY_BATTLE;
                        render_medusa_level(game);
                        return;
                    }
                    
                    break;
            case DOOR_ARAUTO:
               if (door->entity.is_locked_key) {
                    if (player_has_item(player, "arauto_chave")) {

                        al_start_timer(game->timer_game_logs);
                        sprintf(game->log_ln1, "Você usa a chave da Medusa.");
                        sprintf(game->log_ln2, "Onde jaz o para sempre imóvel, lá encontrará o que procura.");
                        sprintf(game->log_ln3, "(O mecanismo da chave destrancou, mas a porta continua selada.)");
                        
                        door->entity.is_locked_key = false; 
                    } else {

                        al_start_timer(game->timer_game_logs);
                        sprintf(game->log_ln1, "Você não possui a chave para abrir essa porta.");
                        sprintf(game->log_ln2, "Essa porta não possui detalhes.");
                    }
                    
                    return; 
                    }
                    if (door->entity.is_locked_puzzle) {
                        
                        al_start_timer(game->timer_game_logs);
                        check_bhaskara_solution(game, door); 
                        return;
                    }

                    if (door->entity.is_locked_key == false && door->entity.is_locked_puzzle == false) {
                        change_game_state(game, GAME_ARAUTO_LEVEL);
                        game->gameplay_state = GAMEPLAY_BATTLE;
                        render_arauto_level(game);
                        return;
                    }
                    
                    break;
            case DOOR_NEXT_LEVEL:
                if(game->state == GAME_FIRST_MISSION){
                    for(int i = 0; i < MAX_EQUIP_SLOTS; i++){
                        if(player->equipment[i] != NULL && player->equipment[i]->type == ITEM_EQUIPMENT && strcmp(player->equipment[i]->id, "segundomapa_chave") == 0){
                            change_game_state(game, GAME_SECOND_MISSION);
                            game->gameplay_state = GAMEPLAY_EXPLORING;
                            load_second_map(game);
                            return;
                        } 
                    }
                    al_start_timer(game->timer_game_logs);
                    sprintf(game->log_ln1, "Você não possui a chave para abrir essa porta");
                    sprintf(game->log_ln2, "Nela está escrito:");
                    sprintf(game->log_ln3, "Vá em frente!");

                } else if(game->state == GAME_SECOND_MISSION){

                     for(int i = 0; i < MAX_EQUIP_SLOTS; i++){
                        if(player->equipment[i] != NULL && player->equipment[i]->type == ITEM_EQUIPMENT && strcmp(player->equipment[i]->id, "terceiromapa_chave") == 0){
                            change_game_state(game, GAME_THIRD_MISSION);
                            game->gameplay_state = GAMEPLAY_EXPLORING;
                            load_third_map(game);
                            return;
                        } 
                    }
                    al_start_timer(game->timer_game_logs);
                    sprintf(game->log_ln1, "Você não possui a chave para abrir essa porta");
                    sprintf(game->log_ln3, "Se conseguir, encontre-se comigo!");
                }
                break;
            case DOOR_EASTER_EGG:
                 for(int i = 0; i < MAX_EQUIP_SLOTS; i++){
                    if(player->equipment[i] != NULL && player->equipment[i]->type == ITEM_EQUIPMENT && strcmp(player->equipment[i]->id, "easteregg_chave") == 0){
                        change_game_state(game, GAME_ARAUTO_LEVEL);
                        game->gameplay_state = GAMEPLAY_BATTLE;
                        render_arauto_level(game);
                        return;
                    } 
                    
                }
                al_start_timer(game->timer_game_logs);
                sprintf(game->log_ln1, "Você não consegue abrir a porta, ela não tem fechadura.");
                sprintf(game->log_ln2, "Essa porta não possui emblema. Só uma escritura direto na madeira: Aqui a verdade jaz...");
                break;
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
   if(game->battle->state == BATTLE_DIALOGUE){
        return;
    }

    if(game->battle->state == BATTLE_NONE){
        change_game_state(game, GAME_FIRST_MISSION);
        game->gameplay_state = GAMEPLAY_EXPLORING;
        load_first_map(game);

        for(int i = 0; i < game->num_world_entities; i++) {
            Entity* current_entity = game->world_entities[i];
            if(current_entity && current_entity->entity_type == DOOR) {
                Door* door = (Door*) current_entity;
                if(door->door_type == DOOR_MINOTAUR) {
                    set_entity_pos(&game->player->entity, door->entity.x, door->entity.y);
                    break; 
                }
            }
        }
        return;
    }
    if (key[ALLEGRO_KEY_K] || game->player->entity.box.x <= 0) {
        
        if (game->battle->enemy) {
            destroy_enemy(game->battle->enemy); 
            game->enemy = NULL;
            game->battle->enemy = NULL;
        }
        
        game->battle->state = BATTLE_NONE;
        game->gameplay_state = GAMEPLAY_EXPLORING;
        load_first_map(game);
        change_game_state(game, GAME_FIRST_MISSION);
            
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
    if(game->battle->state == BATTLE_DIALOGUE){
        return;
    }

    if(game->battle->state == BATTLE_NONE){
        change_game_state(game, GAME_SECOND_MISSION);
        game->gameplay_state = GAMEPLAY_EXPLORING;
        load_second_map(game);

        for(int i = 0; i < game->num_world_entities; i++) {
            Entity* current_entity = game->world_entities[i];
            if(current_entity && current_entity->entity_type == DOOR) {
                Door* door = (Door*) current_entity;
                if(door->door_type == DOOR_MEDUSA) {
                    set_entity_pos(&game->player->entity, door->entity.x, door->entity.y);
                    break; 
                }
            }
        }
        return;
    }
    if (key[ALLEGRO_KEY_K] || game->player->entity.box.x <= 0) {
        
        if (game->battle->enemy) {
            destroy_enemy(game->battle->enemy); 
            game->enemy = NULL;
            game->battle->enemy = NULL;
        }
        
        game->battle->state = BATTLE_NONE;
        game->gameplay_state = GAMEPLAY_EXPLORING;
        load_second_map(game);
        change_game_state(game, GAME_SECOND_MISSION);
            
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
    if(game->battle->state == BATTLE_DIALOGUE){
        return;
    }

    if(game->battle->state == BATTLE_NONE){
        change_game_state(game, GAME_END);
        // game->gameplay_state = GAMEPLAY_EXPLORING;
        // load_third_map(game);

        // for(int i = 0; i < game->num_world_entities; i++) {
        //     Entity* current_entity = game->world_entities[i];
        //     if(current_entity && current_entity->entity_type == DOOR) {
        //         Door* door = (Door*) current_entity;
        //         if(door->door_type == DOOR_ARAUTO) {
        //             set_entity_pos(&game->player->entity, door->entity.x, door->entity.y);
        //             break; 
        //         }
        //     }
        // }
        // return;
    }
    if (key[ALLEGRO_KEY_K] || game->player->entity.box.x <= 0) {
        
        if (game->battle->enemy) {
            destroy_enemy(game->battle->enemy); 
            game->enemy = NULL;
            game->battle->enemy = NULL;
        }
        
        game->battle->state = BATTLE_NONE;
        game->gameplay_state = GAMEPLAY_EXPLORING;
        load_third_map(game);
        change_game_state(game, GAME_THIRD_MISSION);
            
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

static void update_puzzle_state(Game* game, unsigned char* key, float dt){
    
    if (key[ALLEGRO_KEY_E] || key[ALLEGRO_KEY_ESCAPE]) {
        
        key[ALLEGRO_KEY_E] = 0;
        key[ALLEGRO_KEY_ESCAPE] = 0;

        change_game_state(game, game->previous_game_state);
        game->gameplay_state = GAMEPLAY_EXPLORING;

        game->active_puzzle_id = PUZZLE_NONE;

        game->log_ln1[0] = '\0';
        game->log_ln2[0] = '\0';
        game->log_ln3[0] = '\0';
        game->log_ln4[0] = '\0';

        game->log_ln_tip_1[0] = '\0';
        game->log_ln_tip_2[0] = '\0';
        game->log_ln_tip_3[0] = '\0';
        
        return;
    }

    switch (game->active_puzzle_id) {
        
        case PUZZLE_BHASKARA:

            break;
            
        case PUZZLE_PYTHAGORAS:

            break;

        case PUZZLE_EQUATION_BHASKARA:

            break;
        case PUZZLE_NONE:
        default:
            change_game_state(game, game->previous_game_state);
            game->active_puzzle_id = PUZZLE_NONE;
            break;
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
                resolve_interaction_with_door(game, game->player, current_entity, key);
            }
        }

        if (current_entity && current_entity->entity_type == EDUCATIONAL) {
            if (check_interaction(game->controls, &game->player->entity, current_entity)) {
                resolve_interaction_with_puzzle(game, game->player, current_entity, key);
            }
        }
        
        if(current_entity && current_entity->entity_type == PUZZLE_DIAL)
            if (check_interaction(game->controls, &game->player->entity, current_entity)) {
                handle_player_interaction(game, game->player, current_entity, key);
            }
        }

    for(int i = 0; i < MAX_ENEMIES; i++){
        if(game->mobs[i] && game->mobs[i]->entity.isActive){
            update_enemy(game->mobs[i], dt);
        }
    }
}

static void update_battle_state(Game* game, ALLEGRO_EVENT event, unsigned char* key, float dt) {
    Enemy* enemy_in_battle = game->battle->enemy;

    if (game->battle->state == BATTLE_NONE) {
        game->gameplay_state = GAMEPLAY_EXPLORING;

        if(enemy_in_battle && !enemy_in_battle->entity.isActive && enemy_in_battle->enemy_type != MOB){
            destroy_enemy(enemy_in_battle);
            game->enemy = NULL;
            game->battle->enemy = NULL;
        }
        return;
    }

    // if (game->battle->state == BATTLE_WIN) {
    //     game->gameplay_state = GAMEPLAY_EXPLORING;
    //     game->battle = NULL;
    //     return;
    // }
    
    if(enemy_in_battle && enemy_in_battle->entity.isActive == true){
        update_enemy(enemy_in_battle, dt);
    }
    
    manage_battle(game->battle, event, game->state, key, game->subtitle_font, &game->world_enemies);
    update_player_battle(game->player, key, dt);
}

void update_game(Game* game, unsigned char* key, ALLEGRO_EVENT event, float dt) {
    read_mouse(game);

    bool is_up_level = buff_levels(game->player);

    if(is_up_level){
        sprintf(game->log_ln1, "Você upou para o nível %d", game->player->level);
        sprintf(game->log_ln2, "Todas as suas estatisticas foram aumentadas");
        al_start_timer(game->timer_game_logs);
    }

    if(event.timer.source == game->timer_game_logs || event.timer.source == game->timer_game_tips){
        game->log_ln1[0] = '\0';
        game->log_ln2[0] = '\0';
        game->log_ln3[0] = '\0';
        game->log_ln4[0] = '\0';
        game->log_ln_tip_1[0] = '\0';
        game->log_ln_tip_2[0] = '\0';
        game->log_ln_tip_3[0] = '\0';

        al_stop_timer(game->timer_game_logs);
        al_stop_timer(game->timer_game_tips);
        al_set_timer_count(game->timer_game_logs, 0);
        al_set_timer_count(game->timer_game_tips, 0);

        game->can_draw = true;
    }
    
    switch(game->gameplay_state){
        case GAMEPLAY_EXPLORING:
            player_recalculate_stats(game->player);
            //check_map_collision(&game->player->entity, game->map);
            resolve_map_collision(&game->player->entity, game->map);
            log_tips(game);
            break;
        case GAMEPLAY_BATTLE:
            player_recalculate_stats(game->player);
            update_battle_state(game, event, key, dt);

            if(game->battle->enemy && game->battle->enemy->enemy_type != MOB)
                resolve_map_collision_battle(&game->player->entity);
            else 
                resolve_map_collision(&game->player->entity, game->map);
            break;
        case GAMEPLAY_PUZZLE:

            break;
    }

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
                case GAMEPLAY_PUZZLE:
                    update_puzzle_state(game, key, dt);
                    break;
                case GAMEPLAY_BATTLE:
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
                case GAMEPLAY_PUZZLE:
                    update_puzzle_state(game, key, dt);
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
                case GAMEPLAY_PUZZLE:
                    update_puzzle_state(game, key, dt);
                    break;
                case GAMEPLAY_NONE:
                default:
                    break;
            }
            break;
        case GAME_PUZZLE_SCREEN:
            update_puzzle_state(game, key, dt);
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
        0, 0, SCREEN_W, SCREEN_H,    
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
                    change_game_state(game, GAME_FIRST_MISSION);
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

int calculate_hearts(int hp){
    if(hp >= 100) {return 5;}
    else if(hp >= 80) {return 4;}
    else if(hp >= 60) {return 3;}
    else if(hp >= 40) {return 2;}
    else if(hp >= 20) {return 1;}
    else {return 0;}
}


void draw_hp_bar_scalable(ALLEGRO_FONT* font, int x, int y, int width, int height, int hp, int max_hp) {
    
    if (hp < 0) hp = 0;

    float hp_ratio = (float)hp / (float)max_hp;

    int current_hp_width = (int)(hp_ratio * width);

    al_draw_filled_rectangle(x, y, x + width, y + height, al_map_rgb(50, 50, 50));

    al_draw_filled_rectangle(x, y, x + current_hp_width, y + height, al_map_rgb(255, 0, 0));

    al_draw_rectangle(x, y, x + width, y + height, al_map_rgb(255, 255, 255), 2);

    char hp_text[20];
    sprintf(hp_text, "%d / %d", hp, max_hp);

    int text_center_x = x + (width / 2);
    int text_center_y = y + (height / 2) - (12 / 2);

    al_draw_text(font, al_map_rgb(255, 255, 255), text_center_x, text_center_y, ALLEGRO_ALIGN_CENTER, hp_text);
}

void draw_attack(ALLEGRO_BITMAP* bitmap, ALLEGRO_FONT* font, int x, int y, int attack){
    al_draw_scaled_bitmap(
        bitmap,
        0, 0,
        32, 32,
        x, y,
        24, 24,
        0
    );

    al_draw_textf(font, al_map_rgb(255, 255, 255), x + 35, y + 8, ALLEGRO_ALIGN_CENTER, "%d", attack);
}

void draw_defense(ALLEGRO_BITMAP* bitmap, ALLEGRO_FONT* font, int x, int y, int defense){
    al_draw_scaled_bitmap(
        bitmap,
        0, 0,
        32, 32,
        x, y,
        24, 24,
        0
    );
    al_draw_textf(font, al_map_rgb(255, 255, 255), x + 35, y + 8, ALLEGRO_ALIGN_CENTER, "%d", defense);
}

void draw_inventory(Player* player, ALLEGRO_FONT* font){
    for(int i = 0; i < MAX_ITENS; i++){
        Item* current_item = player->inventory.slots[i].item;
        
        if(current_item == NULL) continue;

        al_draw_textf(font, al_map_rgb(255, 255, 255), current_item->entity->x, current_item->entity->y + current_item->entity->box.h - 5, ALLEGRO_ALIGN_CENTER, "%d", player->inventory.slots[i].quantity);
        al_draw_textf(font, al_map_rgb(255, 255, 255), current_item->entity->x + current_item->entity->box.w, current_item->entity->y + current_item->entity->box.h - 5, ALLEGRO_ALIGN_CENTER, "%s", current_item->id);

        draw_entity(current_item->entity);
        float x1 = current_item->entity->box.x - 10;
        float y1 = current_item->entity->box.y;
        float x2 = x1 + current_item->entity->box.w + 20; 
        float y2 = y1 + current_item->entity->box.h + 10;  

        al_draw_rectangle(x1, y1, x2, y2, al_map_rgb(255, 255, 255), 2);
    }

}


void draw_puzzle_state(Game* game) {
    
    al_draw_filled_rectangle(0, 0, SCREEN_W, SCREEN_H, al_map_rgba(0, 0, 0, 150));

    switch (game->active_puzzle_id) {
        
        case PUZZLE_BHASKARA:
            if (game->puzzle_bhaskara_img) {
                float img_w = al_get_bitmap_width(game->puzzle_bhaskara_img);
                float img_h = al_get_bitmap_height(game->puzzle_bhaskara_img);
                float x = (SCREEN_W - img_w) / 2.0 - 200;
                float y = (SCREEN_H - img_h) / 2.0 - 200;
                
                al_draw_scaled_bitmap(
                    game->puzzle_bhaskara_img,
                    0, 0,
                    128, 141,
                    x, y,
                    453, 500,
                    0
                );

                sprintf(game->log_ln1, "Você vê um quadro com a fórmula resolutiva.");
                sprintf(game->log_ln2, "Quem sabe você precise usar mais para frente.");
            } else {
                al_draw_text(game->title_font, al_map_rgb(255, 0, 0), 
                    SCREEN_W / 2, SCREEN_H / 2, 
                    ALLEGRO_ALIGN_CENTER, "Erro: Imagem do puzzle não encontrada!");
            }
            break;

        case PUZZLE_EQUATION_BHASKARA:
            if(game->previous_game_state == GAME_FIRST_MISSION){
                if (game->puzzle_bhaskara_paper) {
                    float img_w = al_get_bitmap_width(game->puzzle_bhaskara_paper);
                    float img_h = al_get_bitmap_height(game->puzzle_bhaskara_paper);
                    float x = (SCREEN_W - img_w) / 2.0;
                    float y = (SCREEN_H - img_h) / 2.0;
                    
                    al_draw_scaled_bitmap(
                        game->puzzle_bhaskara_paper,
                        0, 0,
                        256, 384,
                        x, y,
                        256, 384,
                        0
                    );

                    sprintf(game->log_ln1, "Você vê uma tabuleta sobre a fórmula resolutiva.");
                    sprintf(game->log_ln2, "Estranho...");
                } else {
                    al_draw_text(game->title_font, al_map_rgb(255, 0, 0), 
                                SCREEN_W / 2, SCREEN_H / 2, 
                                ALLEGRO_ALIGN_CENTER, "Erro: Imagem do puzzle não encontrada!");
                }
            } else if(game->previous_game_state == GAME_SECOND_MISSION){
                if (game->puzzle_bhaskara_paper_2) {
                    float img_w = al_get_bitmap_width(game->puzzle_bhaskara_paper_2);
                    float img_h = al_get_bitmap_height(game->puzzle_bhaskara_paper_2);
                    float x = (SCREEN_W - img_w) / 2.0;
                    float y = (SCREEN_H - img_h) / 2.0;
                    
                    al_draw_scaled_bitmap(
                        game->puzzle_bhaskara_paper_2,
                        0, 0,
                        256, 384,
                        x, y,
                        256, 384,
                        0
                    );

                    sprintf(game->log_ln1, "Você vê uma tabuleta sobre a fórmula resolutiva.");
                    sprintf(game->log_ln2, "Estranho...");
                } else {
                    al_draw_text(game->title_font, al_map_rgb(255, 0, 0), 
                                SCREEN_W / 2, SCREEN_H / 2, 
                                ALLEGRO_ALIGN_CENTER, "Erro: Imagem do puzzle não encontrada!");
                }
            } else if(game->previous_game_state == GAME_THIRD_MISSION){
                if (game->puzzle_bhaskara_paper_2) {
                    float img_w = al_get_bitmap_width(game->puzzle_bhaskara_paper_2);
                    float img_h = al_get_bitmap_height(game->puzzle_bhaskara_paper_2);
                    float x = (SCREEN_W - img_w) / 2.0;
                    float y = (SCREEN_H - img_h) / 2.0;
                    
                    al_draw_scaled_bitmap(
                        game->puzzle_bhaskara_paper_2,
                        0, 0,
                        256, 384,
                        x, y,
                        256, 384,
                        0
                    );

                    sprintf(game->log_ln1, "Você vê uma tabuleta sobre a fórmula resolutiva.");
                    sprintf(game->log_ln2, "Estranho...");
                } else {
                    al_draw_text(game->title_font, al_map_rgb(255, 0, 0), 
                                SCREEN_W / 2, SCREEN_H / 2, 
                                ALLEGRO_ALIGN_CENTER, "Erro: Imagem do puzzle não encontrada!");
                }
            }

            
            break;

        case PUZZLE_PYTHAGORAS:
            // TALVEZ EU NAO USE

            // // Lógica similar para o puzzle de Pitágoras, mas ainda não temos sprite
            // if (game->puzzle_pythagoras_img) {
            //     float img_w = al_get_bitmap_width(game->puzzle_pythagoras_img);
            //     float img_h = al_get_bitmap_height(game->puzzle_pythagoras_img);
            //     float x = (SCREEN_W - img_w) / 2.0;
            //     float y = (SCREEN_H - img_h) / 2.0;
            //     al_draw_bitmap(game->puzzle_pythagoras_img, x, y, 0);
            // }
            break;

        case PUZZLE_NONE:
        default:
            al_draw_text(game->subtitle_font, al_map_rgb(255, 255, 255), 
                         SCREEN_W / 2, SCREEN_H / 2, 
                         ALLEGRO_ALIGN_CENTER, "Nenhum puzzle ativo.");
            break;
    }

    
    int base_y = 20;
    int bitmap_width = 32;
    int bitmap_x_start = (SCREEN_W / 2) - (bitmap_width / 2);
    int text1_x_center = bitmap_x_start - 50;
    int text2_x_center = bitmap_x_start + 110;

    al_draw_text(
        game->subtitle_11_font,
        al_map_rgb(255, 255, 255), 
        text1_x_center,
        base_y + 7, 
        ALLEGRO_ALIGN_CENTER, 
        "Pressione"
    );

    al_draw_scaled_bitmap(
            game->controls,
            0, 0,
            16, 16,
            bitmap_x_start,
            base_y,     
            bitmap_width, 32,
            0 
        );   

    al_draw_text(
        game->subtitle_11_font, 
        al_map_rgb(255, 255, 255),
        text2_x_center,
        base_y + 7, 
        ALLEGRO_ALIGN_CENTER, 
        "para fechar."
    );
}

void draw_logs(Game* game){
    int log_x_pos = SCREEN_W - 20;

    int exploring_line_height = al_get_font_line_height(game->subtitle_11_font);
    int exploring_spacing = exploring_line_height + 2; 


    int exploring_base_y = 100 - exploring_line_height;

    ALLEGRO_COLOR log_color = al_map_rgb(255, 255, 255);

    al_draw_text(
        game->log_font,
        log_color,
        log_x_pos, 
        exploring_base_y - (exploring_spacing * 2),
        ALLEGRO_ALIGN_RIGHT, 
        game->log_ln1
    );


    al_draw_text(
        game->log_font,
        log_color,
        log_x_pos, 
        exploring_base_y - exploring_spacing,
        ALLEGRO_ALIGN_RIGHT, 
        game->log_ln2
    );

    al_draw_text(
        game->log_font,
        log_color,
        log_x_pos, 
        exploring_base_y,
        ALLEGRO_ALIGN_RIGHT, 
        game->log_ln3
    );
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

                if (current_entity->entity_type == PUZZLE_DIAL) {
                    al_draw_textf(
                        game->subtitle_font,
                        al_map_rgb(255, 255, 255),
                        current_entity->x,
                        current_entity->y - 20,
                        ALLEGRO_ALIGN_CENTER,
                        "%d",
                        current_entity->puzzle_value
                    );
                }

                /*
                AQUI SE FAZ CHECKAGEM DE INTERAÇÃO PARA DESENHO, REFATORAR ----------------------
                */
                if(current_entity && current_entity->entity_type == DOOR){
                    bool check = check_interaction(game->controls, &game->player->entity, current_entity);
                } else if(current_entity && current_entity->entity_type == EDUCATIONAL){
                    bool check = check_interaction(game->controls, &game->player->entity, current_entity);
                } else if(current_entity && current_entity->entity_type == PUZZLE_DIAL){
                    bool check = check_interaction(game->controls, &game->player->entity, current_entity);
                }
            }
            
            draw_entity(&game->player->entity);
            
            for(int i = 0; i < MAX_ENEMIES; i++){
                if(game->mobs[i] && game->mobs[i]->entity.isActive){
                   draw_entity(&game->mobs[i]->entity);
                }    
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

                if (current_entity->entity_type == PUZZLE_DIAL) {
                    al_draw_textf(
                        game->subtitle_font,
                        al_map_rgb(255, 255, 255),
                        current_entity->x,
                        current_entity->y - 20,
                        ALLEGRO_ALIGN_CENTER,
                        "%d",
                        current_entity->puzzle_value
                    );
                }

                /*
                AQUI SE FAZ CHECKAGEM DE INTERAÇÃO PARA DESENHO, REFATORAR ----------------------
                */
                if(current_entity && current_entity->entity_type == DOOR){
                    bool check = check_interaction(game->controls, &game->player->entity, current_entity);
                } else if(current_entity && current_entity->entity_type == EDUCATIONAL){
                    bool check = check_interaction(game->controls, &game->player->entity, current_entity);
                } else if(current_entity && current_entity->entity_type == PUZZLE_DIAL){
                    bool check = check_interaction(game->controls, &game->player->entity, current_entity);
                }
            }

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
            
            for(int i = 0; i < MAX_ENEMIES; i++){
                if(game->mobs[i] && game->mobs[i]->entity.isActive){
                   draw_entity(&game->mobs[i]->entity);
                }    
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

                if (current_entity->entity_type == PUZZLE_DIAL) {
                    al_draw_textf(
                        game->subtitle_font,
                        al_map_rgb(255, 255, 255),
                        current_entity->x,
                        current_entity->y - 20,
                        ALLEGRO_ALIGN_CENTER,
                        "%d",
                        current_entity->puzzle_value
                    );
                }

                /*
                AQUI SE FAZ CHECKAGEM DE INTERAÇÃO PARA DESENHO, REFATORAR ----------------------
                */
                if(current_entity && current_entity->entity_type == DOOR){
                    bool check = check_interaction(game->controls, &game->player->entity, current_entity);
                } else if(current_entity && current_entity->entity_type == EDUCATIONAL){
                    bool check = check_interaction(game->controls, &game->player->entity, current_entity);
                } else if(current_entity && current_entity->entity_type == PUZZLE_DIAL){
                    bool check = check_interaction(game->controls, &game->player->entity, current_entity);
                }
            }

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
            
            for(int i = 0; i < MAX_ENEMIES; i++){
                if(game->mobs[i] && game->mobs[i]->entity.isActive){
                   draw_entity(&game->mobs[i]->entity);
                }    
            }

            al_identity_transform(&identity_transform);
            al_use_transform(&identity_transform);
            break;
        case GAME_PUZZLE_SCREEN:
            draw_puzzle_state(game);
            break;
        case GAME_OVER:
            reset_world_entities(game);

            al_draw_text(game->title_font, al_map_rgb(255, 0, 0), SCREEN_W / 2, SCREEN_H / 2, ALLEGRO_ALIGN_CENTER, "Você morreu!");
            break;
        case GAME_END:
            reset_world_entities(game);

            al_draw_filled_rectangle(0, 0, SCREEN_W, SCREEN_H, al_map_rgba(0,0,0, 255));
            al_draw_text(game->title_font, al_map_rgb(255, 0, 0), SCREEN_W / 2, SCREEN_H / 2, ALLEGRO_ALIGN_CENTER, "Você morreu!");
    }

    switch (game->gameplay_state){

        case GAMEPLAY_PUZZLE:
            int log_x_pos_0 = SCREEN_W - 20;

            int exploring_line_height_0 = al_get_font_line_height(game->subtitle_11_font);
            int exploring_spacing_0 = exploring_line_height_0 + 10; 

            int exploring_base_y_0 = SCREEN_H - 20 - exploring_line_height_0;

            ALLEGRO_COLOR log_color_0 = al_map_rgb(255, 255, 255);

            al_draw_text(
                game->log_font_20,
                log_color_0,
                log_x_pos_0, 
                exploring_base_y_0 - (exploring_spacing_0 * 2),
                ALLEGRO_ALIGN_RIGHT, 
                game->log_ln1
            );


            al_draw_text(
                game->log_font_20,
                log_color_0,
                log_x_pos_0, 
                exploring_base_y_0 - exploring_spacing_0,
                ALLEGRO_ALIGN_RIGHT, 
                game->log_ln2
            );

            al_draw_text(
                game->log_font_20,
                log_color_0,
                log_x_pos_0, 
                exploring_base_y_0,
                ALLEGRO_ALIGN_RIGHT, 
                game->log_ln3
            );
            break;
        case GAMEPLAY_EXPLORING:        
           
            draw_logs(game);
            
            draw_inventory(game->player, game->subtitle_11_font);

            al_draw_scaled_bitmap(
                game->player->player_hp,
                0, 0,
                922, 432,
                20, 20,
                213, 100,
                0
            );
            
            al_draw_textf(game->subtitle_8_font, al_map_rgb(255, 255, 255), SCREEN_W / 2 - 450, SCREEN_H - 60, ALLEGRO_ALIGN_LEFT, "Nível atual: %d", game->player->level);
            al_draw_textf(game->subtitle_8_font, al_map_rgb(255, 255, 255), SCREEN_W / 2 - 450, SCREEN_H - 50, ALLEGRO_ALIGN_LEFT, "XP atual: %d", game->player->xp);
            al_draw_textf(game->subtitle_8_font, al_map_rgb(255, 255, 255), SCREEN_W / 2 - 450, SCREEN_H - 40, ALLEGRO_ALIGN_LEFT, "XP restante para próximo nível: %d", game->player->xp_for_next_level - game->player->xp);

            draw_hp_bar_scalable(game->subtitle_font, 42, 57, 165, 30, game->player->entity.hp, game->player->entity.max_hp);

            draw_defense(game->player->shield, game->subtitle_11_font, 30, 626,game->player->defense);
            draw_attack(game->player->sword_ui, game->subtitle_11_font, 75, 626,game->player->attack);

            break;
        case GAMEPLAY_BATTLE:
            Enemy* enemy_in_battle = game->battle->enemy;
            draw_inventory(game->player, game->subtitle_8_font);

            if(game->battle->turn_state == TURN_PLAYER && game->battle->state == BATTLE_START)
                al_draw_text(game->subtitle_font, al_map_rgb(255, 255, 0), SCREEN_W / 2, (SCREEN_H / 2) - 35, ALLEGRO_ALIGN_CENTER, "Seu turno");
            else if (game->battle->turn_state == TURN_ENEMY && game->battle->state == BATTLE_START)
                al_draw_text(game->subtitle_font, al_map_rgb(255, 255, 0), SCREEN_W / 2, (SCREEN_H / 2) - 35, ALLEGRO_ALIGN_CENTER, "Turno do inimigo");
            else if (game->battle->turn_state == TURN_EMPTY && game->battle->state == BATTLE_START)
                al_draw_text(game->subtitle_font, al_map_rgb(255, 255, 0), SCREEN_W / 2, (SCREEN_H / 2) - 35, ALLEGRO_ALIGN_CENTER, "FIM!");
                
            if(game->battle->state == BATTLE_WIN) {
                
                al_draw_text(game->subtitle_font, al_map_rgb(255, 255, 0), SCREEN_W / 2, SCREEN_H / 2, ALLEGRO_ALIGN_CENTER, "Você ganhou!");

                int base_y = (SCREEN_H / 2) - 120;
                
                int bitmap_width = 32;

                int bitmap_x_start = (SCREEN_W / 2) - (bitmap_width / 2);

                int text1_x_center = bitmap_x_start - 50; 
                int text2_x_center = bitmap_x_start + 180;

                al_draw_text(
                    game->subtitle_11_font,
                    al_map_rgb(255, 255, 0),
                    text1_x_center,
                    base_y + 7,
                    ALLEGRO_ALIGN_CENTER, 
                    "Pressione"
                );

                al_draw_scaled_bitmap(
                        game->controls,
                        0, 0,
                        16, 16,
                        bitmap_x_start,
                        base_y,
                        bitmap_width, 32,
                        0 
                    );   

                al_draw_text(
                    game->subtitle_11_font, 
                    al_map_rgb(255, 255, 0), 
                    text2_x_center,
                    base_y + 7, 
                    ALLEGRO_ALIGN_CENTER, 
                    "para continuar para o jogo!"
                );

            }else if(game->battle->state == BATTLE_LOST)
                al_draw_text(game->subtitle_font, al_map_rgb(255, 255, 0), SCREEN_W / 2, SCREEN_H / 2, ALLEGRO_ALIGN_CENTER, "Você perdeu!");

            if(game->battle->state != BATTLE_DIALOGUE){
                al_draw_scaled_bitmap(
                    game->player->player_hp,
                    0, 0,
                    922, 432,
                    20, 20,
                    213, 100,
                    0
                );

                

                draw_hp_bar_scalable(game->subtitle_font, 42, 57, 165, 30, game->player->entity.hp, game->player->entity.max_hp);

                
                draw_hp_bar_scalable(game->subtitle_font, SCREEN_W - 338, 70, 235, 35, enemy_in_battle->entity.hp, enemy_in_battle->entity.max_hp);

                draw_defense(game->player->shield, game->subtitle_11_font, 30, 626,game->player->defense);
                draw_attack(game->player->sword_ui, game->subtitle_11_font, 75, 626,game->player->attack);

                al_draw_text(game->subtitle_8_font, al_map_rgb(255, 255, 255), SCREEN_W - 400, SCREEN_H / 2 - 200, ALLEGRO_ALIGN_LEFT, "Pressione ESPAÇO para atacar");
                al_draw_text(game->subtitle_8_font, al_map_rgb(255, 255, 255), SCREEN_W - 400, SCREEN_H / 2 - 180, ALLEGRO_ALIGN_LEFT, "Você pode usar um item antes de atacar");
            }

            if(game->battle->turn_state == TURN_PLAYER || game->battle->turn_state == TURN_ENEMY || game->battle->state == BATTLE_WIN) {
                if(game->event->timer.source == game->battle->timer_enemy)
                    al_start_timer(game->battle->log_timer);

                int battle_log_x = SCREEN_W - 20;
                int battle_line_height = al_get_font_line_height(game->subtitle_11_font);
                int battle_line_spacing = battle_line_height + 10; 


                int battle_base_y = SCREEN_H - 20 - battle_line_height;

                ALLEGRO_COLOR battle_log_color = al_map_rgb(255, 255, 255);

                al_draw_text(
                    game->log_font_20, battle_log_color, battle_log_x, 
                    battle_base_y - (battle_line_spacing * 9),
                    ALLEGRO_ALIGN_RIGHT, game->battle->log_ln1
                );
                al_draw_text(
                    game->log_font_20, battle_log_color, battle_log_x, 
                    battle_base_y - (battle_line_spacing * 8),
                    ALLEGRO_ALIGN_RIGHT, game->battle->log_ln2
                );
                al_draw_text(
                    game->log_font_20, battle_log_color, battle_log_x, 
                    battle_base_y - (battle_line_spacing * 7),
                    ALLEGRO_ALIGN_RIGHT, game->battle->log_ln3
                );
                al_draw_text(
                    game->log_font_20, battle_log_color, battle_log_x, 
                    battle_base_y - (battle_line_spacing * 6),
                    ALLEGRO_ALIGN_RIGHT, game->battle->log_ln4
                );
                al_draw_text(
                    game->log_font_20, battle_log_color, battle_log_x, 
                    battle_base_y - (battle_line_spacing * 5),
                    ALLEGRO_ALIGN_RIGHT, game->battle->log_ln5
                );
                al_draw_text(
                    game->log_font_20, battle_log_color, battle_log_x, 
                    battle_base_y - (battle_line_spacing * 4),
                    ALLEGRO_ALIGN_RIGHT, game->battle->log_ln6
                );
                al_draw_text(
                    game->log_font_20, battle_log_color, battle_log_x, 
                    battle_base_y - (battle_line_spacing * 3),
                    ALLEGRO_ALIGN_RIGHT, game->battle->log_ln7
                );
                al_draw_text(
                    game->log_font_20, battle_log_color, battle_log_x, 
                    battle_base_y - (battle_line_spacing * 2),
                    ALLEGRO_ALIGN_RIGHT, game->battle->log_ln8
                );
                al_draw_text(
                    game->log_font_20, battle_log_color, battle_log_x, 
                    battle_base_y - battle_line_spacing,
                    ALLEGRO_ALIGN_RIGHT, game->battle->log_ln9
                );
                al_draw_text(
                    game->log_font_20, battle_log_color, battle_log_x, 
                    battle_base_y,
                    ALLEGRO_ALIGN_RIGHT, game->battle->log_ln10
                );
            }
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

    for(int i = 0; i < MAX_ENEMIES; i++){
        if(game->mobs[i]){
            destroy_enemy(game->mobs[i]);
            game->mobs[i] = NULL;
        }

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

