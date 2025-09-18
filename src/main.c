#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include "game.h"
#include "time.h"
#include "screen.h"

#define KEY_SEEN 1
#define KEY_DOWN 2

void must_init(bool test, char* description){
    if(test) return;

    printf("Não foi possível inicializar %s", description);
    exit(1);
}

int main(){
    srand(time(NULL));

    /*
        1° Inicialização 
            Obrigatórios
                - Allegro
                - Display (Ponteiros)
                - Timer (FPS) (Ponteiro)
                - Font (Ponteiro)
                - Fila de eventos (Ponteiro)
            Essenciais/Opcionais
                - Teclado, Mouse, Controle (Tudo que precisa para conseguir jogar...)
                    - Entrar na fila de eventos
        2° Loop principal do jogo
            - Lógica
            - Renderização
        3° Liberação de memória (Destroyers)
    */

    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");
    must_init(al_init_primitives_addon(), "primitives");
    must_init(al_init_font_addon(), "font");

    ALLEGRO_DISPLAY* display = al_create_display(SCREEN_W, SCREEN_H);
    ALLEGRO_TIMER* fps = al_create_timer(1.0 / 60.0);
    ALLEGRO_FONT* font = al_create_builtin_font();
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();

    ALLEGRO_TIMER* timer_enemy = al_create_timer(3.0);

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_timer_event_source(fps));
    al_register_event_source(queue, al_get_timer_event_source(timer_enemy));

    bool isRunning = true;
    bool redraw = true;

    ALLEGRO_EVENT event;

    al_start_timer(fps);

    Game* game = create_game(GAME_EXPLORING, font, 200, SCREEN_H / 2, 5, 100);

    unsigned char key[ALLEGRO_KEY_MAX];
    memset(key, 0, sizeof(key));
    while(isRunning){
        al_wait_for_event(queue, &event);

        switch (event.type){
            case ALLEGRO_EVENT_TIMER:
                
                update_game(game, key, event, timer_enemy);

                for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
                    key[i] &= ~KEY_SEEN;

                redraw = true;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                key[event.keyboard.keycode] = KEY_SEEN | KEY_DOWN;

                if(game->state == GAME_BATTLE){
                    if(key[ALLEGRO_KEY_SPACE]) {
                        game->battle->turn_choice = ATTACK;
                    }
                }

                if(key[ALLEGRO_KEY_ESCAPE]) isRunning = false;

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
        }
    }

    al_destroy_display(display);
    al_destroy_font(font);
    al_destroy_timer(fps);
    al_destroy_timer(timer_enemy);
    al_destroy_event_queue(queue);
}