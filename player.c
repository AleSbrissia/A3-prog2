#include <stdio.h>
#include <stdlib.h>

#include "player.h"
#include "joystick.h"

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

void square_destroy(player *element){
	joystick_destroy(element->control);	
	free(element);
}