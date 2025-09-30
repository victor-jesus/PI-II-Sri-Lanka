#include "enemy.h"
#include "allegro5/allegro5.h"

void init_enemy(Enemy* enemy, int x, int y, int vx, int hp){
    init_entity(&enemy->entity, x, y, vx, 5, hp);
    enemy->iniciative = 10;
}


/*
    Aqui vamos atualizar conforme uma função que cria um sistema de "IA" para o modo exploração e batalha
*/
void update_enemy(Enemy* enemy, float dt){

    if(enemy->entity.anim_state == ANIM_ATTACK){
        Sprite* attack = enemy->entity.sprite[ANIM_ATTACK];
        update_sprite(attack, dt);

        if(attack->current_frame == attack->cols - 1){
            enemy->entity.anim_state = ANIM_IDLE;
            attack->current_frame = 0;
            attack->elapsed = 0;
        }
        return;
    }

    if(enemy->entity.anim_state == ANIM_HIT){
        Sprite* hit = enemy->entity.sprite[ANIM_HIT];
        update_sprite(hit, dt);

        if(hit->current_frame == hit->cols - 1){
            enemy->entity.anim_state = ANIM_IDLE;
            hit->current_frame = 0;
            hit->elapsed = 0;
        }
        return;
    }

        if(enemy->entity.anim_state == ANIM_DEATH){
        Sprite* death = enemy->entity.sprite[ANIM_DEATH];
        update_sprite(death, dt);

        if(death->current_frame == death->cols - 1){
            enemy->entity.anim_state = ANIM_IDLE;
            death->current_frame = 0;
            death->elapsed = 0;
        }
        return;
    }

    Sprite* current = enemy->entity.sprite[enemy->entity.anim_state];
    update_sprite(current, dt);
}

