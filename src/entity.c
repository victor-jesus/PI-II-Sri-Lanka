#include "entity.h"
#include "allegro5/allegro_primitives.h"

void init_entity(Entity* entity, int x, int y, int vx, int hp){
    entity->x = x;
    entity->y = y;
    entity->vx = vx;
    entity->hp = hp;
    entity->isActive = true;
}

void update_entity(Entity* entity, const char* key){

}

void take_damage(Entity* entity, int amount){
    entity->hp -= amount;

    if(entity->hp <= 0){
        entity->hp = 0;
        return;
    }
}

void draw_entity(Entity* entity){
    al_draw_filled_circle(entity->x, entity->y, 15, al_map_rgb(255, 255, 0));
}