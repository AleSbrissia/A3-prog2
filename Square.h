#ifndef __SQUARE__ 																												
#define __SQUARE__																	

#include "Joystick.h"							

#define SQUARE_STEP 10																											

typedef struct {																												
	int xside;
	int yside;	
	unsigned short x;	
	unsigned short y;	
	joystick *control;	

} square;	

square* square_create(int xside, int yside, unsigned short x, unsigned short y,
	                  unsigned short max_x, unsigned short max_y);
void square_move(square *element, char steps, unsigned char trajectory,
	             unsigned short max_x, unsigned short max_y);
void square_destroy(square *element);

#endif																															