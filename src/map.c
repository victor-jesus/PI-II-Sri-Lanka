#include "map.h"

#include <stdio.h> 
#include <stdlib.h> 
#include <allegro5/allegro5.h>

static ALLEGRO_BITMAP* load_checked_tile(const char* path, const char* name) {
    ALLEGRO_BITMAP* bmp = al_load_bitmap(path);
    if (!bmp) {
        printf("ERRO FATAL: Nao foi possivel carregar o tile %s em: %s\n", name, path);
        exit(1);
    }
    return bmp;
}

void init_map(Map* map, const char* wall_path, const char* floor_path) {
    if (!map) {
        fprintf(stderr, "ERRO FATAL: Ponteiro 'map' invalido em init_map.\n");
        exit(1);
    }
    
    map->wall = load_checked_tile(wall_path, "wall");
    map->floor = load_checked_tile(floor_path, "floor");
}

void destroy_map(Map* map) {
    if (map) {
        if (map->wall) {
            al_destroy_bitmap(map->wall);
            map->wall = NULL;
        }
        
        if (map->floor) {
            al_destroy_bitmap(map->floor);
            map->floor = NULL;
        }
    }
}