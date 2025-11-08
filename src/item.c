#include <stdlib.h>
#include <stdio.h>
#include "item.h"

Item* create_item(const char* path, const char* id, const char* name, const char* description, double value, bool stackable, int max_stack, Item_type type){
    Item* item = (Item*) malloc(sizeof(Item));
    if (item == NULL) {
        printf("ERRO: Falha ao alocar memória para o item!\n");
        return NULL;
    }

    item->entity = (Entity*) malloc(sizeof(Entity));
    if (item->entity == NULL) {
        printf("ERRO: Falha ao alocar memória para a entidade do item!\n");
        free(item); // Libera o item que já foi alocado
        return NULL;
    }

    init_entity(item->entity, 0, 0, 0, 0, 1, DIALOGUE);
    set_entity_anim(item->entity, path, ANIM_IDLE, 1, 1, 0.1f);
    set_hit_box(item->entity, 0, 0, 0, 0);  

    item->id = id;
    item->name = name;
    item->description = description;
    item->value = value;
    item->stackable = stackable;
    item->max_stack = max_stack;
    item->type = type;
    return item;
}