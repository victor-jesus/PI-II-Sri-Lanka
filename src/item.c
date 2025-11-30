#include <stdlib.h>
#include <stdio.h>
#include "item.h"

Item* BIG_POTION;
Item* SMALL_POTION;
Item* WATER;
Item* AMULET_OF_STRENGTH_MINOR;
Item* AMULET_OF_STRENGTH;
Item* AMULET_OF_PROTECTION;
Item* AMULET_OF_MINOTAUR;
Item* AMULET_OF_STRENGTH_MAJOR;
Item* AMULET_OF_MEDUSA;
Item* AMULET_OF_PROTECTION_MAJOR;


Item* KEY_TO_SECOND_MAP;
Item* KEY_TO_THIRD_MAP;

Item* KEY_TO_MINOTAUR;
Item* KEY_TO_MEDUSA;
Item* KEY_TO_ARAUTO;
Item* KEY_TO_EASTER_EGG;

void load_item_database() {
    BIG_POTION = create_item(path_big_potion, "H", "Poção Grande", "Cura 20 HP.", 20, true, 10, ITEM_HEAL);
    set_entity_scale(BIG_POTION->entity, 1.5);
    set_entity_pos(BIG_POTION->entity, 530, 640); 

    SMALL_POTION = create_item(path_small_potion, "J", "Poção Pequena", "Cura 10 HP.", 10, true, 15, ITEM_SMALL_HEAL);
    set_entity_scale(SMALL_POTION->entity, 1.5);
    set_entity_pos(SMALL_POTION->entity, 600, 640);

    WATER = create_item(path_water, "K", "Garrafa de agua", "Cura 5 HP.", 5, true, 5, ITEM_WATER);
    set_entity_scale(WATER->entity, 1.5);
    set_entity_pos(WATER->entity, 670, 640);

    AMULET_OF_STRENGTH = create_item(path_amulet_atk, "Amuleto de força", "Amuleto da Força", "+5 de Ataque", 5, false, 1, ITEM_EQUIPMENT);
    AMULET_OF_STRENGTH->attack_buff = 5;
    AMULET_OF_STRENGTH->defense_buff = 0; 
    AMULET_OF_STRENGTH->iniciative_buff = 4;
    AMULET_OF_STRENGTH->max_hp_buff = 5;

    AMULET_OF_PROTECTION = create_item(path_amulet_def, "Amuleto de Defesa", "Amuleto da Proteção", "+5 de Defesa", 5, false, 1, ITEM_EQUIPMENT);
    AMULET_OF_PROTECTION->attack_buff = 0;
    AMULET_OF_PROTECTION->defense_buff = 5; 
    AMULET_OF_PROTECTION->iniciative_buff = 0;
    AMULET_OF_PROTECTION->max_hp_buff = 10;

    AMULET_OF_MINOTAUR = create_item(path_amulet_atk, "Amuleto do minotauro", "Amuleto do minotauro.", "Você sente como se alguém estivesse gritando dentro dele. +15 de Defesa +20 Ataque +5 Iniciativa +15 hp", 5, false, 1, ITEM_EQUIPMENT);
    AMULET_OF_MINOTAUR->attack_buff = 30;
    AMULET_OF_MINOTAUR->defense_buff = 25;
    AMULET_OF_MINOTAUR->iniciative_buff = 10;
    AMULET_OF_MINOTAUR->max_hp_buff = 40;

    KEY_TO_MINOTAUR = create_item(path_amulet_atk, "minotauro_chave", "Chave para a porta do minotauro.", "Chave simples e enferrujada", 0, false, 1, ITEM_EQUIPMENT);
    KEY_TO_MINOTAUR->attack_buff = 0;
    KEY_TO_MINOTAUR->defense_buff = 0;
    KEY_TO_MINOTAUR->iniciative_buff = 0;
    KEY_TO_MINOTAUR->max_hp_buff = 0;

    KEY_TO_SECOND_MAP = create_item(path_amulet_atk, "segundomapa_chave", "Chave para a ultima porta.", "Chave com adornos.", 0, false, 1, ITEM_EQUIPMENT);
    KEY_TO_SECOND_MAP->attack_buff = 0;
    KEY_TO_SECOND_MAP->defense_buff = 0;
    KEY_TO_SECOND_MAP->iniciative_buff = 0;
    KEY_TO_SECOND_MAP->max_hp_buff = 0;

    KEY_TO_THIRD_MAP = create_item(path_amulet_atk, "terceiromapa_chave", "Chave para a ultima porta.", "Chave com adornos.", 0, false, 1, ITEM_EQUIPMENT);
    KEY_TO_THIRD_MAP->attack_buff = 0;
    KEY_TO_THIRD_MAP->defense_buff = 0;
    KEY_TO_THIRD_MAP->iniciative_buff = 0;
    KEY_TO_THIRD_MAP->max_hp_buff = 0;

    KEY_TO_MEDUSA = create_item(path_amulet_atk, "medusa_chave", "Chave para a ultima porta.", "Chave com adornos.", 0, false, 1, ITEM_EQUIPMENT);
    KEY_TO_MEDUSA->attack_buff = 0;
    KEY_TO_MEDUSA->defense_buff = 0;
    KEY_TO_MEDUSA->iniciative_buff = 0;
    KEY_TO_MEDUSA->max_hp_buff = 0;

    KEY_TO_ARAUTO = create_item(path_amulet_atk, "arauto_chave", "Chave para a ultima porta.", "Chave com adornos.", 0, false, 1, ITEM_EQUIPMENT);
    KEY_TO_ARAUTO->attack_buff = 0;
    KEY_TO_ARAUTO->defense_buff = 0;
    KEY_TO_ARAUTO->iniciative_buff = 0;
    KEY_TO_ARAUTO->max_hp_buff = 0;

    KEY_TO_EASTER_EGG = create_item(path_amulet_atk, "easteregg_chave", "Chave para a ultima porta.", "Chave com adornos.", 0, false, 1, ITEM_EQUIPMENT);
    KEY_TO_EASTER_EGG->attack_buff = 0;
    KEY_TO_EASTER_EGG->defense_buff = 0;
    KEY_TO_EASTER_EGG->iniciative_buff = 0;
    KEY_TO_EASTER_EGG->max_hp_buff = 0;

    AMULET_OF_MEDUSA = create_item(path_amulet_atk, "Amuleto da medusa", "Amuleto da Medusa.", "Amuleto com intenso brilho verde, você guarda dentro da sua bolsa. +35 de Defesa +40 de Ataque +20 de Iniciativa +30 hp", 5, false, 1, ITEM_EQUIPMENT);
    AMULET_OF_MEDUSA->attack_buff = 35;
    AMULET_OF_MEDUSA->defense_buff = 40;
    AMULET_OF_MEDUSA->iniciative_buff = 20;
    AMULET_OF_MEDUSA->max_hp_buff = 60;

    AMULET_OF_STRENGTH_MAJOR = create_item(path_amulet_atk, "Amuleto de grande força", "Amuleto de Grande Força", "+25 de Ataque +3 de Defesa +5 Iniciativa +5 de HP", 5, false, 1, ITEM_EQUIPMENT);
    AMULET_OF_STRENGTH_MAJOR->attack_buff = 25;
    AMULET_OF_STRENGTH_MAJOR->defense_buff = 3; 
    AMULET_OF_STRENGTH_MAJOR->iniciative_buff = 5;
    AMULET_OF_STRENGTH_MAJOR->max_hp_buff = 5;

    AMULET_OF_PROTECTION_MAJOR = create_item(path_amulet_def, "Amuleto de Proteção Maior", "Amuleto de Proteção Maior", "+25 de Defesa +5 de Ataque +15 de HP", 5, false, 1, ITEM_EQUIPMENT);
    AMULET_OF_PROTECTION_MAJOR->attack_buff = 5;
    AMULET_OF_PROTECTION_MAJOR->defense_buff = 25; 
    AMULET_OF_PROTECTION_MAJOR->iniciative_buff = 0;
    AMULET_OF_PROTECTION_MAJOR->max_hp_buff = 20;


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

