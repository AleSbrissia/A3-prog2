#include <stdio.h>														
#include <stdlib.h>
#include <stdbool.h>
#include <allegro5/allegro5.h>			
#include <allegro5/allegro_font.h>	
#include <allegro5/allegro_ttf.h>			
#include <allegro5/allegro_primitives.h>	
#include <allegro5/allegro_image.h>	

#include "player.h"
#include "game.h"
#include "fase.h"
#include "obstacle.h"

int main() {

	al_init();	
	al_init_primitives_addon();	
	al_init_font_addon() ;
	al_install_keyboard();
	al_init_image_addon();
	al_init_ttf_addon();

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);	
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();	
	ALLEGRO_FONT* font = NULL ;	
	ALLEGRO_DISPLAY* disp = al_create_display(X_SCREEN, Y_SCREEN);
    ALLEGRO_BITMAP* game_bg_img = al_load_bitmap("assets/bg.png") ;

	if(!timer || !queue || !disp)
		return -1 ;	

	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(disp));
	al_register_event_source(queue, al_get_timer_event_source(timer));

	if (!game_bg_img) {       

		fprintf(stderr, "erro carregamento de sprite\n") ;
	}

	font = al_load_font("assets/Daydream.ttf", 36, 0) ;
	if (!font) {

		fprintf(stderr, "NAO TEM FONTE\n") ;
		font = al_create_builtin_font() ;
		if(!font)
			return -1 ;
	}

	player* p1 = NULL ;
	square* floor = NULL ;
    obstacle_manager* obs_manager = NULL ;
    game_state estado = MENU ;
	bool done = false;
	bool redraw = true ; 
	int menu_select = 1 ;

    game_set(&p1, &floor, &obs_manager) ;

	al_start_timer(timer);

	while(!done){
		ALLEGRO_EVENT event;
		al_wait_for_event(queue, &event);

        switch (event.type)
		{
			case ALLEGRO_EVENT_TIMER :
                
				if(estado == GAMEPLAY) {
					player_update_movement(p1, 1.0/FPS, floor) ;
					obstacle_manager_update(obs_manager, 1.0/FPS, p1, X_SCREEN, Y_FLOOR, GRAVITY) ;

					if(p1->health == 0) {
						p1->alive = false ;
						estado = GAMEOVER ;
					}
				}
				redraw = true ;
			break;

			case ALLEGRO_EVENT_KEY_DOWN :

			    switch (estado)
				{
					case GAMEPLAY : 
						wasd_keys(p1, &event) ;

					break;

					case MENU:

						if (event.keyboard.keycode == ALLEGRO_KEY_UP) 
						    menu_select = 1;
						else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) 
							menu_select = 2;
						else if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
							if (menu_select == 1) {
								estado = GAMEPLAY;
							} 
							else {
								done = true;
							}
						}
					break;

					case GAMEOVER :

						if (event.keyboard.keycode == ALLEGRO_KEY_UP) 
						    menu_select = 1;
						else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) 
							menu_select = 2;
						else if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
							if (menu_select == 1) {

								obstacle_manager_reset_all(obs_manager, X_SCREEN, Y_FLOOR) ;
								game_clean(p1, floor, obs_manager) ;
								game_set(&p1, &floor, &obs_manager) ;
								estado = GAMEPLAY;
							} 
							else {
								done = true;
							}
						}
					break;

						
					redraw = true ;
				}

			break;

			case ALLEGRO_EVENT_KEY_UP :
				wasd_keys(p1, &event) ;
				
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
					al_clear_to_color(al_map_rgb(0, 0, 0));
					draw_menu(game_bg_img, font, menu_select) ;
				
				break;

				case GAMEOVER:
					al_clear_to_color(al_map_rgb(0, 0, 0));
					draw_gameover(game_bg_img, font, menu_select) ;
				
				break;

				case GAMEPLAY : 
					al_clear_to_color(al_map_rgb(0, 0, 0));
					draw_gameplay(game_bg_img, p1, floor) ;
					obstacle_manager_draw(obs_manager) ;

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
	player_destroy(p1) ;
	obstacle_manager_destroy(obs_manager) ;

	return 0;
}