#ifndef ENEMY_H
#define ENEMY_H

#include "entity.h"

typedef struct {
    Entity entity;
    int iniciative;
} Enemy;

void init_enemy(Enemy* enemy, int x, int y, int vx, int hp);

#endif