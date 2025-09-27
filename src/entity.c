#include "entity.h"
#include "allegro5/allegro_primitives.h"

void init_entity(Entity* entity, int x, int y, int vx, int vy, int hp){
    for(int i = 0; i < QUANT_ANIMATIONS; i++){
        entity->sprite[i] = NULL;
    }

    entity->anim_state = ANIM_IDLE;
    entity->x = x;
    entity->y = y;
    entity->vx = vx;
    entity->vy = vy;
    entity->hp = hp;
    entity->flip = 0;
    entity->isActive = true;
}

void toggle_entity_active(Entity* entity, bool change){
    entity->isActive = change;
}

void set_entity_sprite(Entity* entity, AnimationState state, Sprite* sprite){
    entity->sprite[state] = sprite;
}

void set_entity_anim(Entity* entity, const char* path, AnimationState animation_type, int cols, int rows, float frame_time){
    Sprite* anim = malloc(sizeof(Sprite));
    create_sprite(anim, path, cols, rows, frame_time);
    set_entity_sprite(entity, animation_type, anim);
}

void update_entity(Entity* entity, float dt){
    Sprite* current = entity->sprite[entity->anim_state];
    update_sprite(current, dt);
}

void take_damage(Entity* entity, int amount){
    entity->hp -= amount;

    entity->anim_state = ANIM_HIT;

    if(entity->hp <= 0){
        entity->hp = 0;
        return;
    }
}

void draw_entity(Entity* entity){
    float dx = entity->x;
    float dy = entity->y;

    Sprite* current = entity->sprite[entity->anim_state];
    draw_sprite(current, dx, dy, entity->flip);
}