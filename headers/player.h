#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include <stdbool.h>

typedef struct {
    Entity entity;
    int iniciative;
    int hp, max_hp;
    bool moving;
} Player;

void init_player(Player* player, int max_hp, int x, int y, int vx, int vy, int offset_up, int offset_down, int offset_left, int offset_right);
void update_player(Player* player, unsigned char* key, float dt);

#endif