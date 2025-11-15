#include <allegro5/allegro5.h>												
#include <allegro5/allegro_font.h>									
#include <allegro5/allegro_primitives.h>					

#include "Square.h"

#include <stdio.h>														

#define X_SCREEN 1280											
#define Y_SCREEN 720					

/*unsigned char collision_2D(square *a, square *b){

	if ((((a->y +a->yside/2 >= b->y -b->yside/2) &&
	    (b->y -b->yside/2 >= a->y -a->yside/2)) || 
		((b->y +b->yside/2 >= a->y -a->yside/2) &&
		(a->y -a->yside/2 >= b->y -b->yside/2))) && 	
		(((a->x +a->xside/2 >= b->x -b->xside/2) &&
		(b->x -b->xside/2 >= a->x -a->xside/2)) || 
		((b->x +b->xside/2 >= a->x -a->xside/2) &&
		(a->x -a->xside/2 >= b->x -b->xside/2)))) 
		return 1;		
	return 0;
}*/

// Checagem AABB simples e correta
unsigned char collision_2D(square *a, square *b) {
    int a_left   = a->x - a->xside / 2;
    int a_right  = a->x + a->xside / 2;
    int a_top    = a->y - a->yside / 2;
    int a_bottom = a->y + a->yside / 2;

    int b_left   = b->x - b->xside / 2;
    int b_right  = b->x + b->xside / 2;
    int b_top    = b->y - b->yside / 2;
    int b_bottom = b->y + b->yside / 2;

    // se há separação em qualquer eixo, NÃO colidem
    if (a_right < b_left)  return 0; // a está à esquerda de b
    if (a_left  > b_right) return 0; // a está à direita de b
    if (a_bottom < b_top)  return 0; // a está acima de b
    if (a_top    > b_bottom) return 0; // a está abaixo de b

    // caso contrário, há interseção
    return 1;
}





//Função de atualização das posições dos quadrados conforme os comandos do controle
/*void update_position(square *player_1, square *player_2){	

    if (player_1->control) {
		if (player_1->control->left){		
			square_move(player_1, 1, 0, X_SCREEN, Y_SCREEN);	
			if (collision_2D(player_1, player_2))
				square_move(player_1, -1, 0, X_SCREEN, Y_SCREEN);	
		}
		if (player_1->control->right){	
			square_move(player_1, 1, 1, X_SCREEN, Y_SCREEN);
			if (collision_2D(player_1, player_2)) 
				square_move(player_1, -1, 1, X_SCREEN, Y_SCREEN);	
		}
		if (player_1->control->up) {	
			square_move(player_1, 1, 2, X_SCREEN, Y_SCREEN);
			if (collision_2D(player_1, player_2)) 
				square_move(player_1, -1, 2, X_SCREEN, Y_SCREEN);
		}
		if (player_1->control->down){	
			square_move(player_1, 1, 3, X_SCREEN, Y_SCREEN);	
			if (collision_2D(player_1, player_2)) 
				square_move(player_1, -1, 3, X_SCREEN, Y_SCREEN);	
		}
	}

    if (player_2->control) {
		if (player_2->control->left){		
			square_move(player_2, 1, 0, X_SCREEN, Y_SCREEN);	
			if (collision_2D(player_2, player_1)) 
				square_move(player_2, -1, 0, X_SCREEN, Y_SCREEN);	
		}
		
		if (player_2->control->right){ 				
			square_move(player_2, 1, 1, X_SCREEN, Y_SCREEN);	
			if (collision_2D(player_2, player_1)) 
				square_move(player_2, -1, 1, X_SCREEN, Y_SCREEN);	
		}																									
		if (player_2->control->up){			
			square_move(player_2, 1, 2, X_SCREEN, Y_SCREEN);	
			if (collision_2D(player_2, player_1))
				square_move(player_2, -1, 2, X_SCREEN, Y_SCREEN);		
		}
		if (player_2->control->down){							
			square_move(player_2, 1, 3, X_SCREEN, Y_SCREEN);		
			if (collision_2D(player_2, player_1)) 
				square_move(player_2, -1, 3, X_SCREEN, Y_SCREEN);	
		}
    }
}*/

void update_position(square *player_1, square *player_2){
    if (!player_1 || !player_2) return;

    /* cópias temporárias para calcular posições pretendidas usando square_move */
    square temp1 = *player_1;
    square temp2 = *player_2;

    /* aplicar movimentos pretendidos nas cópias (preserva validações de square_move) */
    if (player_1->control){
        if (player_1->control->left)  square_move(&temp1, 1, 0, X_SCREEN, Y_SCREEN);
        if (player_1->control->right) square_move(&temp1, 1, 1, X_SCREEN, Y_SCREEN);
        if (player_1->control->up)    square_move(&temp1, 1, 2, X_SCREEN, Y_SCREEN);
        if (player_1->control->down)  square_move(&temp1, 1, 3, X_SCREEN, Y_SCREEN);
    }

    if (player_2->control){
        if (player_2->control->left)  square_move(&temp2, 1, 0, X_SCREEN, Y_SCREEN);
        if (player_2->control->right) square_move(&temp2, 1, 1, X_SCREEN, Y_SCREEN);
        if (player_2->control->up)    square_move(&temp2, 1, 2, X_SCREEN, Y_SCREEN);
        if (player_2->control->down)  square_move(&temp2, 1, 3, X_SCREEN, Y_SCREEN);
    }

    /* se as posições pretendidas não colidirem, commit */
    if (!collision_2D(&temp1, &temp2)){
        player_1->x = temp1.x; player_1->y = temp1.y;
        player_2->x = temp2.x; player_2->y = temp2.y;
    } else {
        /* política simples: se só um se moveu, permita; se ambos, cancele */
        int moved1 = (temp1.x != player_1->x) || (temp1.y != player_1->y);
        int moved2 = (temp2.x != player_2->x) || (temp2.y != player_2->y);
        if (moved1 && !moved2) { player_1->x = temp1.x; player_1->y = temp1.y; }
        else if (!moved1 && moved2) { player_2->x = temp2.x; player_2->y = temp2.y; }
        /* se ambos moveram -> cancelar ambos (ou implementar outra política) */
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
	if(!timer || !queue || !font || !disp)
		return -1 ;	

	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(disp));
	al_register_event_source(queue, al_get_timer_event_source(timer));

	square* player_1 = square_create(20, 20, 10, Y_SCREEN/2, X_SCREEN, Y_SCREEN);
	if (!player_1) 
	    return 1;
	square* player_2 = square_create(100, 20, X_SCREEN/2, Y_SCREEN/2, X_SCREEN, Y_SCREEN);
	if (!player_2) 
	    return 2;
	square* floor = square_create(X_SCREEN, 200, X_SCREEN/2, Y_SCREEN -200, X_SCREEN, Y_SCREEN);
	if (!floor) 
	    return 3;
	joystick_destroy(floor->control) ;

	ALLEGRO_EVENT event;
	al_start_timer(timer);
	while(1){
		al_wait_for_event(queue, &event);

		if (event.type == 30){
			update_position(player_1, floor);
			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_draw_filled_rectangle(player_1->x -player_1->xside/2, player_1->y -player_1->yside/2,
				                     player_1->x +player_1->xside/2, player_1->y +player_1->yside/2,
									 al_map_rgb(255, 0, 0));	
			al_draw_filled_rectangle(player_2->x -player_2->xside/2, player_2->y -player_2->yside/2,
				                     player_2->x +player_2->xside/2, player_2->y +player_2->yside/2, 
									 al_map_rgb(0, 255, 0));	
			al_draw_filled_rectangle(floor->x -floor->xside/2, floor->y -floor->yside/2,
				                     floor->x +floor->xside/2, floor->y +floor->yside/2, 
									 al_map_rgb(0, 255, 0));	
    		al_flip_display();
		}
		else if ((event.type == 10) || (event.type == 12)) {

			if (event.keyboard.keycode == 1) 
			    joystick_left(player_1->control);			
			else if (event.keyboard.keycode == 4) 
			    joystick_right(player_1->control);		
			else if (event.keyboard.keycode == 23) 
			    joystick_up(player_1->control);				
			else if (event.keyboard.keycode == 19) 
			    joystick_down(player_1->control);				
			else if (event.keyboard.keycode == 82) 
			    joystick_left(player_2->control);						
			else if (event.keyboard.keycode == 83) 
			    joystick_right(player_2->control);							
			else if (event.keyboard.keycode == 84) 
			    joystick_up(player_2->control);										
			else if (event.keyboard.keycode == 85) 
			    joystick_down(player_2->control);										
		}
		else if (event.type == 42) break;								
	}

	al_destroy_font(font);	
	al_destroy_display(disp);																																						
	al_destroy_timer(timer);	
	al_destroy_event_queue(queue);	

	return 0;
}
