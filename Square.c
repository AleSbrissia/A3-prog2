#include <stdlib.h>
#include "Square.h"

square* square_create(int xside, int yside, int x, int y,
	                  int max_x, int max_y){						

	if ((x - xside/2 < 0) || (x + xside/2 > max_x))
	    return NULL;
	if ((y - yside/2 < 0) || (y + yside/2 > max_y))
	    return NULL;

	square *new_square = (square*) malloc(sizeof(square));	
	new_square->xside = xside;
	new_square->yside = yside;
	new_square->x = x;	
	new_square->y = y;	
	new_square->control = joystick_create();
	return new_square;
}

void square_move(square *element, char steps, int trajectory,
	             int max_x, int max_y){

	if (!trajectory){
		if ((element->x - steps*SQUARE_STEP) - element->xside/2 >= 0)
			element->x = element->x - steps*SQUARE_STEP;
	} 
	else if (trajectory == 1){
		if ((element->x + steps*SQUARE_STEP) + element->xside/2 <= max_x)
		    element->x = element->x + steps*SQUARE_STEP;
	}
	else if (trajectory == 2){
		if ((element->y - steps*SQUARE_STEP) - element->yside/2 >= 0)
		    element->y = element->y - steps*SQUARE_STEP;
	}
	else if (trajectory == 3){ 
		if ((element->y + steps*SQUARE_STEP) + element->yside/2 <= max_y)
		    element->y = element->y + steps*SQUARE_STEP;
	}
}

void square_destroy(square *element){
	joystick_destroy(element->control);	
	free(element);
}