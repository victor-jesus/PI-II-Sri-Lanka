#ifndef MAP_H
#define MAP_H

#define MAP_WIDTH 125
#define MAP_HEIGHT 20
#define TILE_W 32
#define TILE_H 32

typedef struct {
    int tiles[MAP_HEIGHT][MAP_WIDTH];
} Map;

#endif