#include "enemy.h"
#include "allegro5/allegro5.h"

void init_enemy(Enemy* enemy, Enemy_type enemy_type, int x, int y, int vx, int hp, int offset_up, int offset_down, int offset_left, int offset_right){
    init_entity(&enemy->entity, x, y, vx, 5, hp, CHARACTER);
    enemy->enemy_type = enemy_type;
    enemy->moving = false;
    enemy->iniciative = 0;

    set_hit_box(&enemy->entity, offset_up, offset_down, offset_left, offset_right);
}

/*
    Aqui vamos atualizar conforme uma função que cria um sistema de "IA" para o modo exploração e batalha
*/
void update_enemy(Enemy* enemy, float dt){
    update_hit_box(&enemy->entity);
    enemy->moving = false;

    if(enemy->entity.anim_state == ANIM_DEATH){

        Sprite* death = enemy->entity.sprite[ANIM_DEATH];

        if(death->current_frame < death->cols - 1)
            update_sprite(death, dt);
            return;

        return;
    }
            

    if(enemy->entity.anim_state == ANIM_HIT){
        Sprite* hit = enemy->entity.sprite[ANIM_HIT];
        update_sprite(hit, dt);

        if(hit->current_frame == 0 && hit->elapsed < dt){
            enemy->entity.anim_state = ANIM_IDLE;
            hit->current_frame = 0;
            hit->elapsed = 0;
        }
        return;
    }

    if(enemy->entity.anim_state == ANIM_ATTACK){
        Sprite* attack = enemy->entity.sprite[ANIM_ATTACK];
        update_sprite(attack, dt);
        if(attack->current_frame == 0 && attack->elapsed < dt){
            enemy->entity.anim_state = ANIM_IDLE;
            attack->current_frame = 0;
            attack->elapsed = 0;
        }
        return;
    }

      

    if(enemy->moving){
        enemy->entity.anim_state = ANIM_RUN;
        enemy->entity.x += enemy->entity.vx * dt;
        enemy->entity.y += enemy->entity.vy * dt;
    } else {
        enemy->entity.anim_state = ANIM_IDLE;
    }
    
    Sprite* current = enemy->entity.sprite[enemy->entity.anim_state];
    update_sprite(current, dt);
}

void destroy_enemy(Enemy* enemy){
    free(enemy);
}