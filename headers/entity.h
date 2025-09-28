#ifndef ENTITY_H
#define ENTITY_H

#include <sprite.h>
#include <animation_state.h>
#include <stdbool.h>

#define QUANT_ANIMATIONS 5

typedef struct {
    Sprite* sprite[QUANT_ANIMATIONS];
    AnimationState anim_state;
    int x, y, vx, vy;
    int flip;
    int hp;
    int offset_up, offset_down, offset_left, offset_right;
    bool isActive;
} Entity;

void init_entity(Entity* entity, int x, int y, int vx, int vy, int hp);
void set_entity_sprite(Entity* entity, AnimationState state, Sprite* sprite);
void set_entity_anim(Entity* entity, const char* path, AnimationState animation_type, int cols, int rows, float frame_time);
void toggle_entity_active(Entity* entity, bool change);
void update_entity(Entity* entity, float dt);
void take_damage(Entity* entity, int amount);
void draw_entity(Entity* entity);

#endif