#include "player.h"
#include "game.h"
#include "map.h"
#include "inventory.h"
#include "item_type.h"
#include "allegro5/allegro5.h"
#include <stdio.h>

void init_player(Player* player, const char* name, int max_hp, int x, int y, int vx, int vy, int attack, int defense, int offset_up, int offset_down, int offset_left, int offset_right){
    init_entity(&player->entity, x, y, vx, vy, max_hp, CHARACTER);
    init_inventory(&player->inventory);

    player->name = name;
    player->iniciative = 10;
    player->attack = attack;
    player->defense = defense;
    player->base_attack = attack;
    player->base_defense = defense;
    player->entity.max_hp = max_hp;
    player->entity.base_max_hp = max_hp;

    player->level = 1;
    player->xp = 0;
    player->xp_for_next_level = 100;
    player->is_level_up = false;

    player->can_act = false;
    player->can_use_item = false;

    for (int i = 0; i < MAX_EQUIP_SLOTS; i++) {
        player->equipment[i] = NULL;
    }

    set_hit_box(&player->entity, offset_up, offset_down, offset_left, offset_right);
}

void add_xp(Player* player, int amount) {
    if (player->is_level_up) return;

    player->xp += amount;

    level_up(player);
}

void level_up(Player* player){
    if (player->xp >= player->xp_for_next_level && !player->is_level_up) {
        
        player->xp -= player->xp_for_next_level;
        player->level++;
        player->is_level_up = true;
        
        player->xp_for_next_level = player->xp_for_next_level * 10; 

        printf("LEVEL UP! Você alcançou o nível %d!\n", player->level);
    }
}

bool buff_levels(Player* player){
    if(!player->is_level_up) return false;

    printf("Aplicando buffs para o Nível %d!\n", player->level);


    switch (player->level)
    {
    case 2:
        player->entity.base_max_hp += 10;
        player->base_attack += 5;
        player->base_defense += 3;
        player->iniciative += 2;
        break;
    case 3:
        player->entity.base_max_hp += 15;
        player->base_attack += 5;
        player->base_defense += 3;
        player->iniciative += 3;
        break;
    default:
        player->entity.base_max_hp += 5;
        player->base_attack += 2;
        player->base_defense += 1;
        break;
    }

    player->is_level_up = false;

    player_recalculate_stats(player);
    player->entity.hp = player->entity.max_hp; 

    return true;
}

void player_recalculate_stats(Player* player) {
    player->attack = player->base_attack;
    player->defense = player->base_defense;
    player->entity.max_hp = player->entity.base_max_hp;

    for (int i = 0; i < MAX_EQUIP_SLOTS; i++) {
        if (player->equipment[i] != NULL) {
            player->attack += player->equipment[i]->attack_buff;
            player->defense += player->equipment[i]->defense_buff;
            player->iniciative += player->equipment[i]->iniciative_buff;
            player->entity.max_hp += player->equipment[i]->max_hp_buff;
        }
    }    

    if (player->is_defending) {
        player->defense *= 2;
    }

    if (player->entity.hp > player->entity.max_hp) {
        player->entity.hp = player->entity.max_hp;
    }
}

void player_equip_item(Player* player, Item* item_to_equip) {
    if (item_to_equip->type != ITEM_EQUIPMENT) {
        printf("Erro: %s nao é um item equipavel!\n", item_to_equip->name);
        return;
    }
    
    for (int i = 0; i < MAX_EQUIP_SLOTS; i++) {
        if (player->equipment[i] == NULL) {
            player->equipment[i] = item_to_equip;
            printf("Equipou: %s\n", item_to_equip->name);
            
            
            player_recalculate_stats(player);
            return;
        }
    }

    printf("Slots de equipamento cheios! Nao foi possivel equipar %s.\n", item_to_equip->name);
}

void update_player_battle(Player* player, unsigned char* key, float dt){
    update_hit_box(&player->entity);
    player->moving = false;

    if(player->entity.anim_state == ANIM_HIT){
        Sprite* hit = player->entity.sprite[ANIM_HIT];
        update_sprite(hit, dt);

        if(hit->current_frame == 0 && hit->elapsed < dt){
            player->entity.anim_state = ANIM_IDLE;
            hit->current_frame = 0;
            hit->elapsed = 0;
        }
        return;
    }

    if(player->entity.anim_state == ANIM_ATTACK){
        
        Sprite* attack = player->entity.sprite[ANIM_ATTACK];
        update_sprite(attack, dt);
        if(attack->current_frame == 0 && attack->elapsed < dt){
            player->entity.anim_state = ANIM_IDLE;
            attack->current_frame = 0;
            attack->elapsed = 0;
        }
        return;
    }
    
    Sprite* current = player->entity.sprite[player->entity.anim_state];
    update_sprite(current, dt);
}

void update_player(Player* player, unsigned char* key, float dt){
    update_hit_box(&player->entity);
    player->moving = false;

    if(player->entity.anim_state == ANIM_HIT){
        Sprite* hit = player->entity.sprite[ANIM_HIT];
        update_sprite(hit, dt);

        if(hit->current_frame == 0 && hit->elapsed < dt){
            player->entity.anim_state = ANIM_IDLE;
            hit->current_frame = 0;
            hit->elapsed = 0;
        }
        return;
    }

    if(player->entity.anim_state == ANIM_ATTACK){
        
        Sprite* attack = player->entity.sprite[ANIM_ATTACK];
        update_sprite(attack, dt);
        if(attack->current_frame == 0 && attack->elapsed < dt){
            player->entity.anim_state = ANIM_IDLE;
            attack->current_frame = 0;
            attack->elapsed = 0;
        }
        return;
    }

    if (key[ALLEGRO_KEY_A] || key[ALLEGRO_KEY_LEFT]) { 
        player->entity.vx = 5;
        player->entity.x -= player->entity.vx; 
        player->entity.flip = ALLEGRO_FLIP_HORIZONTAL;
        player->moving = true; 
    } else if (key[ALLEGRO_KEY_D] ||  key[ALLEGRO_KEY_RIGHT]) { 
        player->entity.vx = 5;
        player->entity.x += player->entity.vx; 
        player->entity.flip = 0;
        player->moving = true; 
    }
    
    if (key[ALLEGRO_KEY_W] || key[ALLEGRO_KEY_UP]) { 
        player->entity.vy = 5;
        player->entity.y -= player->entity.vy; 
        player->moving = true; 
    } else if (key[ALLEGRO_KEY_S] ||  key[ALLEGRO_KEY_DOWN]) { 
        player->entity.vy = 5;
        player->entity.y += player->entity.vy; 
        player->moving = true; 
    }
    
    if(player->moving){
        player->entity.anim_state = ANIM_RUN;
        player->entity.x += player->entity.vx * dt;
        player->entity.y += player->entity.vy * dt;
    } else {
        player->entity.anim_state = ANIM_IDLE;
    }
    
    Sprite* current = player->entity.sprite[player->entity.anim_state];
    update_sprite(current, dt);
}

void select_item_battle(Player* player, unsigned char* key){
    if(key[ALLEGRO_KEY_H]){
        player->turn_choice = TURN_BIG_POTION;
        return;
    }
    
    if(key[ALLEGRO_KEY_J]){
        player->turn_choice = TURN_SMALL_POTION;
        return;
    }
    
    if(key[ALLEGRO_KEY_K]){
        player->turn_choice = TURN_WATER;
        return;
    }
}

void select_item(Player* player, unsigned char* key){
    if(key[ALLEGRO_KEY_H]){
        for(int i = 0; i < MAX_ITENS; i++){
            Item* current_item = player->inventory.slots[i].item;

            
            if(current_item != NULL && current_item->type == ITEM_HEAL){
                if(player->inventory.slots[i].quantity <= 0) return;

                player->entity.hp += current_item->value;
                player->inventory.slots[i].quantity--;
                printf("Quantidade: %d\n", player->inventory.slots[i].quantity);
                return;
            }
        }
    } 
    
    if(key[ALLEGRO_KEY_J]){
        for(int i = 0; i < MAX_ITENS; i++){
            Item* current_item = player->inventory.slots[i].item;

            
            if(current_item->type == ITEM_SMALL_HEAL){
                if(player->inventory.slots[i].quantity <= 0) return;

                player->entity.hp += current_item->value;
                player->inventory.slots[i].quantity--;
                printf("Quantidade: %d\n", player->inventory.slots[i].quantity);
                return;
            }
        }
    }
    
    if(key[ALLEGRO_KEY_K]){
        for(int i = 0; i < MAX_ITENS; i++){
            Item* current_item = player->inventory.slots[i].item;

            
            if(current_item->type == ITEM_WATER){
                if(player->inventory.slots[i].quantity <= 0) return;

                player->entity.hp += current_item->value;
                player->inventory.slots[i].quantity--;
                printf("Quantidade: %d\n", player->inventory.slots[i].quantity);
                return;
            }
        }
    }
}

void destroy_sprite_from_player(Player* player){
    for(int i = 0; i < QUANT_ANIMATIONS; i++){
        destroy_sprite(player->entity.sprite[i]);
    }
}

void destroy_player(Player* player){
    free(player);
}