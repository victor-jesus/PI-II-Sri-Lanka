#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"

typedef struct {
    Entity entity;
    int iniciative;
} Player;

void init_player(Player* player, int pos_x, int pos_y, int vx, int hp);
void update_player(Player* player, const char* key);

#endif