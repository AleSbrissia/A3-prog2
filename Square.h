#ifndef __SQUARE__ 	
#define __SQUARE__

#include "Joystick.h"

#define SQUARE_STEP 10	

typedef struct {
	int xside;
	int yside;	
	int x;	
	int y;	
	joystick *control;	

} square;	

square* square_create(int xside, int yside, int x, int y,
	                  int max_x, int max_y);
void square_move(square *element, char steps, int trajectory,
	             int max_x, int max_y);
void square_destroy(square *element);

#endif