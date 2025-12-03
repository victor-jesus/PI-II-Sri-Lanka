#ifndef DOOR_H
#define DOOR_H

#include "entity.h"

typedef enum{
    DOOR_NEXT_LEVEL,
    DOOR_RETURN,
    DOOR_MINOTAUR,
    DOOR_MEDUSA,
    DOOR_ARAUTO,
    DOOR_WEREWOLF,
    DOOR_EASTER_EGG
} Door_type;

typedef struct {
    Entity entity;
    Door_type door_type;
} Door;

void init_door(Door* door);
void set_door_type(Door* door, Door_type door_type);

#endif 

