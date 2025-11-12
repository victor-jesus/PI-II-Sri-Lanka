#include "battle.h"
#include "dice.h"
#include "screen.h"
#include <stdlib.h>
#include <stdio.h>
#include <allegro5/allegro5.h>
#include "allegro5/allegro5.h"
#include "allegro5/allegro_font.h"
#include "inventory.h"

#include <time.h>

Turn_state turn_state;

void start_battle(Battle* battle, Player* player, Enemy* enemy){
    
    battle->player = player;
    battle->enemy = enemy;

    battle->enemy->is_battle = true;

    battle->dropped_itens = false;

    battle->player->can_act = true;
    battle->player->can_use_item = true;

    battle->player->entity.anim_state = ANIM_IDLE;
    battle->enemy->entity.anim_state = ANIM_IDLE;

    battle->log_ln1[0] = '\0';
    battle->log_ln2[0] = '\0';
    battle->log_ln3[0] = '\0';
    battle->log_ln4[0] = '\0';
    battle->log_ln5[0] = '\0';
    battle->log_ln6[0] = '\0';
    battle->log_ln7[0] = '\0';
    battle->log_ln8[0] = '\0';
    battle->log_ln9[0] = '\0';
    battle->log_ln10[0] = '\0';

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

typedef enum {
    HIT,
    MISS
} Attack_hit;

void use_item(Battle* battle, ALLEGRO_EVENT event, ALLEGRO_FONT* font, Player* player, Enemy* enemy, Turn_state turn_state){

}

void attack_state(Battle* battle, ALLEGRO_EVENT event, ALLEGRO_FONT* font, Player* player, Enemy* enemy, Turn_state turn_state){
    int d20;
    
    switch (turn_state) {
        case TURN_PLAYER:
            d20 = roll(D_20);
            int attack_total = d20 + player->attack;

            if (d20 == 1) {
            sprintf(battle->log_ln1, "Rolagem Dado: %d.", d20);

            sprintf(battle->log_ln2, "Ataque: %d. ERRO CRÍTICO!", d20);
            sprintf(battle->log_ln3, "Você errou feio!");

        } else if (d20 == 20) {
            int damage = (roll(D_8) + player->attack) * 2; 

            sprintf(battle->log_ln1, "Rolagem Dado: %d. Ataque %s: %d", d20, battle->player->name, battle->player->attack);


            sprintf(battle->log_ln2, "Ataque: %d. ACERTO CRÍTICO!", d20);
            sprintf(battle->log_ln3, "Você causou %d de dano!", damage);
            take_damage(&enemy->entity, damage);

        } else {
            int attack_total = d20 + player->attack;
            
            if (attack_total >= enemy->defense) {
                int damage = roll(D_8) + player->attack; 

                sprintf(battle->log_ln1, "Rolagem Dado: %d. Ataque %s: %d", d20, battle->player->name, battle->player->attack);

                sprintf(battle->log_ln2, "Ataque: %d+%d=%d. Acertou!", d20, player->attack, attack_total);
                sprintf(battle->log_ln3, "Você causou %d de dano.", damage);
                take_damage(&enemy->entity, damage);
            } else {
                
                sprintf(battle->log_ln1, "Rolagem Dado: %d. Ataque %s: %d", d20, battle->player->name, battle->player->attack);

                sprintf(battle->log_ln2, "Ataque: %d+%d=%d. Errou!", d20, player->attack, attack_total);
                sprintf(battle->log_ln3, "Defesa do inimigo: %d", enemy->defense);
            }
        }

        battle->player->entity.anim_state = ANIM_ATTACK;
        // battle->turn_state = TURN_ENEMY;
        // battle->player->turn_choice = TURN_NONE;
        break;
    case TURN_ENEMY:
        
    default:
        break;
    }
}

void deal_choice(Battle* battle, ALLEGRO_EVENT event, Turn_choice choice){

    if (choice == TURN_NONE) return;

    // Depois separar cada elemnto em função, inclusive o uso de itens passando o tipo do item
    // REFATORAR ESSE CÓDIGO
    switch (battle->turn_state)
    {
    case TURN_PLAYER:
        if(choice == TURN_ATTACK && battle->player->can_act == true){
            attack_state(battle, event, battle->battle_font, battle->player, battle->enemy, battle->turn_state);
            battle->player->can_act = false;
            battle->turn_state = TURN_ENEMY;
        } else {
            if(battle->player->can_use_item == false) return;
        
            if (choice == TURN_BIG_POTION){
                for(int i = 0; i < MAX_ITENS; i++){
                    Item* current_item = battle->player->inventory.slots[i].item;

                    if(current_item != NULL && current_item->type == ITEM_HEAL){
                        if(battle->player->inventory.slots[i].quantity <= 0) return;

                        battle->player->entity.hp += current_item->value;
                        battle->player->inventory.slots[i].quantity--;
                        printf("Quantidade: %d\n", battle->player->inventory.slots[i].quantity);
                        sprintf(battle->log_ln5, "Você usou a %s", current_item->name);

                        battle->player->can_use_item = false;

                        return;
                    }
                }
            } else if (choice == TURN_SMALL_POTION){
            for(int i = 0; i < MAX_ITENS; i++){
                Item* current_item = battle->player->inventory.slots[i].item;

                if(current_item != NULL && current_item->type == ITEM_SMALL_HEAL){
                    if(battle->player->inventory.slots[i].quantity <= 0) return;

                    battle->player->entity.hp += current_item->value;
                    battle->player->inventory.slots[i].quantity--;
                    printf("Quantidade: %d\n", battle->player->inventory.slots[i].quantity);
                    sprintf(battle->log_ln5, "Você usou a %s", current_item->name);

                    battle->player->can_use_item = false;

                    return;
                }
            }
            } else if (choice == TURN_WATER){
                for(int i = 0; i < MAX_ITENS; i++){
                    Item* current_item = battle->player->inventory.slots[i].item;

                    if(current_item != NULL && current_item->type == ITEM_WATER){
                        if(battle->player->inventory.slots[i].quantity <= 0) return;

                        battle->player->entity.hp += current_item->value;
                        battle->player->inventory.slots[i].quantity--;
                        printf("Quantidade: %d\n", battle->player->inventory.slots[i].quantity);
                        sprintf(battle->log_ln5, "Você usou a %s", current_item->name);

                        battle->player->can_use_item = false;

                        return;
                    }
                }
            }
        }

        battle->player->turn_choice = TURN_NONE; 
        break;
    default:
        break;
    }
}


void drop_itens(Battle* battle, int* world_enemies, Game_state game_state){
    if(battle->dropped_itens) return;
    battle->dropped_itens = true;
    
    int rand_chance, qtd, qtd_xp;
    
    switch (battle->enemy->enemy_type){
        case MOB:
            qtd_xp = 50;
            add_xp(battle->player, qtd_xp);
            sprintf(battle->log_ln10, "Você ganhou %d", qtd_xp);
            
            if(*world_enemies <= 1 && battle && game_state == GAME_FIRST_MISSION){
                player_equip_item(battle->player, KEY_TO_MINOTAUR);
                printf("Chave\n");
                
                sprintf(battle->log_ln1, "O %s dropou um item.", battle->enemy->name);
                sprintf(battle->log_ln2, "Item: %s adicionado ao inventário.", KEY_TO_MINOTAUR->name);
                sprintf(battle->log_ln3, "%s", KEY_TO_MINOTAUR->description);
            }

            rand_chance = rand() % 10 + 1;
            printf("rand_chance: %d\n", rand_chance);
            if(rand_chance >= 6){
                qtd = rand() % 4 + 1;
                inventory_add_item(&battle->player->inventory, SMALL_POTION, qtd);

                sprintf(battle->log_ln5, "O %s dropou um item.", battle->enemy->name);
                sprintf(battle->log_ln6, "Item: %s adicionado ao inventário. Quantidade: %d", SMALL_POTION->name, qtd);

                player_equip_item(battle->player, AMULET_OF_STRENGTH);
                
                sprintf(battle->log_ln7, "Item: %s adicionado ao inventário.", AMULET_OF_STRENGTH->name);

                sprintf(battle->log_ln8, "Buffs/Debuffs:");

                sprintf(battle->log_ln9, " Atk: %d Def: %d Ini: %d Max hp: %d", AMULET_OF_STRENGTH->attack_buff, AMULET_OF_STRENGTH->defense_buff, AMULET_OF_STRENGTH->iniciative_buff, AMULET_OF_STRENGTH->max_hp_buff);

               
            } else {
                sprintf(battle->log_ln3, "O %s não dropou itens.", battle->enemy->name);
            }
        break;
    case MINOTAUR:
        rand_chance = rand() % 10 + 1;
        
        qtd_xp = 1000;
        add_xp(battle->player, qtd_xp);
        sprintf(battle->log_ln4, "Você ganhou %d", qtd_xp);

        player_equip_item(battle->player, KEY_TO_SECOND_MAP);
        printf("Chave\n");
        
        sprintf(battle->log_ln1, "O %s dropou um item.", battle->enemy->name);
        sprintf(battle->log_ln2, "Item: %s adicionado ao inventário.", KEY_TO_SECOND_MAP->name, qtd);
        sprintf(battle->log_ln3, "%s", KEY_TO_SECOND_MAP->description);
        
        qtd = rand() % 4 + 1;
        inventory_add_item(&battle->player->inventory, BIG_POTION, qtd);

        sprintf(battle->log_ln3, "O %s dropou um item.", battle->enemy->name);
        sprintf(battle->log_ln4, "Item: %s adicionado ao inventário. Quantidade: %d", BIG_POTION->name, qtd);

        player_equip_item(battle->player, AMULET_OF_MINOTAUR);
        sprintf(battle->log_ln5, "Item: %s adicionado ao inventário.", AMULET_OF_MINOTAUR->name);

        sprintf(battle->log_ln7, "Buffs/Debuffs:");

        sprintf(battle->log_ln8, " Atk: %d Def: %d Ini: %d Max hp: %d", AMULET_OF_MINOTAUR->attack_buff, AMULET_OF_MINOTAUR->defense_buff, AMULET_OF_MINOTAUR->iniciative_buff, AMULET_OF_MINOTAUR->max_hp_buff);
        
        break;

    case MEDUSA:
        rand_chance = rand() % 10 + 1;
        
        qtd_xp = 10000;
        add_xp(battle->player, qtd_xp);
        sprintf(battle->log_ln4, "Você ganhou %d", qtd_xp);

        player_equip_item(battle->player, KEY_TO_SECOND_MAP);
        printf("Chave\n");
        
        sprintf(battle->log_ln1, "O %s dropou um item.", battle->enemy->name);
        sprintf(battle->log_ln2, "Item: %s adicionado ao inventário.", KEY_TO_SECOND_MAP->name, qtd);
        sprintf(battle->log_ln3, "%s", KEY_TO_SECOND_MAP->description);
        
        qtd = rand() % 4 + 1;
        inventory_add_item(&battle->player->inventory, BIG_POTION, qtd);

        sprintf(battle->log_ln3, "O %s dropou um item.", battle->enemy->name);
        sprintf(battle->log_ln4, "Item: %s adicionado ao inventário. Quantidade: %d", BIG_POTION->name, qtd);

        player_equip_item(battle->player, AMULET_OF_MINOTAUR);
        sprintf(battle->log_ln5, "Item: %s adicionado ao inventário.", AMULET_OF_MINOTAUR->name);

        sprintf(battle->log_ln7, "Buffs/Debuffs:");

        sprintf(battle->log_ln8, " Atk: %d Def: %d Ini: %d Max hp: %d", AMULET_OF_MINOTAUR->attack_buff, AMULET_OF_MINOTAUR->defense_buff, AMULET_OF_MINOTAUR->iniciative_buff, AMULET_OF_MINOTAUR->max_hp_buff);
        
        break;

    default:
        break;
    }

    *world_enemies -= 1;
}

bool talk_minus_than_75 = false;
bool talk_minus_than_50 = false;
bool talk_minus_than_25 = false;

void manage_battle(Battle* battle, ALLEGRO_EVENT event, Game_state game_state, unsigned char* key, ALLEGRO_FONT* font, int* world_enemies){
    if(battle->player->entity.hp <= 75 && talk_minus_than_75 == false && battle->enemy->enemy_type == MINOTAUR){
        battle->state = BATTLE_DIALOGUE;
        battle->dialogues = DIALOGUE_BATTLE_6;
        talk_minus_than_75 = true;
    }
    if(battle->player->entity.hp <= 50 && talk_minus_than_50 == false && battle->enemy->enemy_type == MINOTAUR){
        battle->dialogues = DIALOGUE_BATTLE_5;
        battle->state = BATTLE_DIALOGUE;
        talk_minus_than_50 = true;
    }
    if(battle->player->entity.hp <= 25 && talk_minus_than_25 == false && battle->enemy->enemy_type == MINOTAUR){
        battle->dialogues = DIALOGUE_BATTLE_7;
        battle->state = BATTLE_DIALOGUE;
        talk_minus_than_25 = true;
    }
    if(battle->state == BATTLE_DIALOGUE) return;

    
    if(event.timer.source == battle->log_timer){
        al_stop_timer(battle->log_timer);
        al_set_timer_count(battle->log_timer, 0);   
    }

    if(battle->enemy->entity.hp <= 0){
        
        drop_itens(battle, world_enemies, game_state);
        //calculate_xp(battle);

        battle->turn_state = TURN_EMPTY;
       

        battle->enemy->entity.anim_state = ANIM_DEATH;    
        battle->state = BATTLE_WIN;
        if(key[ALLEGRO_KEY_E]){
            battle->enemy->entity.isActive = false;
            battle->state = BATTLE_NONE;
            battle->log_ln1[0] = '\0';
            battle->log_ln2[0] = '\0';
            battle->log_ln3[0] = '\0';
            battle->log_ln4[0] = '\0';
            battle->log_ln5[0] = '\0';
            battle->log_ln6[0] = '\0';
            battle->log_ln7[0] = '\0';
            battle->log_ln8[0] = '\0';
            battle->log_ln9[0] = '\0';
            battle->log_ln10[0] = '\0';
        }
        return;
    }


    if(battle->turn_state == TURN_ENEMY) {
        al_start_timer(battle->timer_enemy);
        if(event.timer.source == battle->timer_enemy) {
            al_stop_timer(battle->timer_enemy); 
            al_set_timer_count(battle->timer_enemy, 0);

            Turn_choice choice = enemy_choice(battle);
            
            if(choice == TURN_ATTACK) {
                int d20 = roll(D_20);
                if (d20 == 1) {
                    sprintf(battle->log_ln1, "Rolagem Dado: %d. Ataque %s: %d", d20, battle->enemy->name, battle->enemy->attack);
                    
                    sprintf(battle->log_ln2, "Ataque Inimigo: %d. ERRO CRÍTICO!", d20);
                    sprintf(battle->log_ln3, "O inimigo errou feio!");
                } else if (d20 == 20) {
                    int damage;
                    if(battle->enemy->enemy_type == MINOTAUR){
                        damage = (roll(D_10) + battle->enemy->attack) * 2;
                    } else if(battle->enemy->enemy_type == MOB){
                        damage = (roll(D_6) + battle->enemy->attack) * 2;
                    }
                    sprintf(battle->log_ln1, "Rolagem Dado: %d. Ataque %s: %d", d20, battle->enemy->name, battle->enemy->attack);

                    sprintf(battle->log_ln2, "Ataque Inimigo: %d. ACERTO CRÍTICO!", d20);
                    sprintf(battle->log_ln3, "Inimigo causou %d de dano!", damage);
                    take_damage(&battle->player->entity, damage);
                } else { 
                    int damage;
                    if(battle->enemy->enemy_type == MINOTAUR){
                        damage = (roll(D_10) + battle->enemy->attack);
                    } else if(battle->enemy->enemy_type == MOB){
                        damage = (roll(D_6) + battle->enemy->attack);
                    }

                    int attack_total = d20 + battle->enemy->attack;

                    if (attack_total >= battle->player->defense) { 
                        sprintf(battle->log_ln1, "Rolagem Dado: %d. Ataque %s: %d", d20, battle->enemy->name, battle->enemy->attack);

                        sprintf(battle->log_ln2, "Ataque Inimigo: %d+%d=%d. Acertou!", d20, battle->enemy->attack, attack_total);
                        sprintf(battle->log_ln3, "Inimigo causou %d de dano.", damage);
                        take_damage(&battle->player->entity, damage);
                    } else { 
                        sprintf(battle->log_ln1, "Rolagem Dado: %d. Ataque %s: %d", d20, battle->enemy->name, battle->enemy->attack);

                        sprintf(battle->log_ln2, "Ataque Inimigo: %d+%d=%d. Errou!", d20, battle->enemy->attack, attack_total);
                        sprintf(battle->log_ln3, "Sua defesa: %d", battle->player->defense);
                    }
                }
                battle->enemy->entity.anim_state = ANIM_ATTACK;
            }

            battle->enemy->turn_choice = TURN_NONE;
            battle->turn_state = TURN_PLAYER;

            battle->player->can_act = true;
            battle->player->can_use_item = true;
        }
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