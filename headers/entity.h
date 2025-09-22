#ifndef ENTITY_H
#define ENTITY_H

#include <stdbool.h>

typedef struct {
    int x, y, vx;
    int hp;
    bool isActive;
} Entity;

void init_entity(Entity* entity, int x, int y, int vx, int hp);
void update_entity(Entity* entity, const char* key);
void take_damage(Entity* entity, int amount);
void draw_entity(Entity* entity);

#endif