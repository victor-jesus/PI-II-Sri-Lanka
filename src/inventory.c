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
    if (amount <= 0) return true; // Nada a adicionar

    // --- PASSO 1: TENTAR EMPILHAR EM SLOTS EXISTENTES ---
    if (item_to_add->stackable) {
        for (int i = 0; i < MAX_ITENS; i++) {
            Inventory_slot* slot = &inventory->slots[i];

            // Se é o item certo E o slot não está cheio
            if (slot->item == item_to_add && slot->quantity < item_to_add->max_stack) {
                
                // Calcula quanto espaço livre tem neste stack
                int space_left = item_to_add->max_stack - slot->quantity;
                
                // Descobre quanto realmente podemos adicionar
                int amount_to_add_now = (amount < space_left) ? amount : space_left;
                
                slot->quantity += amount_to_add_now;
                amount -= amount_to_add_now; // "Consome" o que foi adicionado

                // Se já adicionamos tudo, pode sair
                if (amount <= 0) return true; 
            }
        }
    }

    // --- PASSO 2: TENTAR COLOCAR EM SLOTS VAZIOS ---
    // (Esta parte só roda se 'amount' ainda for > 0)
    for (int i = 0; i < MAX_ITENS; i++) {
        if (amount <= 0) return true; // Garante que saia se o loop anterior terminou

        Inventory_slot* slot = &inventory->slots[i];

        // Se o slot está vazio...
        if (slot->item == NULL) {
            slot->item = item_to_add; // Coloca o item (ponteiro para o molde)
            
            // Descobre quanto podemos colocar (respeitando o max_stack)
            int amount_to_add_now = (amount < item_to_add->max_stack) ? amount : item_to_add->max_stack;
            
            slot->quantity = amount_to_add_now;
            amount -= amount_to_add_now; // Consome o que foi adicionado
            
            // Se 'amount' ainda for > 0, o loop continua para
            // encontrar o PRÓXIMO slot vazio.
        }
    }

    // Se saiu dos loops e 'amount' ainda é > 0, o inventário está cheio
    if (amount > 0) {
        printf("Inventário Cheio! Sobraram %d de %s.\n", amount, item_to_add->name);
        return false; // Falha (parcial ou total)
    }
    
    return true; // Sucesso
}