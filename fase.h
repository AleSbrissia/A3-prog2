#ifndef FASE_H
#define FASE_H


#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int w, h, x, y ;
}square ;


square* square_create(int w, int h, int x, int y, int max_x, int max_y) ;

#endif 
