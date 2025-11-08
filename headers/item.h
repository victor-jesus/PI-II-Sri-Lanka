#ifndef ITEM_H
#define ITEM_H

#include <stdbool.h>
#include "entity.h"
#include "item_type.h"

typedef struct {
    const char* id;
    unsigned char* name;
    unsigned char* description;
    double value;
    bool stackable;
    int max_stack;
    Item_type type;
    Entity* entity;
} Item;

Item* create_item(const char* path, const char* id, const char* name, const char* description, double value, bool stackable, int max_stack, Item_type type);

#endif