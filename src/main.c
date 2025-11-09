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

#define KEY_SEEN 1
#define KEY_DOWN 2

void must_init(bool test, char* description){
    if(test) return;

    printf("Não foi possível inicializar %s\n", description);
    exit(1);
}

int main(){
    srand(time(NULL));

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
    must_init(font, "font builtin");
    
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
    al_start_timer(fps);

    Game* game = create_game(GAME_MENU, font, title, subtitle, queue, &event, 200, SCREEN_H / 2, 5, 100);

    unsigned char key[ALLEGRO_KEY_MAX];
    memset(key, 0, sizeof(key));
    
    while(isRunning){
        al_wait_for_event(queue, &event);

        // Update/Lógica
        switch (event.type){
            case ALLEGRO_EVENT_TIMER:
                update_game(game, key, event, (1.0/60));
                
                if(game->state == GAME_OVER){
                    isRunning = false;
                    break;
                }
                
                for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
                    key[i] &= ~KEY_SEEN;

                redraw = true;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                key[event.keyboard.keycode] = KEY_SEEN | KEY_DOWN;

                if(game->gameplay_state == GAMEPLAY_EXPLORING) {
                    select_item(game->player, key);
                }

                if(game->state == GAME_INIT){
                    if(key[ALLEGRO_KEY_E]){
                        game->init_dialogues++;
                    }
                }

                if(game->battle->state == BATTLE_DIALOGUE){ 
                    if(key[ALLEGRO_KEY_E]){


                        if(game->battle->dialogues == DIALOGUE_FINAL_INTRO){
                            game->battle->state = BATTLE_START;
                        } 

                        game->battle->dialogues++;
                    }
                }

                if(key[ALLEGRO_KEY_Y]) game->player->entity.hp = game->player->entity.hp - 20;
                if(key[ALLEGRO_KEY_U]) game->player->entity.hp = game->player->entity.hp - 15;
                if(key[ALLEGRO_KEY_I]) game->player->entity.hp = game->player->entity.hp - 10;
                if(key[ALLEGRO_KEY_O]) game->player->entity.hp = game->player->entity.hp - 1;


                if(key[ALLEGRO_KEY_SPACE] && game->gameplay_state == GAMEPLAY_EXPLORING) {
                    game->player->entity.anim_state = ANIM_ATTACK;
                }

                if(key[ALLEGRO_KEY_SPACE] && game->gameplay_state == GAMEPLAY_BATTLE && game->battle->turn_state == TURN_PLAYER){
                    game->player->turn_choice = TURN_ATTACK;
                }

                if(key[ALLEGRO_KEY_ESCAPE]){
                    isRunning = false;
                    break;
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

        // Render
        if(redraw && al_is_event_queue_empty(queue)){
            al_clear_to_color(al_map_rgb(0, 0, 0));

            draw_game(game);

            al_flip_display();
            redraw = false;
        }
    }

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
