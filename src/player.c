#include "player.h"
#include "allegro5/allegro5.h"



void init_player(Player* player, int max_hp, int x, int y, int vx, int vy, int offset_up, int offset_down, int offset_left, int offset_right){
    init_entity(&player->entity, x, y, vx, vy, max_hp);
    player->iniciative = 10;
}

void set_player_sprite(Player* player, const char* path, AnimationState animation_type, int cols, int rows, float frame_time){
    Sprite* anim = malloc(sizeof(Sprite));
    create_sprite(anim, path, cols, rows, frame_time);
    set_entity_sprite(&player->entity, animation_type, anim);
}

void update_player(Player* player, unsigned char* key, float dt){

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