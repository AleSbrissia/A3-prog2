#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <stdbool.h>
#include "joystick.h"
#include "fase.h"

#define PLAYER_W 20	
#define PLAYER_H 40	

#define PLAYER_START_X PLAYER_W/2	

#define PLAYER_STEP 5	
#define PLAYER_JUMP -20

typedef struct {
	int w;
	int h;	
	int x;	
	int y;	
	joystick *control;	

	bool ground;
	float fall;

	int health, max_health ; 

} player;		

player* player_create(int xside, int yside, int x, int y,
	                  int max_x, int max_y);
void player_move(player *element, char steps, int trajectory,
	             int max_x, int max_y);
void player_destroy(player *element);

void player_update_movement(player *p, float dt, square *floor) ;

#endif