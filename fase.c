#include <stdio.h>
#include <stdlib.h>

#include "fase.h"

square* square_create(int w, int h, int x, int y, int max_x, int max_y){

	if ((x - w/2 < 0) || (x + w/2 > max_x))
	    return NULL;
	if ((y - h/2 < 0) || (y + h/2 > max_y))
	    return NULL;

	square *t = (square*) malloc(sizeof(square));
	t->w = w;
	t->h = h;
	t->x = x;
	t->y = y;
	return t;
}
