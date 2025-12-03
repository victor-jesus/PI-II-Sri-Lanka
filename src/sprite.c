#include "sprite.h"
#include <stdio.h>
#include <allegro5/allegro5.h>

void create_sprite(Sprite* sprite, const char* path, int cols, int rows, float frame_time){

    sprite->bitmap = al_load_bitmap(path);
    if (!sprite->bitmap) {
        printf("Falha ao carregar bitmap: %s\n", path);
        exit(1);
    }

    sprite->cols = cols;
    sprite->rows = rows;
    sprite->frame_w = al_get_bitmap_width(sprite->bitmap) / cols;
    sprite->frame_h = al_get_bitmap_height(sprite->bitmap) / rows;
    sprite->frame_time = frame_time;
    sprite->current_frame = 0;
    sprite->current_row = 0;
    sprite->elapsed = 0;
    sprite->flags = 0;
}

void update_sprite(Sprite* sprite, float dt){
    if (!sprite) return;

    sprite->elapsed += dt; 

    while(sprite->elapsed >= sprite->frame_time){
        sprite->elapsed -= sprite->frame_time;  
        sprite->current_frame++;
        if(sprite->current_frame >= sprite->cols){
            sprite->current_frame = 0;
        }
    }
}

void draw_sprite(Sprite* sprite, int dx, int dy, int flip, float size){
    if (!sprite) return;

    int sx = sprite->current_frame * sprite->frame_w;
    int sy = sprite->current_row * sprite->frame_h;
    float dw = sprite->frame_w * size;
    float dh = sprite->frame_h * size;

    al_draw_scaled_bitmap(sprite->bitmap, sx, sy, sprite->frame_w, sprite->frame_h, dx, dy, dw, dh, flip);
}

void destroy_sprite(Sprite* sprite){
    if(sprite){
        if(sprite->bitmap){
            al_destroy_bitmap(sprite->bitmap);
        }
        //free(sprite);
    }
}

