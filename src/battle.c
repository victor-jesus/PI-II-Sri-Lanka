#include "battle.h"
#include "dice.h"
#include "screen.h"
#include <stdlib.h>
#include <stdio.h>
#include <allegro5/allegro5.h>
#include "allegro5/allegro5.h"
#include "allegro5/allegro_font.h"

Turn_state turn_state;

void start_battle(Battle* battle, Player* player, Enemy* enemy){
    battle->player = player;
    battle->enemy = enemy;

    battle->log_ln1[0] = '\0';
    battle->log_ln2[0] = '\0';

    battle->state = BATTLE_DIALOGUE;

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

typedef enum {
    HIT,
    MISS
} Attack_hit;

void attack_state(Battle* battle, ALLEGRO_EVENT event, ALLEGRO_FONT* font, Player* player, Enemy* enemy, Turn_state turn_state){
    int d20;
    
    switch (turn_state) {
    case TURN_PLAYER:
        d20 = roll(D_20);
        int attack_total = d20 + player->attack;
        
        if(attack_total >= enemy->defense){
            sprintf(battle->log_ln1, "Ataque: %d+%d=%d. Acertou!", d20, player->attack, attack_total);
            sprintf(battle->log_ln2, "Defesa inimigo: %d", enemy->defense);

            battle->player->entity.anim_state = ANIM_ATTACK;
            take_damage(&battle->enemy->entity, attack_total);
            battle->turn_state = TURN_ENEMY;
            battle->player->turn_choice = TURN_NONE;
        } else {
            sprintf(battle->log_ln1, "Ataque: %d+%d=%d. Errou!", d20, player->attack, attack_total);        
            sprintf(battle->log_ln2, "Defesa inimigo: %d", enemy->defense);
            battle->player->entity.anim_state = ANIM_ATTACK;
            battle->turn_state = TURN_ENEMY;
            battle->player->turn_choice = TURN_NONE;
        }

        break;
    case TURN_ENEMY:
        al_start_timer(battle->timer_enemy);

        Turn_choice choice = enemy_choice(battle);
        if(event.timer.source == battle->timer_enemy){
            if(choice == TURN_ATTACK){
                d20 = roll(D_20);
                int attack_total = d20 + enemy->attack;

                if(attack_total >= player->defense){
                    sprintf(battle->log_ln1, "Ataque: %d+%d=%d. Inimigo acertou!", d20, enemy->attack, attack_total);
                    sprintf(battle->log_ln2, "Defesa player: %d", player->defense);

                    battle->enemy->entity.anim_state = ANIM_ATTACK;
                    take_damage(&battle->player->entity, attack_total);
                    battle->enemy->turn_choice = TURN_NONE;
                    battle->turn_state = TURN_PLAYER;
                    al_stop_timer(battle->timer_enemy);
                    al_set_timer_count(battle->timer_enemy, 0);
                }
               
            }
            return;
        }
    default:
        break;
    }
}

void deal_choice(Battle* battle, ALLEGRO_EVENT event, Turn_choice choice){

    switch (battle->turn_state)
    {
    case TURN_PLAYER:
        if(choice == TURN_ATTACK){
            attack_state(battle, event, battle->battle_font, battle->player, battle->enemy, battle->turn_state);
        }
        break;
    case TURN_ENEMY:
        attack_state(battle, event, battle->battle_font, battle->player, battle->enemy, battle->turn_state);
    default:
        break;
    }


}

bool talk_minus_than_75 = false;
bool talk_minus_than_50 = false;
bool talk_minus_than_25 = false;

void manage_battle(Battle* battle, ALLEGRO_EVENT event, ALLEGRO_FONT* font){
    
    if(battle->player->entity.hp <= 75 && talk_minus_than_75 == false){
        battle->state = BATTLE_DIALOGUE;
        battle->dialogues = DIALOGUE_BATTLE_6;
        talk_minus_than_75 = true;
    }
    if(battle->player->entity.hp <= 50 && talk_minus_than_50 == false){
        battle->dialogues = DIALOGUE_BATTLE_5;
        battle->state = BATTLE_DIALOGUE;
        talk_minus_than_50 = true;
    }
    if(battle->player->entity.hp <= 25 && talk_minus_than_25 == false){
        battle->dialogues = DIALOGUE_BATTLE_7;
        battle->state = BATTLE_DIALOGUE;
        talk_minus_than_25 = true;
    }
    if(battle->state == BATTLE_DIALOGUE) return;

    
    if(event.timer.source == battle->log_timer){
        al_stop_timer(battle->log_timer);
        al_set_timer_count(battle->log_timer, 0);

        battle->log_ln1[0] = '\0';
        battle->log_ln2[0] = '\0';

    }

    if(battle->enemy->entity.hp <= 0){
        battle->turn_state = TURN_EMPTY;
        al_start_timer(battle->timer_end);

        battle->enemy->entity.anim_state = ANIM_DEATH;        
        battle->state = BATTLE_WIN;
        if(event.timer.source == battle->timer_end){
            battle->enemy->entity.isActive = false;
            battle->state = BATTLE_NONE;
            al_stop_timer(battle->timer_end);
            al_set_timer_count(battle->timer_end, 0);
        }
        return;
    }

    if(battle->turn_state == TURN_ENEMY){
        deal_choice(battle, event, battle->turn_choice);
        return;
    }

    if(battle->turn_state == TURN_PLAYER){
        deal_choice(battle, event, battle->player->turn_choice);
        return;
    }
}

void destroy_battle(Battle* battle){
    free(battle);
}