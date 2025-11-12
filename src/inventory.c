#include <stdlib.h>
#include <stdio.h>
#include "inventory.h"

void init_inventory(Inventory* inventory){
    for(int i = 0; i < MAX_ITENS; i++){
        inventory->slots[i].item = NULL;
        inventory->slots[i].quantity = 0;
    }
}

bool inventory_add_item(Inventory* inventory, Item* item_to_add, int amount) {
    if (amount <= 0) return true;

    if (item_to_add->stackable) {
        for (int i = 0; i < MAX_ITENS; i++) {
            Inventory_slot* slot = &inventory->slots[i];

            if (slot->item == item_to_add && slot->quantity < item_to_add->max_stack) {
                
                int space_left = item_to_add->max_stack - slot->quantity;
                
                int amount_to_add_now = (amount < space_left) ? amount : space_left;
                
                slot->quantity += amount_to_add_now;
                amount -= amount_to_add_now;

                if (amount <= 0) return true; 
            }
        }
    }

    for (int i = 0; i < MAX_ITENS; i++) {
        if (amount <= 0) return true; 

        Inventory_slot* slot = &inventory->slots[i];
        if (slot->item == NULL) {
            slot->item = item_to_add;
            
            int amount_to_add_now = (amount < item_to_add->max_stack) ? amount : item_to_add->max_stack;
            
            slot->quantity = amount_to_add_now;
            amount -= amount_to_add_now;

        }
    }

    if (amount > 0) {
        printf("Inventário Cheio! Sobraram %d de %s.\n", amount, item_to_add->name);
        return false; 
    }
    return true; 
}