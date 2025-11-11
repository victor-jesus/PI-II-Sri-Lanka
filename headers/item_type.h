#ifndef ITEM_TYPE_H
#define ITEM_TYPE_H

typedef enum{
    ITEM_HEAL,
    ITEM_SMALL_HEAL,
    ITEM_WATER,
    ITEM_EQUIPMENT,  // <-- Adicionado (para Amuletos, Anéis, etc.)
    ITEM_QUEST,      // <-- Adicionado (para itens de história)
    ITEM_NONE
} Item_type;

#endif