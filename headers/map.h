#ifndef MAP_H
#define MAP_H

#define MAP_W 150
#define MAP_H 23
#define TILE_W 32
#define TILE_H 32

typedef struct {
    int tiles[MAP_H][MAP_W];
} Map;

#endif