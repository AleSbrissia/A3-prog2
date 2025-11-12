#include <allegro5/allegro5.h>												
#include <allegro5/allegro_font.h>									
#include <allegro5/allegro_primitives.h>					

#include "Square.h"

#include <stdio.h>														

#define X_SCREEN 1280											
#define Y_SCREEN 720					

unsigned char collision_2D(square *element_first, square *element_second){

	if ((((element_first->y+element_first->side/2 >= element_second->y-element_second->side/2) &&
	(element_second->y-element_second->side/2 >= element_first->y-element_first->side/2)) || 				
		((element_second->y+element_second->side/2 >= element_first->y-element_first->side/2) &&
		(element_first->y-element_first->side/2 >= element_second->y-element_second->side/2))) && 	
		(((element_first->x+element_first->side/2 >= element_second->x-element_second->side/2) &&
		(element_second->x-element_second->side/2 >= element_first->x-element_first->side/2)) || 
		((element_second->x+element_second->side/2 >= element_first->x-element_first->side/2) &&
		(element_first->x-element_first->side/2 >= element_second->x-element_second->side/2)))) return 1;		
	else return 0;
}

//Função de atualização das posições dos quadrados conforme os comandos do controle
void update_position(square *player_1, square *player_2){				
	if (player_1->control->left){																				
		square_move(player_1, 1, 0, X_SCREEN, Y_SCREEN);														
		if (collision_2D(player_1, player_2)) square_move(player_1, -1, 0, X_SCREEN, Y_SCREEN);	
	}
	if (player_1->control->right){																				
		square_move(player_1, 1, 1, X_SCREEN, Y_SCREEN);														
		if (collision_2D(player_1, player_2)) square_move(player_1, -1, 1, X_SCREEN, Y_SCREEN);					
	}
	if (player_1->control->up) {																				
		square_move(player_1, 1, 2, X_SCREEN, Y_SCREEN);														
		if (collision_2D(player_1, player_2)) square_move(player_1, -1, 2, X_SCREEN, Y_SCREEN);					
	}
	if (player_1->control->down){																				
		square_move(player_1, 1, 3, X_SCREEN, Y_SCREEN);														
		if (collision_2D(player_1, player_2)) square_move(player_1, -1, 3, X_SCREEN, Y_SCREEN);					
	}

	if (player_2->control->left){																				
		square_move(player_2, 1, 0, X_SCREEN, Y_SCREEN);																																				
		if (collision_2D(player_2, player_1)) square_move(player_2, -1, 0, X_SCREEN, Y_SCREEN);																											
	}
	
	if (player_2->control->right){ 																																										
		square_move(player_2, 1, 1, X_SCREEN, Y_SCREEN);																																				
		if (collision_2D(player_2, player_1)) square_move(player_2, -1, 1, X_SCREEN, Y_SCREEN);	
	}																									
	if (player_2->control->up){																																											
		square_move(player_2, 1, 2, X_SCREEN, Y_SCREEN);																																				
		if (collision_2D(player_2, player_1)) square_move(player_2, -1, 2, X_SCREEN, Y_SCREEN);																														
	}
	if (player_2->control->down){																																										
		square_move(player_2, 1, 3, X_SCREEN, Y_SCREEN);																																				
		if (collision_2D(player_2, player_1)) square_move(player_2, -1, 3, X_SCREEN, Y_SCREEN);																											
	}
}

int main(){
	al_init();																																															
	al_init_primitives_addon();																																											
	
	al_install_keyboard();																																												

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);																																				
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();																									
	ALLEGRO_FONT* font = al_create_builtin_font();																												
	ALLEGRO_DISPLAY* disp = al_create_display(X_SCREEN, Y_SCREEN);																	

	al_register_event_source(queue, al_get_keyboard_event_source());																									
	al_register_event_source(queue, al_get_display_event_source(disp));																															
	al_register_event_source(queue, al_get_timer_event_source(timer));																																	

	square* player_1 = square_create(20, 10, Y_SCREEN/2, X_SCREEN, Y_SCREEN);																										
	if (!player_1) return 1;																																										
	square* player_2 = square_create(20, X_SCREEN-10, Y_SCREEN/2, X_SCREEN, Y_SCREEN);												
	if (!player_2) return 2;																																											

	ALLEGRO_EVENT event;																																												
	al_start_timer(timer);																																												
	while(1){																																															
		al_wait_for_event(queue, &event);																																								

		if (event.type == 30){
			update_position(player_1, player_2);																																						
			al_clear_to_color(al_map_rgb(0, 0, 0));																																	
			al_draw_filled_rectangle(player_1->x-player_1->side/2, player_1->y-player_1->side/2, player_1->x+player_1->side/2, player_1->y+player_1->side/2, al_map_rgb(255, 0, 0));					
			al_draw_filled_rectangle(player_2->x-player_2->side/2, player_2->y-player_2->side/2, player_2->x+player_2->side/2, player_2->y+player_2->side/2, al_map_rgb(0, 0, 255));					
    		al_flip_display();																																							
		}
		else if ((event.type == 10) || (event.type == 12)) {

			if (event.keyboard.keycode == 1) joystick_left(player_1->control);			
			else if (event.keyboard.keycode == 4) joystick_right(player_1->control);		
			else if (event.keyboard.keycode == 23) joystick_up(player_1->control);				
			else if (event.keyboard.keycode == 19) joystick_down(player_1->control);				
			else if (event.keyboard.keycode == 82) joystick_left(player_2->control);						
			else if (event.keyboard.keycode == 83) joystick_right(player_2->control);							
			else if (event.keyboard.keycode == 84) joystick_up(player_2->control);										
			else if (event.keyboard.keycode == 85) joystick_down(player_2->control);										
		}
		else if (event.type == 42) break;								
	}

	al_destroy_font(font);																																								
	al_destroy_display(disp);																																						
	al_destroy_timer(timer);																																									
	al_destroy_event_queue(queue);																																		

	return 0;
}
