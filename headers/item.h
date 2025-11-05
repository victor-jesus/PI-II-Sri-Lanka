#ifndef ITEM_H
#define ITEM_H

#include <stdbool.h>
#include "item_type.h"

typedef struct {
    int id;
    unsigned char* name;
    unsigned char* description;
    double value;
    bool stackable;
    int max_stack;
    Item_type type;
} Item;

Item* create_item(int id, const char* name, const char* description, double value, bool stackable, int max_stack, Item_type type);

#endif