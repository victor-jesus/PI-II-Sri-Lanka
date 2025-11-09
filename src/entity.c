#include "entity.h"
#include "allegro5/allegro_primitives.h"

void init_entity(Entity* entity, int x, int y, int vx, int vy, int hp, Entity_type entity_type){
    for(int i = 0; i < QUANT_ANIMATIONS; i++){
        entity->sprite[i] = NULL;
    }

    entity->anim_state = ANIM_IDLE;
    entity->x = x;
    entity->y = y;
    entity->vx = vx;
    entity->vy = vy;
    entity->hp = hp;
    entity->max_hp = hp;
    entity->flip = 0;
    entity->isActive = true;
    entity->scale_factor = 1.0f; 
    entity->entity_type = entity_type;
}

void set_entity_pos(Entity* entity, int x, int y){
    entity->x = x;
    entity->y = y;

    update_hit_box(entity);
}

void set_entity_scale(Entity* entity, float scale) {
    entity->scale_factor = scale;
    update_hit_box(entity);
}

void toggle_entity_active(Entity* entity, bool change){
    entity->isActive = change;
}

void set_entity_sprite(Entity* entity, AnimationState state, Sprite* sprite){
    entity->sprite[state] = sprite;
}

void set_hit_box(Entity* entity, int offset_up, int offset_down, int offset_left, int offset_right){
    entity->offset_up = offset_up;
    entity->offset_down = offset_down;
    entity->offset_left = offset_left;
    entity->offset_right = offset_right;

    update_hit_box(entity);
}

void set_entity_anim(Entity* entity, const char* path, AnimationState animation_type, int cols, int rows, float frame_time){
    Sprite* anim = malloc(sizeof(Sprite));
    create_sprite(anim, path, cols, rows, frame_time);
    set_entity_sprite(entity, animation_type, anim);
}

void update_entity(Entity* entity, float dt){
    if(entity->entity_type == ENVIRONMENT_NO_MOVE) return;
    Sprite* current = entity->sprite[entity->anim_state];
    
    if (!current) {
        if (entity->sprite[ANIM_IDLE]) {
            entity->anim_state = ANIM_IDLE;
            current = entity->sprite[ANIM_IDLE];
        } else {
            return; 
        }
    }    
    
    update_hit_box(entity);
    update_sprite(current, dt);
}

void update_hit_box(Entity* entity){
    Sprite* sprite = entity->sprite[entity->anim_state];
    if(!sprite) {
        sprite = entity->sprite[ANIM_IDLE];
        if (!sprite) return;
    }
    
    entity->box.x = entity->x + entity->offset_left * entity->scale_factor;
    entity->box.y = entity->y + entity->offset_up  * entity->scale_factor;
    entity->box.w = (entity->sprite[entity->anim_state]->frame_w - (entity->offset_left + entity->offset_right)) * entity->scale_factor;
    entity->box.h = (entity->sprite[entity->anim_state]->frame_h - (entity->offset_up + entity->offset_down)) * entity->scale_factor;
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
    Sprite* current = entity->sprite[entity->anim_state];
    if (!current) {
        current = entity->sprite[ANIM_IDLE];
        if (!current) return;
    }

    float dx, dy;

    dy = entity->box.y - (entity->offset_up * entity->scale_factor);

    if (entity->flip == ALLEGRO_FLIP_HORIZONTAL) {
        dx = entity->box.x - (entity->offset_right * entity->scale_factor);
    } else {
        dx = entity->box.x - (entity->offset_left * entity->scale_factor);
    }

    draw_sprite(current, dx, dy, entity->flip, entity->scale_factor);

    float x1 = entity->box.x;
    float y1 = entity->box.y;
    float x2 = x1 + entity->box.w; 
    float y2 = y1 + entity->box.h;  

    al_draw_rectangle(x1, y1, x2, y2, al_map_rgb(255, 0, 0), 2);
}