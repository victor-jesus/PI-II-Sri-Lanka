#ifndef MAP_H
#define MAP_H

#define MAP_WIDTH 125
#define MAP_HEIGHT 20
#define TILE_W 32
#define TILE_H 32

#include <allegro5/allegro5.h>

typedef struct {
    int tiles[MAP_HEIGHT][MAP_WIDTH];
    ALLEGRO_BITMAP* wall;
    ALLEGRO_BITMAP* floor;
} Map;

void init_map(Map* map, const char* wall, const char* floor);

void destroy_map(Map* map); // Para limpeza

#endif