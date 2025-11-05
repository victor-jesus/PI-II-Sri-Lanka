#include <stdlib.h>
#include <stdio.h>
#include "inventory.h"

void init_inventory(Inventory* inventory){
    for(int i = 0; i < MAX_ITENS; i++){
        inventory->slots[i].item = NULL;
        inventory->slots[i].quantity = 0;
    }
}

void add_item(Inventory* inventory, Item* item, int amount){
    if(amount <= 0) return;

    if(item->stackable){
        for(int i = 0; i < MAX_ITENS; i++){
            Inventory_slot* slot = &inventory->slots[i];

            if(slot->item == item && slot->quantity < item->max_stack){
                int space_left = item->max_stack - slot->quantity;
                int amount_to_add = (amount < space_left) ? amount : space_left;
                
                slot->quantity += amount_to_add;
                amount -= amount_to_add;

                if (amount <= 0) return;
            }
        }
    }

    for(int i = 0; i < MAX_ITENS; i++){
        if (amount <= 0) return;

        Inventory_slot* slot = &inventory->slots[i];

        if(slot->item == NULL){
            slot->item = item;
            
            int amount_to_add = (amount < item->max_stack) ? amount : item->max_stack;
            
            slot->quantity = amount_to_add;
            amount -= amount_to_add;
        }
    }

    if(amount > 0){
        printf("Invetário Cheio!");
        return;
    }
}
