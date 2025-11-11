#ifndef INVENTORY_H
#define INVENTORY_H

#include "item.h"
#include <stdbool.h>

#define MAX_ITENS 20

typedef struct {
    Item* item;
    int quantity;
} Inventory_slot;

typedef struct {
    Inventory_slot slots[MAX_ITENS];
} Inventory;

void init_inventory(Inventory* inventory);
bool inventory_add_item(Inventory* inventory, Item* item_to_add, int amount);
void clear_inventory(Inventory* inventory);
void destroy_inventory(Inventory* inventory);

#endif