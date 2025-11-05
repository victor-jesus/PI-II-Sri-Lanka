#include <stdlib.h>
#include <stdio.h>
#include "item.h"

Item* create_item(int id, const char* name, const char* description, double value, bool stackable, int max_stack, Item_type type){
    Item* item = (Item*) malloc(sizeof(Item));
    if (item == NULL) {
        printf("ERRO: Falha ao alocar memória para o item!\n");
        return NULL;
    }
    item->id = id;
    item->name = name;
    item->description = description;
    item->value = value;
    item->stackable = stackable;
    item->max_stack = max_stack;
    item->type = type;
    return item;
}