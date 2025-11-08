#ifndef ENTITY_H
#define ENTITY_H

#include <sprite.h>
#include <animation_state.h>
#include <stdbool.h>
#include "box.h"

#define QUANT_ANIMATIONS 5

typedef enum{
    DOOR,
    KEY,
    DIALOGUE,
    ENVIRONMENT_NO_MOVE,
    ENVIRONMENT_MOVE,
    INVENTORY,
    UI,
    CHARACTER
} Entity_type;

typedef struct {
    Sprite* sprite[QUANT_ANIMATIONS];
    AnimationState anim_state;
    Box box;
    int x, y, vx, vy;
    int flip;
    int hp;
    int offset_up, offset_down, offset_left, offset_right;
    float scale_factor;
    Entity_type entity_type;
    bool isActive;
} Entity;

void init_entity(Entity* entity, int x, int y, int vx, int vy, int hp, Entity_type type);
void set_entity_pos(Entity* entity, int x, int y);
void set_entity_sprite(Entity* entity, AnimationState state, Sprite* sprite);
void set_entity_scale(Entity* entity, float scale_factor);
void set_hit_box(Entity* entity, int offset_up, int offset_down, int offset_left, int offset_right);
void set_entity_anim(Entity* entity, const char* path, AnimationState animation_type, int cols, int rows, float frame_time);
void toggle_entity_active(Entity* entity, bool change);
void update_entity(Entity* entity, float dt);
void update_hit_box(Entity* entity);
void take_damage(Entity* entity, int amount);
void draw_entity(Entity* entity);

#endif