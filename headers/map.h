#ifndef MAP_H
#define MAP_H

#define MAP_WIDTH 50
#define MAP_HEIGHT 5
#define TILE_W 128
#define TILE_H 128

#include <allegro5/allegro5.h>

typedef struct {
    int tiles[MAP_HEIGHT][MAP_WIDTH];
    ALLEGRO_BITMAP* wall;
    ALLEGRO_BITMAP* floor_2;
    ALLEGRO_BITMAP* floor;
} Map;

void init_map(Map* map, const char* wall, const char* floor, const char* floor_2);

void destroy_map(Map* map); // Para limpeza

#endif