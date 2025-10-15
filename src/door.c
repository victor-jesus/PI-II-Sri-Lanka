#include "door.h"

void init_door(Door* door){
    init_entity(&door->entity, 0, 0, 0, 0, 1, ENVIRONMENT_NO_MOVE);
}

void set_door_type(Door* door, Door_type door_type){
    door->door_type = door_type;
}