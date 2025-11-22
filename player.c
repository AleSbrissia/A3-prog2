#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "player.h"
#include "joystick.h"
#include "game.h"
#include "fase.h"

player* player_create(int xside, int yside, int x, int y, int max_x, int max_y){						

	if ((x - xside/2 < 0) || (x + xside/2 > max_x))
	    return NULL;
	if ((y - yside/2 < 0) || (y + yside/2 > max_y))
	    return NULL;

	player *new_player = (player*) malloc(sizeof(player));	
	new_player->w = xside;
	new_player->h = yside;
	new_player->x = x;	
	new_player->y = y;	
	new_player->ground = true;	
	new_player->fall = 0 ;
	new_player->control = joystick_create();
	return new_player;
}

void player_move(player *element, char steps, int trajectory, int max_x, int max_y){

	if (!trajectory){
		if ((element->x - steps*PLAYER_STEP) - element->w/2 >= 0)
			element->x = element->x - steps*PLAYER_STEP;
	} 
	else if (trajectory == 1){
		if ((element->x + steps*PLAYER_STEP) + element->w/2 <= max_x)
		    element->x = element->x + steps*PLAYER_STEP;
	}
	else if (trajectory == 2){
		if ((element->y - steps*PLAYER_STEP) - element->h/2 >= 0)
		    element->y = element->y - steps*PLAYER_STEP;
	}
	else if (trajectory == 3){ 
		if ((element->y + steps*PLAYER_STEP) + element->h/2 <= max_y)
		    element->y = element->y + steps*PLAYER_STEP;
	}
}

void player_destroy(player *element){
	joystick_destroy(element->control);	
	free(element);
}


// Atualiza o movimento e estado do jogador com base nos controles
void player_update_movement(player *p, float dt, square *floor) {

    if (!p || !p->control) {
		fprintf(stderr, "NULL pointer player_update \n") ;
		return;
	}

	//const float move_dist = PLAYER_SPEED_PER_SEC * dt;
	int move_dist = PLAYER_STEP ;

    // --- LÓGICA DE MOVIMENTO FÍSICO ---
    // Apenas atualiza a posição e a direção do jogador
    if (p->control->right) {
        p->x += move_dist;
    } else if (p->control->left) {
        p->x -= move_dist;
    }
    if (p->x < p->w/2) {
        p->x = p->w/2;
    }
	if (p->x + p->w/2 > X_SCREEN) {
		p->x = X_SCREEN - p->w/2;  // Encosta na borda direita
    }
    if (p->y - p->h/2 < 0) {
        p->y = p->h/2;  // Encosta no topo
    }

    if (p->control->up && p->ground) {
        p->fall = PLAYER_JUMP;
        p->ground = false;
    }
    if (!p->ground) {
        p->fall += GRAVITY;
        p->y += p->fall;
        if (p->y >= Y_FLOOR) {
            p->y = Y_FLOOR ;
            p->fall = 0;
            p->ground = true;
        }
    }
}

