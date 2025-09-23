#include "player.h"
#include "allegro5/allegro5.h"

const char* path_idle = "assets/sprites/samurai/idle.png";
const char* path_run = "assets/sprites/samurai/run.png";
const char* path_attack = "assets/sprites/samurai/attack.png";
const char* path_hit = "assets/sprites/samurai/hurt.png";

void init_player(Player* player, int max_hp, int x, int y, int vx, int vy, int offset_up, int offset_down, int offset_left, int offset_right){
    init_entity(&player->entity, x, y, vx, vy, max_hp);
    player->iniciative = 10;

    Sprite* idle   = malloc(sizeof(Sprite));
    Sprite* run    = malloc(sizeof(Sprite));
    Sprite* attack = malloc(sizeof(Sprite));
    Sprite* hit = malloc(sizeof(Sprite));

    // ajusta frame_time de cada animação
    create_sprite(idle, path_idle, 10, 1, 0.1f);
    create_sprite(run, path_run, 16, 1, 0.06f);
    create_sprite(attack, path_attack, 4, 1, 0.1f);
    create_sprite(hit, path_hit, 1, 1, 0.5f);

    set_entity_sprite(&player->entity, ANIM_IDLE, idle);
    set_entity_sprite(&player->entity, ANIM_RUN, run);
    set_entity_sprite(&player->entity, ANIM_ATTACK, attack);
    set_entity_sprite(&player->entity, ANIM_HIT, hit);

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