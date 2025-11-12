#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "turn.h"
#include "inventory.h"
#include "item.h"
#include <stdbool.h>

struct Game;

#define MAX_EQUIP_SLOTS 15

typedef struct {
    const char* name;
    Entity entity;

    int iniciative;

    Turn_choice turn_choice;
    int max_itens;
    Inventory inventory;
    Item* equipment[MAX_EQUIP_SLOTS];
    bool moving;

    int level;
    int xp;
    int xp_for_next_level;
    bool is_level_up;

    int attack, defense, base_attack, base_defense;

    ALLEGRO_BITMAP* hp_heart;
    ALLEGRO_BITMAP* shield;
    ALLEGRO_BITMAP* portrait;
    ALLEGRO_BITMAP* player_hp;
    ALLEGRO_BITMAP* sword_ui;

    bool can_act, can_use_item;
    bool is_defending;
} Player;

void init_player(Player* player, const char* name, int max_hp, int x, int y, int vx, int vy, int attack, int defense, int offset_up, int offset_down, int offset_left, int offset_right);
void add_xp(Player* player, int amount);
void level_up(Player* player);
bool buff_levels(Player* player);
void player_equip_item(Player* player, Item* item_to_equip);
void player_recalculate_stats(Player* player);
void update_player_battle(Player* player, unsigned char* key, float dt);
void update_player(Player* player, unsigned char* key, float dt);
void select_item(Player* player, unsigned char* key);
void select_item_battle(Player* player, unsigned char* key);
void destroy_player(Player* player);

#endif