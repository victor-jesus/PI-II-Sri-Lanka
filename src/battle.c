#include "battle.h"
#include "dice.h"
#include "screen.h"
#include <stdlib.h>
#include <stdio.h>
#include <allegro5/allegro5.h>
#include "allegro5/allegro5.h"
#include "allegro5/allegro_font.h"

void start_battle(Battle* battle, Player* player, Enemy* enemy){
    battle->player = player;
    battle->enemy = enemy;

    battle->state = BATTLE_START;

    int dice = D_20;

    int dice_player = roll(dice);
    int player_iniciative = player->iniciative + dice_player;
    printf("\nPlayer iniciative: %d\n", player_iniciative);

    int dice_enemy = roll(dice);
    int enemy_iniciative = enemy->iniciative + dice_enemy;
    printf("\nEnemy iniciative: %d\n", enemy_iniciative);

    if(player_iniciative >= enemy_iniciative){
        battle->turn_state = TURN_PLAYER;
        battle->turn_choice = TURN_NONE;
    } else {
        battle->turn_state = TURN_ENEMY;
        battle->turn_choice = TURN_NONE;
    }

    battle->player->turn_choice = TURN_NONE;
    battle->enemy->turn_choice = TURN_NONE;
}

Turn_choice enemy_choice(Battle* battle){
    return TURN_ATTACK;
}

void manage_battle(Battle* battle, ALLEGRO_EVENT event, ALLEGRO_TIMER* timer_enemy, ALLEGRO_FONT* font){

    if(battle->enemy->entity.hp <= 0){
        battle->turn_state = TURN_EMPTY;
        al_start_timer(battle->timer_end);

        battle->enemy->entity.anim_state = ANIM_DEATH;        
        battle->state = BATTLE_WIN;
        if(event.timer.source == battle->timer_end){
            battle->enemy->entity.isActive = false;
            al_stop_timer(battle->timer_end);
            al_set_timer_count(battle->timer_end, 0);
        }
        return;
    }

    if(battle->turn_state == TURN_ENEMY){
        al_start_timer(timer_enemy);

        Turn_choice choice = enemy_choice(battle);
        if(event.timer.source == timer_enemy){
            if(choice == TURN_ATTACK){
                battle->enemy->entity.anim_state = ANIM_ATTACK;
                take_damage(&battle->player->entity, 10);
                battle->enemy->turn_choice = TURN_NONE;
                battle->turn_state = TURN_PLAYER;
                al_stop_timer(timer_enemy);
                al_set_timer_count(timer_enemy, 0);
            }
            return;
        }

    }

    if(battle->turn_state == TURN_PLAYER){
        if(battle->player->turn_choice == TURN_ATTACK){
            battle->player->entity.anim_state = ANIM_ATTACK;
            take_damage(&battle->enemy->entity, 100);
            battle->turn_state = TURN_ENEMY;
            battle->player->turn_choice = TURN_NONE;
        }
        return;
    }

    
}

void destroy_battle(Battle* battle){
    free(battle);
}