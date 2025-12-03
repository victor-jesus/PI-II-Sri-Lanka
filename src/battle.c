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

    battle->can_act = true;
    battle->can_use_item = true;

    battle->player->entity.anim_state = ANIM_IDLE;
    battle->enemy->entity.anim_state = ANIM_IDLE;

    battle->enemy->entity.y = battle->player->entity.y + (battle->player->entity.box.h / 2);
    battle->opt_player = 1;

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

    battle->is_selecting_item = false;

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

    battle->player->original_defense = battle->player->defense;
}

void end_battle(Battle* battle, Player* player, Enemy* enemy){
    battle->player = player;
    battle->enemy = enemy;

    battle->enemy->is_battle = false;

    battle->dropped_itens = false;

    battle->can_act = false;
    battle->can_use_item = false;

    battle->player->entity.anim_state = ANIM_IDLE;
    battle->enemy->entity.anim_state = ANIM_IDLE;

    battle->enemy->entity.y = battle->player->entity.y + (battle->player->entity.box.h / 2);
    battle->opt_player = 1;

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

    battle->is_selecting_item = false;

    battle->state = BATTLE_END;

}

Turn_choice enemy_choice(Battle* battle) {
    Enemy* enemy = battle->enemy;
    Player* player = battle->player;
    
    int enemy_hp_percent = (int)(((float)enemy->entity.hp / (float)enemy->entity.max_hp) * 100);
    int player_hp_percent = (int)(((float)player->entity.hp / (float)player->entity.max_hp) * 100);

    int chance = roll(D_20); 

    switch (enemy->enemy_type) {
        
        case MOB: 
            // Se estiver com pouca vida, tem 50% de chance de defender
            if (enemy_hp_percent < 40 && chance > 10) { 
                return TURN_DEFEND;
            }
            
            return TURN_ATTACK;

        case MINOTAUR: 
            if (enemy_hp_percent < 25) {
                return TURN_ATTACK;
            }
            
            if (chance > 14) { 
                return TURN_DEFEND;
            }
            return TURN_ATTACK;
            
        case MEDUSA:
            
            if (player_hp_percent > 75 && chance > 10) {
                return TURN_DEFEND;
            }
            
            if (enemy_hp_percent < 30 && chance > 10) {
                return TURN_DEFEND;
            }
            return TURN_ATTACK;

        case ARAUTO: 
            
            if (player_hp_percent < 25 && chance > 4) { 
                return TURN_ATTACK; 
            }
            
            if (enemy_hp_percent < 40 && chance > 10) {
                return TURN_DEFEND;
            }
            
            if (chance > 15) {
                return TURN_DEFEND;
            }
            return TURN_ATTACK;

        case BOSSES: 
        default:
            
            if (chance > 14) { 
                return TURN_DEFEND;
            }
            return TURN_ATTACK;
    }
}


void attack_state(Battle* battle, ALLEGRO_EVENT event, ALLEGRO_FONT* font, Player* player, Enemy* enemy, Turn_state turn_state){
    int d20;
    
    switch (turn_state) {
        case TURN_PLAYER:
            d20 = roll(D_20);
            int attack_total = d20 + player->attack;

            if (d20 == 1) {
                sprintf(battle->log_ln1, "Rolagem dado: %d", d20);
                sprintf(battle->log_ln2, "ERRO CRÍTICO!");
            } else if (d20 == 20) {
                int damage = (roll(D_8) + player->attack) * 2; 

                sprintf(battle->log_ln1, "Dano: %d", damage);
                take_damage(&enemy->entity, damage);
            } else {
                int attack_total = d20 + player->attack;
                
                if (attack_total >= enemy->defense) {
                    int damage = roll(D_8) + player->attack; 

                    sprintf(battle->log_ln1, "Rolagem dado: %d", d20);
                    sprintf(battle->log_ln2, "%s acertou!", battle->player->name);
                    sprintf(battle->log_ln3, "%s causou %d de dano!", battle->player->name, damage);
                    take_damage(&enemy->entity, damage);
                } else {
                    sprintf(battle->log_ln2, " Errou!", d20, player->attack, attack_total);
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
        if (battle->player->is_defending) {
            battle->player->defense = battle->enemy->original_defense;
            battle->player->is_defending = false;
        }

        if(choice == TURN_ATTACK && battle->can_act == true){
            attack_state(battle, event, battle->battle_font, battle->player, battle->enemy, battle->turn_state);
            battle->can_act = false;
            battle->turn_state = TURN_ENEMY;
        } else if(choice == TURN_DEFEND){
            sprintf(battle->log_ln1, "%s se prepara!", battle->player->name);
            sprintf(battle->log_ln2, "Sua defesa dobrou por 1 turno!");
            
            battle->player->defense *= 2;
            battle->player->is_defending = true;
            battle->player->entity.anim_state = ANIM_IDLE;

            battle->turn_state = TURN_ENEMY;
        } else {
            if(battle->can_use_item == false) return;
        
            if (choice == TURN_BIG_POTION){
                for(int i = 0; i < MAX_ITENS; i++){
                    Item* current_item = battle->player->inventory.slots[i].item;

                    if(current_item != NULL && current_item->type == ITEM_HEAL){
                        if(battle->player->inventory.slots[i].quantity <= 0) return;

                        battle->player->entity.hp += current_item->value;
                        battle->player->inventory.slots[i].quantity--;
                        printf("Quantidade: %d\n", battle->player->inventory.slots[i].quantity);
                        sprintf(battle->log_ln1, "Você usou a %s", current_item->name);

                        battle->can_use_item = false;

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
                        sprintf(battle->log_ln1, "Você usou a %s", current_item->name);

                        battle->can_use_item = false;

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
                        sprintf(battle->log_ln1, "Você usou a %s", current_item->name);

                        battle->can_use_item = false;

                        return;
                    }
                }
            }
        }

        battle->player->turn_choice = TURN_NONE; 
        break;
    case TURN_ENEMY:
      
    default:
        break;
    }
}

void mob_death(Battle* battle, int* world_enemies, Game_state game_state){
    int rand_chance = rand() % 10 + 1;

    int qtd_xp = 50;
    add_xp(battle->player, qtd_xp);
    sprintf(battle->log_ln10, "Você ganhou %d XP", qtd_xp);
    
    if(*world_enemies <= 1 && battle && game_state == GAME_FIRST_MISSION){
        player_equip_item(battle->player, KEY_TO_MINOTAUR);
        printf("Chave\n");
        
        sprintf(battle->log_ln1, "O %s dropou um %s", battle->enemy->name, KEY_TO_MINOTAUR->name);
    } else if(*world_enemies <= 1 && battle && game_state == GAME_SECOND_MISSION){
        player_equip_item(battle->player, KEY_TO_MEDUSA);
        printf("Chave\n");
        
        sprintf(battle->log_ln1, "O %s dropou um %s", battle->enemy->name, KEY_TO_MINOTAUR->name, KEY_TO_MEDUSA->name);
    } else if(*world_enemies <= 1 && battle && game_state == GAME_THIRD_MISSION){
        player_equip_item(battle->player, KEY_TO_ARAUTO);
        printf("Chave\n");
        
        sprintf(battle->log_ln1, "O %s dropou um %s", battle->enemy->name, KEY_TO_MINOTAUR->name, KEY_TO_MINOTAUR->name);
    }

    rand_chance = rand() % 10 + 1;
    printf("rand_chance: %d\n", rand_chance);
    if(rand_chance >= 4){
        int qtd = rand() % 4 + 1;
        inventory_add_item(&battle->player->inventory, SMALL_POTION, qtd);

        sprintf(battle->log_ln2, "%s dropou %s.", battle->enemy->name, SMALL_POTION->name);
        sprintf(battle->log_ln3, "Quantidade: %d", qtd);

        player_equip_item(battle->player, AMULET_OF_STRENGTH);
        
        sprintf(battle->log_ln4, "%s dropou %s.", battle->enemy->name, AMULET_OF_STRENGTH->name);

        sprintf(battle->log_ln5, "Buffs/Debuffs:");

        sprintf(battle->log_ln6, " Atk: %d Def: %d Ini: %d Max hp: %d", AMULET_OF_STRENGTH->attack_buff, AMULET_OF_STRENGTH->defense_buff, AMULET_OF_STRENGTH->iniciative_buff, AMULET_OF_STRENGTH->max_hp_buff);

        
    } else {
        sprintf(battle->log_ln1, "%s não dropou itens.", battle->enemy->name);
    }
}

void minotaur_death(Battle* battle){
        int rand_chance = rand() % 10 + 1;
        
        int qtd_xp = 1000;
        add_xp(battle->player, qtd_xp);
        sprintf(battle->log_ln1, "Você ganhou %d XP", qtd_xp);

        player_equip_item(battle->player, KEY_TO_SECOND_MAP);
        sprintf(battle->log_ln2, "%s dropou %s", battle->enemy->name, KEY_TO_SECOND_MAP->name);
        
        int qtd = rand() % 4 + 1;
        inventory_add_item(&battle->player->inventory, BIG_POTION, qtd);

        sprintf(battle->log_ln3, "%s dropou %s", battle->enemy->name, BIG_POTION->name);
        sprintf(battle->log_ln4, "Quantidade: %d", qtd);

        player_equip_item(battle->player, AMULET_OF_MINOTAUR);
        sprintf(battle->log_ln6, "%s dropou %s", battle->enemy->name, AMULET_OF_MINOTAUR->name);

        sprintf(battle->log_ln7, "Buffs/Debuffs:");

        sprintf(battle->log_ln8, " Atk: %d Def: %d Ini: %d Max hp: %d", AMULET_OF_MINOTAUR->attack_buff, AMULET_OF_MINOTAUR->defense_buff, AMULET_OF_MINOTAUR->iniciative_buff, AMULET_OF_MINOTAUR->max_hp_buff);
}

void medusa_death(Battle* battle){
    int rand_chance = rand() % 10 + 1;
        
    int qtd_xp = 10000;
    add_xp(battle->player, qtd_xp);
    sprintf(battle->log_ln1, "Você ganhou %d XP", qtd_xp);

    player_equip_item(battle->player, KEY_TO_THIRD_MAP);
    
    sprintf(battle->log_ln2, "%s dropou um %s", battle->enemy->name, KEY_TO_THIRD_MAP->name);

    int qtd = rand() % 6 + 1;
    inventory_add_item(&battle->player->inventory, BIG_POTION, qtd);

    sprintf(battle->log_ln4, "%s dropou um item.", battle->enemy->name);
    sprintf(battle->log_ln5, "Quantidade: %d", qtd);

    player_equip_item(battle->player, AMULET_OF_MEDUSA);
    sprintf(battle->log_ln6, "%s dropou um %s", battle->enemy->name, AMULET_OF_MEDUSA->name);

    sprintf(battle->log_ln7, "Buffs/Debuffs:");

    sprintf(battle->log_ln8, " Atk: %d Def: %d Ini: %d Max hp: %d", AMULET_OF_MEDUSA->attack_buff, AMULET_OF_MEDUSA->defense_buff, AMULET_OF_MEDUSA->iniciative_buff, AMULET_OF_MEDUSA->max_hp_buff);
}

void drop_itens(Battle* battle, int* world_enemies, Game_state game_state){
    if(battle->dropped_itens) return;
    battle->dropped_itens = true;
        
    switch (battle->enemy->enemy_type){
        case MOB:
            mob_death(battle, world_enemies, game_state);
            *world_enemies -= 1;
        break;
        case MINOTAUR:
            minotaur_death(battle);
            break;

        case MEDUSA:
            medusa_death(battle);
            break;

        default:
            break;
    }    
}

void enemy_action(Battle* battle, ALLEGRO_EVENT event){
        al_start_timer(battle->timer_enemy);

        if(event.timer.source == battle->timer_enemy) {
            al_stop_timer(battle->timer_enemy); 
            al_set_timer_count(battle->timer_enemy, 0);

            if (battle->enemy->is_defending) {
                battle->enemy->defense = battle->enemy->original_defense;
                battle->enemy->is_defending = false;
            }

            Turn_choice choice = enemy_choice(battle);
            
            switch (choice)
            {
            case TURN_ATTACK:
                int d20 = roll(D_20);

                if (d20 == 1) {
                    sprintf(battle->log_ln1, "Rolagem dado: %ds", d20, battle->enemy->name, battle->enemy->attack);
                    
                    sprintf(battle->log_ln2, "ERRO CRÍTICO!");
                } else if (d20 == 20) {
                    int damage;
                    if(battle->enemy->enemy_type == MINOTAUR){
                        damage = (roll(D_10) + battle->enemy->attack) * 2;
                    } else if(battle->enemy->enemy_type == MEDUSA){
                        damage = (roll(D_6) + roll(D_6) + battle->enemy->attack) * 2;
                    } else if(battle->enemy->enemy_type == ARAUTO){
                        damage = (roll(D_8) + roll(D_8) + battle->enemy->attack) * 2;
                    } else if(battle->enemy->enemy_type == MOB){
                        damage = (roll(D_6) + battle->enemy->attack) * 2;
                    }
                    sprintf(battle->log_ln1, "Rolagem dado: %d", d20, battle->enemy->name, battle->enemy->attack);

                    sprintf(battle->log_ln2, "ACERTO CRÍTICO!", d20);
                    sprintf(battle->log_ln3, "Inimigo causou %d de dano!", damage);
                    take_damage(&battle->player->entity, damage);
                } else { 
                    int damage;
                    if(battle->enemy->enemy_type == MINOTAUR){
                        damage = (roll(D_10) + battle->enemy->attack);
                    } else if(battle->enemy->enemy_type == MEDUSA){
                        damage = (roll(D_6) + roll(D_6) + battle->enemy->attack);
                    } else if(battle->enemy->enemy_type == ARAUTO){
                        damage = (roll(D_8) + roll(D_8) + battle->enemy->attack);
                    } else if(battle->enemy->enemy_type == MOB){
                        damage = (roll(D_4) + battle->enemy->attack);
                    }

                    int attack_total = d20 + battle->enemy->attack;

                    if (attack_total >= battle->player->defense) { 
                        sprintf(battle->log_ln1, "Rolagem dado: %d", d20, battle->enemy->name, battle->enemy->attack);
                        sprintf(battle->log_ln2, "%s acertou", battle->enemy->name);
                        sprintf(battle->log_ln3, "Inimigo causou %d de dano.", damage);
                        take_damage(&battle->player->entity, damage);
                    } else { 
                        sprintf(battle->log_ln1, "Rolagem dado: %d", d20, battle->enemy->name, battle->enemy->attack);
                        sprintf(battle->log_ln2, "%s errou", battle->enemy->name);
                        sprintf(battle->log_ln3, "Sua defesa: %d", battle->player->defense);
                    }
                }
                battle->enemy->entity.anim_state = ANIM_ATTACK;
                break;
            case TURN_DEFEND:
                sprintf(battle->log_ln1, "O inimigo %s se prepara!", battle->enemy->name);
                sprintf(battle->log_ln2, "A defesa de %s dobrou por 1 turno!", battle->enemy->name);
                
                battle->enemy->defense *= 2;
                battle->enemy->is_defending = true;
                battle->enemy->entity.anim_state = ANIM_IDLE;
            default:
                break;
            }

            battle->enemy->turn_choice = TURN_NONE;
            battle->turn_state = TURN_PLAYER;

            battle->can_act = true;
            battle->can_use_item = true;

            if (battle->player->is_defending) {
                battle->player->defense = battle->player->original_defense;
                battle->player->is_defending = false;
            }
        }
}

bool talk_minus_than_75 = false;
bool talk_minus_than_50 = false;
bool talk_minus_than_25 = false;
void manage_battle(Battle* battle, ALLEGRO_EVENT event, ALLEGRO_TIMER* timer_death, Game_state game_state, unsigned char* key, ALLEGRO_FONT* font, int* world_enemies){
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
    } else if (event.timer.source == battle->error_timer){
        al_stop_timer(battle->error_timer);
        al_set_timer_count(battle->error_timer, 0);   
    }

    if(battle->player->entity.hp <= 0){
        
        battle->turn_state = TURN_EMPTY;
       

        battle->player->entity.anim_state = ANIM_DEATH;    
        battle->state = BATTLE_LOST;
        if(key[ALLEGRO_KEY_ENTER]){
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

    if(battle->enemy->entity.hp <= 0){
        
        drop_itens(battle, world_enemies, game_state);

        battle->turn_state = TURN_EMPTY;
       

        battle->enemy->entity.anim_state = ANIM_DEATH;    
        battle->state = BATTLE_WIN;
        if(key[ALLEGRO_KEY_ENTER]){
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
        enemy_action(battle, event);
        return;
    }

    if(battle->turn_state == TURN_PLAYER){
        deal_choice(battle, event, battle->player->turn_choice);
        return;
    }
}


void destroy_battle(Battle* battle){
    free(battle);

void destroy_battle(Battle* battle) {
    if (!battle) return;

    if (battle->timer_dialogue) {
        al_destroy_timer(battle->timer_dialogue);
        battle->timer_dialogue = NULL;
    }

    if (battle->timer_enemy) {
        al_destroy_timer(battle->timer_enemy);
        battle->timer_enemy = NULL;
    }

    if (battle->timer_end) {
        al_destroy_timer(battle->timer_end);
        battle->timer_end = NULL;
    }

    if (battle->log_timer) {
        al_destroy_timer(battle->log_timer);
        battle->log_timer = NULL;
    }

    if (battle->error_timer) {
        al_destroy_timer(battle->error_timer);
        battle->error_timer = NULL;
    }

    if (battle->dialogue_sprite) {
        al_destroy_bitmap(battle->dialogue_sprite);
        battle->dialogue_sprite = NULL;
    }

    free(battle);
}
}