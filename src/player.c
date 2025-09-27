#include "player.h"
#include "allegro5/allegro5.h"
#include <stdio.h>

void init_player(Player* player, int max_hp, int x, int y, int vx, int vy){
    init_entity(&player->entity, x, y, vx, vy, max_hp);
    player->iniciative = 10;
}


// ISSO TRVA O GAME <BATTLE>
void update_player_battle(Player* player, float dt){

    if(player->entity.anim_state == ANIM_HIT){
        Sprite* hit = player->entity.sprite[ANIM_HIT];
        update_sprite(hit, dt);

        if(hit->current_frame == hit->cols - 1){
            player->entity.anim_state = ANIM_IDLE;
            hit->current_frame = 0;
            hit->elapsed = 0;
        }
        return;
    }

    if(player->entity.anim_state == ANIM_ATTACK){
        Sprite* attack = player->entity.sprite[ANIM_ATTACK];

        do{
            update_sprite(attack, dt);
        }
        while(attack->current_frame == attack->cols - 1);
        player->entity.anim_state = ANIM_IDLE;
        attack->current_frame = 0;
        attack->elapsed = 0;
        
        return;
    }

    Sprite* current = player->entity.sprite[player->entity.anim_state];
    update_sprite(current, dt);
}

void update_player(Player* player, unsigned char* key, float dt){
    player->moving = false;

    if(player->entity.anim_state == ANIM_HIT){
        Sprite* hit = player->entity.sprite[ANIM_HIT];
        update_sprite(hit, dt);

        if(hit->current_frame == hit->cols - 1){
            player->entity.anim_state = ANIM_IDLE;
            hit->current_frame = 0;
            hit->elapsed = 0;
        }
        return;
    }

    if(player->entity.anim_state == ANIM_ATTACK){
        
        Sprite* attack = player->entity.sprite[ANIM_ATTACK];
        update_sprite(attack, dt);
        if(attack->current_frame == attack->cols - 1){
            player->entity.anim_state = ANIM_IDLE;
            attack->current_frame = 0;
            attack->elapsed = 0;
            return;
        }
    }

    if (key[ALLEGRO_KEY_A] || key[ALLEGRO_KEY_LEFT]) { 
        player->entity.x -= player->entity.vx; 
        player->entity.flip = ALLEGRO_FLIP_HORIZONTAL;
        player->moving = true; 
    }
    if (key[ALLEGRO_KEY_D] ||  key[ALLEGRO_KEY_RIGHT]) { 
        player->entity.x += player->entity.vx; 
        player->entity.flip = 0;
        player->moving = true; 
    }
    
    if(player->moving){
        player->entity.anim_state = ANIM_RUN;
    } else {
        player->entity.anim_state = ANIM_IDLE;
    }
    
    Sprite* current = player->entity.sprite[player->entity.anim_state];
    update_sprite(current, dt);
}

void destroy_sprite_from_player(Player* player){
    for(int i = 0; i < QUANT_ANIMATIONS; i++){
        destroy_sprite(player->entity.sprite[i]);
    }
}