#include "battle.h"
#include "dice.h"
#include "screen.h"
#include <stdlib.h>
#include <stdio.h>
#include <allegro5/allegro5.h>

Battle* start_battle(Player* player, Enemy* enemy){
    Battle* battle = malloc(sizeof(Battle));
    battle->player = player;
    battle->enemy = enemy;

    battle->player->entity.x = 300;
    battle->enemy->entity.x = SCREEN_W - 300;

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
    } else {
        battle->turn_state = TURN_ENEMY;
    }

    return battle;
}

void manage_battle(Battle* battle, ALLEGRO_EVENT event, ALLEGRO_TIMER* timer_enemy){
    if(battle->turn_state == TURN_ENEMY){
        al_start_timer(timer_enemy);

        if(event.timer.source == timer_enemy){
            take_damage(&battle->player->entity, 10);
            battle->turn_choice = NONE;
            battle->turn_state = TURN_PLAYER;
            al_stop_timer(timer_enemy);
            al_set_timer_count(timer_enemy, 0);
        }
    }

    if(battle->turn_state == TURN_PLAYER){
        if(battle->turn_choice == ATTACK){
            take_damage(&battle->enemy->entity, 10);
            battle->turn_choice = NONE;
            battle->turn_state = TURN_ENEMY;
        }
    }

    if(battle->enemy->entity.hp == 0){
        battle->state = BATTLE_END;
        battle->enemy->entity.isActive = false;
    }
}

void destroy_battle(Battle* battle){
    free(battle);
}