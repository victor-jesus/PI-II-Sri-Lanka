#ifndef SPRITE_H
#define SPRITE_H

#include <allegro5/allegro5.h>

typedef struct {
    ALLEGRO_BITMAP* bitmap;
    int frame_x, frame_y;
    int frame_w, frame_h;
    int cols, rows;
    int frame_count;
    int current_frame;
    int current_row;
    float frame_time;
    float elapsed;
    int flags;
} Sprite;

void create_sprite(Sprite* sprite, const char* path, int cols, int rows, float frame_time);
void update_sprite(Sprite* sprite, float dt);
void draw_sprite(Sprite* sprite, int dx, int dy, int flip, float size);
void destroy_sprite(Sprite* sprite);

#endif