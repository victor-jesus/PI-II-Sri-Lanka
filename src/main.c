#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/mouse.h>
#include "game.h"
#include "time.h"
#include "screen.h"
#include "game_state.h"
#include "inventory.h"

#define KEY_SEEN 1
#define KEY_DOWN 2

// FUNÇÕES AUXILIARES DE INICIALIZAÇÃO
void must_init(bool test, char* description){
    if(test) return;
    printf("Não foi possível inicializar %s\n", description);
    exit(1);
}

// 1. Input dos Diálogos de Batalha
void handle_battle_dialogue_input(Game* game, unsigned char* key) {
    if(!key[ALLEGRO_KEY_E]) return;

    // Lógica de SAÍDA (Comum a todos)
    if(game->battle->dialogues == DIALOGUE_BATTLE_FINAL_INTRO || 
       game->battle->dialogues == DIALOGUE_AFTER_TRASH_TALK ||
       game->battle->dialogues == DIALOGUE_MEDUSA_FINAL_3 ||
       game->battle->dialogues == DIALOGUE_ARAUTO_5 ||
       game->battle->dialogues == DIALOGUE_ARAUTO_FINAL_5) {
        game->battle->state = BATTLE_START;
    } 
    // Lógica de PULO(Minotauro)
    else if(game->battle->dialogues == DIALOGUE_BATTLE_5 || 
            game->battle->dialogues == DIALOGUE_BATTLE_6 || 
            game->battle->dialogues == DIALOGUE_BATTLE_7) {
        game->battle->dialogues = DIALOGUE_AFTER_TRASH_TALK;
    }
    // Lógica de PULO  (Medusa)
    else if(game->battle->dialogues == DIALOGUE_MEDUSA_5) {
        game->battle->dialogues = DIALOGUE_BATTLE_FINAL_INTRO;
    }
    else if(game->battle->dialogues == DIALOGUE_MEDUSA_RANDOM_1 ||
            game->battle->dialogues == DIALOGUE_MEDUSA_RANDOM_2 ||
            game->battle->dialogues == DIALOGUE_MEDUSA_RANDOM_3) {
        game->battle->dialogues = DIALOGUE_AFTER_TRASH_TALK;
    }
    // Lógica de PULO (Arauto)
    else if(game->battle->dialogues == DIALOGUE_ARAUTO_RANDOM_1 ||
            game->battle->dialogues == DIALOGUE_ARAUTO_RANDOM_2 ||
            game->battle->dialogues == DIALOGUE_ARAUTO_RANDOM_3) {
        game->battle->dialogues = DIALOGUE_AFTER_TRASH_TALK;
    }
    else {
        game->battle->dialogues++;
    }
}

// 2. Input do Submenu de Itens
void handle_battle_item_menu(Game* game, unsigned char* key) {
    int real_item_count = count_consumable_items(&game->player->inventory);
    int total_options = real_item_count + 1; // +1 para o botão VOLTAR

    // Navegação
    if(key[ALLEGRO_KEY_DOWN] || key[ALLEGRO_KEY_S]){
        if(game->battle->item_selection_index < total_options - 1){
            game->battle->item_selection_index++;
        } else {
            game->battle->item_selection_index = 0;
        }
    } 
    else if (key[ALLEGRO_KEY_UP] || key[ALLEGRO_KEY_W]){
        if(game->battle->item_selection_index > 0){
            game->battle->item_selection_index--;
        } else {
            game->battle->item_selection_index = total_options - 1;
        }
    }

    // Confirmação
    if(key[ALLEGRO_KEY_E] || key[ALLEGRO_KEY_ENTER] || key[ALLEGRO_KEY_SPACE]){
        // Opção VOLTAR selecionada
        if (game->battle->item_selection_index == real_item_count) {
            game->battle->is_selecting_item = false;
        } 
        // Item selecionado
        else if (total_options > 0) {
            int slot_index = get_inventory_slot_by_menu_index(&game->player->inventory, game->battle->item_selection_index);
            
            if (slot_index != -1) {
                Item* item_usado = game->player->inventory.slots[slot_index].item;

                // Aplica efeito
                game->player->entity.hp += item_usado->value;
                if(game->player->entity.hp > game->player->entity.max_hp) {
                    game->player->entity.hp = game->player->entity.max_hp;
                }

                // Remove do inventário
                game->player->inventory.slots[slot_index].quantity--;

                // Define animação/ação
                if (item_usado->type == ITEM_HEAL) game->player->turn_choice = TURN_BIG_POTION;
                else if (item_usado->type == ITEM_SMALL_HEAL) game->player->turn_choice = TURN_SMALL_POTION;
                else if (item_usado->type == ITEM_WATER) game->player->turn_choice = TURN_WATER;

                // Consome a permissão de usar item (Lógica adicionada na conversa anterior)
                game->battle->can_use_item = false;

                // Fecha menu
                game->battle->is_selecting_item = false;
            }
        }
    }

    // Voltar com ESC/Backspace
    if(key[ALLEGRO_KEY_ESCAPE] || key[ALLEGRO_KEY_BACKSPACE]){
        game->battle->is_selecting_item = false;
        key[ALLEGRO_KEY_ESCAPE] = 0; // Previne fechar o jogo
    }

    key[ALLEGRO_KEY_E] = 0;
    key[ALLEGRO_KEY_SPACE] = 0;
    key[ALLEGRO_KEY_ENTER] = 0;
}

// 3. Input do Menu Principal de Batalha (Atacar/Defender/Item)
void handle_battle_main_menu(Game* game, unsigned char* key) {
    if(key[ALLEGRO_KEY_E] || key[ALLEGRO_KEY_ENTER] || key[ALLEGRO_KEY_SPACE]){
        switch(game->battle->opt_player){
            case 1: // ATACAR
                game->player->turn_choice = TURN_ATTACK;
                break;
            case 2: // DEFENDER
                game->player->turn_choice = TURN_DEFEND;
                break;
            case 3: // USAR ITEM
            if (game->battle->can_use_item) {
                game->battle->is_selecting_item = true;
                game->battle->item_selection_index = 0;
            } else {
                if (!al_get_timer_started(game->battle->error_timer)) {
                    al_start_timer(game->battle->error_timer);
                    
                }
            }
            break;
        }
    }

    // Navegação
    if(key[ALLEGRO_KEY_DOWN]){
        if(game->battle->opt_player < 3){
            game->battle->opt_player++;
        } else {
            game->battle->opt_player = 1;
        }
    } else if (key[ALLEGRO_KEY_UP]){
        if(game->battle->opt_player > 1){
            game->battle->opt_player--;
        } else {
            game->battle->opt_player = 3;
        }
    }
}

void handle_input(Game* game, unsigned char* key) {
    if(game->gameplay_state == GAMEPLAY_EXPLORING) {
        select_item(game->player, key);
    } 

    if(game->state == GAME_TUTORIAL){
        if(key[ALLEGRO_KEY_ESCAPE] || key[ALLEGRO_KEY_B]){
            game->state = GAME_MENU;
            key[ALLEGRO_KEY_ESCAPE] = 0;
        }
    }

    // 3. Input de Inicialização
    if(game->state == GAME_INIT){
        if(key[ALLEGRO_KEY_E]){
            game->init_dialogues++;
        }
        key[ALLEGRO_KEY_E] = 0;
    }

    // 4. Lógica Principal de Batalha
    if (game->gameplay_state == GAMEPLAY_BATTLE) {
        
        // Se estiver em Diálogo
        if (game->battle->state == BATTLE_DIALOGUE) {
            handle_battle_dialogue_input(game, key);
        }
        // Se for turno do Jogador
        else if (game->battle->turn_state == TURN_PLAYER) {
            if (game->battle->is_selecting_item) {
                handle_battle_item_menu(game, key);
            } else {
                handle_battle_main_menu(game, key);
            }
        }
    }
}

void handle_pause(unsigned char* key, Game *game)
{
    if (key[ALLEGRO_KEY_ESCAPE] && game->gameplay_state == GAMEPLAY_EXPLORING)
    {
        game->previous_gameplay_state = game->gameplay_state;
        game->gameplay_state = GAMEPLAY_PAUSE;
        key[ALLEGRO_KEY_ESCAPE] = 0;
    }

    if (game->gameplay_state == GAMEPLAY_PAUSE)
    {
        if (key[ALLEGRO_KEY_ESCAPE])
        {
            game->gameplay_state = game->previous_gameplay_state;
            key[ALLEGRO_KEY_ESCAPE] = 0;
        }
    }
}

// --- MAIN ---
int main(){
    srand(time(NULL));

    // Inicializações Allegro
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");
    must_init(al_install_mouse(), "mouse");
    must_init(al_init_primitives_addon(), "primitives");
    must_init(al_init_font_addon(), "font");
    must_init(al_init_ttf_addon(), "ttf");
    must_init(al_init_image_addon(), "image");
    
    ALLEGRO_DISPLAY* display = al_create_display(SCREEN_W, SCREEN_H);
    must_init(display, "display");

    ALLEGRO_TIMER* fps = al_create_timer(1.0 / 60.0);
    must_init(fps, "timer fps");
    
    ALLEGRO_FONT* font = al_create_builtin_font();
    ALLEGRO_FONT* title = al_load_ttf_font("assets/fonts/pressStart2p.ttf", 32, 0);
    ALLEGRO_FONT* subtitle = al_load_ttf_font("assets/fonts/pressStart2p.ttf", 16, 0);
    ALLEGRO_FONT* subtitle_8 = al_load_ttf_font("assets/fonts/pressStart2p.ttf", 8, 0);
    
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_timer_event_source(fps));

    al_show_mouse_cursor(display);
    
    bool isRunning = true;
    bool redraw = true;
    ALLEGRO_EVENT event;

    unsigned char key[ALLEGRO_KEY_MAX];
    memset(key, 0, sizeof(key));

    Game* game = create_game(GAME_MENU, font, title, subtitle, queue, &event, 200, SCREEN_H / 2, 5, 100);

    al_start_timer(fps);


    // Loop Principal
    while(isRunning){
        al_wait_for_event(queue, &event);

        switch (event.type){
            case ALLEGRO_EVENT_TIMER:
                update_game(game, key, event, (1.0/60));
                
                if(game->state == GAME_CLOSE){
                    isRunning = false;
                    break;
                }
                
                for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
                    key[i] &= ~KEY_SEEN;

                redraw = true;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                key[event.keyboard.keycode] = KEY_SEEN | KEY_DOWN;
                
                // Chama a função refatorada de input
                handle_input(game, key);

                handle_pause(key, game);

                if(key[ALLEGRO_KEY_ESCAPE] && game->state == GAME_MENU && !game->battle->is_selecting_item){
                    isRunning = false;
                } 

                break;

            case ALLEGRO_EVENT_KEY_UP:
                key[event.keyboard.keycode] &= ~KEY_DOWN;
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                isRunning = false;
                break;
        }
   
        if(!isRunning) break;

        if(redraw && al_is_event_queue_empty(queue)){
            al_clear_to_color(al_map_rgb(0, 0, 0));
            draw_game(game);
            al_flip_display();
            redraw = false;
        }
    }

    // Limpeza
    destroy_game(game);
    if(title) al_destroy_font(title);
    if(subtitle) al_destroy_font(subtitle);
    if(font) al_destroy_font(font);
    if(subtitle_8) al_destroy_font(subtitle_8);
    al_destroy_display(display);
    al_destroy_timer(fps);
    al_destroy_event_queue(queue);

    return 0;
}
