#include "player.h"
#include "allegro5/allegro5.h"

void init_player(Player* player, int pos_x, int pos_y, int vx, int hp){
    init_entity(&player->entity, pos_x, pos_y, vx, hp);
    player->iniciative = 10;
}

void update_player(Player* player, const char* key){
    if(key[ALLEGRO_KEY_D]) player->entity.x += player->entity.vx;
    if(key[ALLEGRO_KEY_A]) player->entity.x -= player->entity.vx;
}

