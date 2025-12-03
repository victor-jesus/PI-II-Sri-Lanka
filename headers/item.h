#ifndef ITEM_H
#define ITEM_H

#include <stdbool.h>
#include "entity.h"
#include "item_type.h"

static const char* path_water = "assets/sprites/transparent/water-potion.png";
static const char* path_small_potion = "assets/sprites/transparent/small-potion.png";
static const char* path_big_potion = "assets/sprites/transparent/big-potion.png";
static const char* path_amulet_atk = "assets/sprites/transparent/amulet_attack.png";
static const char* path_amulet_def = "assets/sprites/transparent/amulet_defense.png";

typedef struct {
    const char* id;
    unsigned char* name;
    unsigned char* description;
    double value;
    bool stackable;
    int max_stack;
    Item_type type;
    Entity* entity;

    int attack_buff;
    int defense_buff;
    int iniciative_buff;
    int max_hp_buff;
} Item;

Item* create_item(const char* path, const char* id, const char* name, const char* description, double value, bool stackable, int max_stack, Item_type type);
void load_item_database();

extern Item* BIG_POTION;
extern Item* SMALL_POTION;
extern Item* WATER;

// MINOTAUR
extern Item* AMULET_OF_MINOTAUR;
extern Item* AMULET_OF_STRENGTH_MINOR;

// COMMON
extern Item* AMULET_OF_STRENGTH;
extern Item* AMULET_OF_PROTECTION;

// MEDUSA
extern Item* AMULET_OF_STRENGTH_MAJOR;
extern Item* AMULET_OF_MEDUSA;
extern Item* AMULET_OF_PROTECTION_MAJOR;

// Keys
extern Item* KEY_TO_MINOTAUR;
extern Item* KEY_TO_MEDUSA;
extern Item* KEY_TO_ARAUTO;
extern Item* KEY_TO_SECOND_MAP;
extern Item* KEY_TO_THIRD_MAP;
extern Item* KEY_TO_EASTER_EGG;

#endif