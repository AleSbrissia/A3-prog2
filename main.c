#include <stdio.h>														
#include <stdlib.h>
#include <stdbool.h>
#include <allegro5/allegro5.h>			
#include <allegro5/allegro_font.h>	
#include <allegro5/allegro_primitives.h>	
#include <allegro5/allegro_image.h>	

#include "player.h"
#include "game.h"
#include "fase.h"

int main(){

	al_init();	
	al_init_primitives_addon();	
	al_init_font_addon() ;
	al_install_keyboard();
	al_init_image_addon();

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);	
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();	
	ALLEGRO_FONT* font = al_create_builtin_font();	
	ALLEGRO_DISPLAY* disp = al_create_display(X_SCREEN, Y_SCREEN);
    ALLEGRO_BITMAP* game_bg_img = NULL ;

	if(!timer || !queue || !font || !disp)
		return -1 ;	

	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(disp));
	al_register_event_source(queue, al_get_timer_event_source(timer));

    game_bg_img = al_load_bitmap("assets/bg.png")  ;
	if (!game_bg_img) {
        
		fprintf(stderr, "NAO TEM BG\n") ;
	}

    /* escolha do tamanho do jogador */
	int player_w = PLAYER_W;
	int player_h = PLAYER_H;
	int player_start_x = 10;

	/* floor/ground definido pela constante Y_GROUND (altura) */
	int floor_h = Y_GROUND;
	int floor_center_y = Y_SCREEN - floor_h / 2;

	/* criar jogador já posicionado em cima do chão */
	int player_start_y = Y_FLOOR ; // floor_top - player_h/2

	//int player_start_y = (Y_SCREEN - floor_h) - (player_h / 2);

	player* p1 = player_create(player_w, player_h, player_start_x, player_start_y, X_SCREEN, Y_SCREEN);
	/* criar o objeto chão com centro calculado */
	square* floor = square_create(X_SCREEN, floor_h, X_SCREEN/2, floor_center_y, X_SCREEN, Y_SCREEN);

	if(!p1 || !floor)
		return -1 ;

	al_start_timer(timer);
    game_state estado = MENU ;
	bool done = false;
	bool redraw = true ; 
	int menu_select = 1 ;

	while(!done){
		ALLEGRO_EVENT event;
		al_wait_for_event(queue, &event);

        switch (event.type)
		{
			case ALLEGRO_EVENT_TIMER :
                
				if(estado == GAMEPLAY) {
					player_update_movement(p1, 1.0/FPS, floor) ;

				}
				redraw = true ;
			break;

			case ALLEGRO_EVENT_KEY_DOWN :

			    switch (estado)
				{
					case GAMEPLAY : 
						if (event.keyboard.keycode == 1) 
							joystick_left(p1->control);
						else if (event.keyboard.keycode == 4) 
							joystick_right(p1->control);	
						else if (event.keyboard.keycode == 23) 
							joystick_up(p1->control);
						else if (event.keyboard.keycode == 19)
							joystick_down(p1->control);	
					break;

					case MENU:

						if (event.keyboard.keycode == ALLEGRO_KEY_UP) menu_select = 1;
						else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) menu_select = 2;
						else if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
							if (menu_select == 1) {
								// Cria os objetos do jogo ao iniciar
								if (!p1) 
								    return -1 ;//p1 = player_create(20, 20, 10, Y_SCREEN/2, X_SCREEN, Y_SCREEN);

								estado = GAMEPLAY;
						} else {
							done = true;
						}
					}
					break;
				    
					redraw = true ;
				}

			break;

			case ALLEGRO_EVENT_KEY_UP :

			    //movimentacao no WASD
				if (event.keyboard.keycode == 1) 
					joystick_left(p1->control);
				if (event.keyboard.keycode == 4) 
					joystick_right(p1->control);	
				if (event.keyboard.keycode == 23) 
					joystick_up(p1->control);
				if (event.keyboard.keycode == 19)
					joystick_down(p1->control);	
				
			break ;
			

			case ALLEGRO_EVENT_DISPLAY_CLOSE :
			    done = true ;
		    break ;
		
		}
		
		//desenho 
		if (redraw && al_is_event_queue_empty(queue)) {
			redraw = false ;

			switch (estado)
			{
				case MENU:
					draw_menu(NULL, font, menu_select) ;
				
				break;

				case GAMEPLAY : 
					draw_gameplay(game_bg_img, p1, floor) ;

				break;
			}
			al_flip_display() ;
		}
	}

    al_destroy_font(font);	
    al_destroy_display(disp);	
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    if (game_bg_img) al_destroy_bitmap(game_bg_img);  

	return 0;
}
