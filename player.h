#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include "joystick.h"
#include "fase.h"

#define PLAYER_STEP 10	
#define PLAYER_JUMP -15

typedef struct {
	int w;
	int h;	
	int x;	
	int y;	
	joystick *control;	

	bool ground;
	float fall;

} player;		

player* player_create(int xside, int yside, int x, int y,
	                  int max_x, int max_y);
void player_move(player *element, char steps, int trajectory,
	             int max_x, int max_y);
void player_destroy(player *element);

void player_update_movement(player *p, float dt, square *floor) ;

#endif