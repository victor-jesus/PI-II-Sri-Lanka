#ifndef ENEMY_H
#define ENEMY_H

#include "turn.h"
#include "entity.h"
#include <stdbool.h>

typedef enum {
    MOB,
    BOSSES,
    ARAUTO
} Enemy_type;

typedef struct {
    Entity entity;
    int iniciative;
    bool moving;
    Turn_choice turn_choice;
    ALLEGRO_BITMAP* hp_heart;
    Enemy_type enemy_type;
} Enemy;

void init_enemy(Enemy* enemy, Enemy_type enemy_type, int x, int y, int vx, int hp);
void update_enemy(Enemy* enemy, float dt);
void destroy_enemy(Enemy* enemy);

#endif