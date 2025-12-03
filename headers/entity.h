#ifndef ENTITY_H
#define ENTITY_H

#include "sprite.h"
#include "animation_state.h"
#include <stdbool.h>
#include "box.h"

#define QUANT_ANIMATIONS 5

typedef enum {
    PUZZLE_NONE = 0,
    PUZZLE_BHASKARA,
    PUZZLE_1GRAU,
    PUZZLE_EQUATION_1GRAU,
    PUZZLE_PITAGORAS,
    PUZZLE_EQUATION_PITAGORAS,
    PUZZLE_NEWTONS_LAWS,
    PUZZLE_DOOR_BHASKARA,
    PUZZLE_EQUATION_BHASKARA,
    PUZZLE_DIAL_1,
    PUZZLE_DIAL_2,
    PUZZLE_DIAL_3
} Puzzle_id;

typedef enum{
    DOOR,
    KEY,
    DIALOGUE,
    ENVIRONMENT_NO_MOVE,
    ENVIRONMENT_MOVE,
    INVENTORY,
    UI,
    EDUCATIONAL,
    PUZZLE_DIAL,
    PUZZLE_LEVER,
    CHARACTER
} Entity_type;

typedef struct {
    Sprite* sprite[QUANT_ANIMATIONS];
    AnimationState anim_state;
    Box box;
    int x, y, vx, vy;
    int flip;
    int hp, base_max_hp,max_hp; 
    int offset_up, offset_down, offset_left, offset_right;
    float scale_factor;
    Entity_type entity_type;
    bool isActive;

    Puzzle_id puzzle_id;
    int puzzle_value;

    bool is_locked_key, is_locked_puzzle;
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