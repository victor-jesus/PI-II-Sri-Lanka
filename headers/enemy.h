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
    const char* name;
    Entity entity;
    int iniciative;
    bool moving;
    Turn_choice turn_choice;
    ALLEGRO_BITMAP* hp_heart;
    ALLEGRO_BITMAP* shield_enemy;
    Enemy_type enemy_type;
    int attack, defense;
} Enemy;

void init_enemy(Enemy* enemy, const char* name, Enemy_type enemy_type, int x, int y, int vx, int attack, int defense, int hp, int offset_up, int offset_down, int offset_left, int offset_right);
void update_enemy(Enemy* enemy, float dt);
void destroy_enemy(Enemy* enemy);

#endif