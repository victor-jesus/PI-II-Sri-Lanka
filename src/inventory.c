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

// Retorna a quantidade de itens consumíveis válidos no inventário
int count_consumable_items(Inventory* inventory) {
    int count = 0;
    for (int i = 0; i < MAX_ITENS; i++) {
        Item* item = inventory->slots[i].item;
        // Verifica se existe, se é consumível e se tem quantidade > 0
        if (item != NULL && (item->type == ITEM_HEAL || item->type == ITEM_SMALL_HEAL || item->type == ITEM_WATER)) {
            if (inventory->slots[i].quantity > 0) {
                count++;
            }
        }
    }
    return count;
}

// Retorna o índice real no inventário ("slot") baseado no índice visual do menu
// Exemplo: Se o item selecionado no menu é o 2º, essa função acha em qual slot do inventário ele está
int get_inventory_slot_by_menu_index(Inventory* inventory, int menu_index) {
    int current_valid_index = 0;
    for (int i = 0; i < MAX_ITENS; i++) {
        Item* item = inventory->slots[i].item;
        if (item != NULL && (item->type == ITEM_HEAL || item->type == ITEM_SMALL_HEAL || item->type == ITEM_WATER)) {
            if (inventory->slots[i].quantity > 0) {
                if (current_valid_index == menu_index) {
                    return i; // Retorna o índice real do slot
                }
                current_valid_index++;
            }
        }
    }
    return -1;
}