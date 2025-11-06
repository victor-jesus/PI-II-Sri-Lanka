#ifndef INVENTORY_H
#define INVENTORY_H

#include "item.h"

#define MAX_ITENS 20

typedef struct {
    Item* item;
    int quantity;
} Inventory_slot;

typedef struct {
    Inventory_slot slots[MAX_ITENS];
} Inventory;

void init_inventory(Inventory* inventory);
void add_item(Inventory* inventory, Item* item, int amount);
void clear_inventory(Inventory* inventory);
void destroy_inventory(Inventory* inventory);

#endif