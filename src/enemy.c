#include "enemy.h"
#include "allegro5/allegro5.h"

void init_enemy(Enemy* enemy, int x, int y, int vx, int hp){
    init_entity(&enemy->entity, x, y, vx, 5, hp);
    enemy->iniciative = 10;
}

/*
    Aqui vamos atualizar conforme uma função que cria um sistema de "IA" para o modo exploração e batalha
*/
void update_enemy(Entity* entity, float dt){


    Sprite* current = entity->sprite[entity->anim_state];
    update_sprite(current, dt);
}

