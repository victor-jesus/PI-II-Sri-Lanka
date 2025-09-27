#ifndef ENEMY_H
#define ENEMY_H

#include "turn.h"
#include "entity.h"

typedef struct {
    Entity entity;
    int iniciative;
    Turn_choice turn_choice;
} Enemy;

void init_enemy(Enemy* enemy, int x, int y, int vx, int hp);
void update_enemy(Entity* entity, float dt);

#endif