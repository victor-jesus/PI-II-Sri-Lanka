#include <stdlib.h>
#include <stdio.h>
#include "item.h"

Item* BIG_POTION;
Item* SMALL_POTION;
Item* WATER;
Item* AMULET_OF_STRENGTH;
Item* AMULET_OF_PROTECTION;
Item* AMULET_OF_MINOTAUR;

void load_item_database() {
    BIG_POTION = create_item(path_big_potion, "H", "Poção Grande", "Cura 20 HP.", 20, true, 5, ITEM_HEAL);
    set_entity_pos(BIG_POTION->entity, 550, 680); 

    SMALL_POTION = create_item(path_small_potion, "J", "Poção Pequena", "Cura 10 HP.", 10, true, 5, ITEM_SMALL_HEAL);
    set_entity_pos(SMALL_POTION->entity, 600, 680);

    WATER = create_item(path_water, "K", "Garrafa de agua", "Cura 5 HP.", 5, true, 5, ITEM_WATER);
    set_entity_pos(WATER->entity, 650, 680);

    AMULET_OF_STRENGTH = create_item(path_amulet_atk, "Amuleto de força", "Amuleto da Força", "+5 de Ataque", 5, false, 1, ITEM_EQUIPMENT);
    AMULET_OF_STRENGTH->attack_buff = 5; 
    AMULET_OF_STRENGTH->defense_buff = 0;

    AMULET_OF_PROTECTION = create_item(path_amulet_def, "Amulet de força", "Amuleto da Proteção", "+5 de Defesa", 5, false, 1, ITEM_EQUIPMENT);
    AMULET_OF_PROTECTION->defense_buff = 5; 

    AMULET_OF_MINOTAUR = create_item(path_amulet_atk, "Amulet do minotauro", "Amuleto do minotauro.", " Você sente como se alguém estivesse gritando dentro dele. +15 de Defesa +20 Ataque +5 Iniciativa", 5, false, 1, ITEM_EQUIPMENT);
    AMULET_OF_MINOTAUR->attack_buff = 20;
    AMULET_OF_MINOTAUR->defense_buff = 15;
    AMULET_OF_MINOTAUR->iniciative_buff = 5;
    AMULET_OF_MINOTAUR->max_hp_buff = 10;
}

Item* create_item(const char* path, const char* id, const char* name, const char* description, double value, bool stackable, int max_stack, Item_type type){
    Item* item = (Item*) malloc(sizeof(Item));
    if (item == NULL) {
        printf("ERRO: Falha ao alocar memória para o item!\n");
        return NULL;
    }

    item->entity = (Entity*) malloc(sizeof(Entity));
    if (item->entity == NULL) {
        printf("ERRO: Falha ao alocar memória para a entidade do item!\n");
        free(item);
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

