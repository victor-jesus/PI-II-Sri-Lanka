#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "turn.h"
#include <stdbool.h>

struct Battle;

typedef struct {
    Entity entity;
    int iniciative;
    int hp, max_hp;
    Turn_choice turn_choice;
    bool moving;
} Player;

void init_player(Player* player, int max_hp, int x, int y, int vx, int vy);
void update_player_battle(struct Battle* battle, float dt);
void update_player(Player* player, unsigned char* key, float dt);

#endif