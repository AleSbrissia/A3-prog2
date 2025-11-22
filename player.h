#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <stdbool.h>
#include "joystick.h"
#include "fase.h"

#define PLAYER_W 40	
#define PLAYER_H 100	

#define PLAYER_START_X PLAYER_W/2	

#define HEALTH_BAR_X 20
#define HEALTH_BAR_Y 20
#define HEALTH_BAR_W 25
#define HEALTH_BAR_H 25
#define HEALTH_BAR_SPACE 5
#define PLAYER_MAX_HEALTH 5
#define PLAYER_DAMAGE_DALAY 30 //1s invulneravel 

#define PLAYER_STEP 5	
#define PLAYER_JUMP -20

typedef struct {

	int w, h, x, y;	

	joystick *control;	

	bool ground;
	float fall;

	int health, max_health ; 
	bool alive ;

	int damage_dalay ;

} player;		

player* player_create(int xside, int yside, int x, int y,
	                  int max_x, int max_y);
void player_move(player *element, char steps, int trajectory,
	             int max_x, int max_y);
void player_destroy(player *element);

void player_update_movement(player *p, float dt, square *floor) ;

void player_draw_health(player *p) ;

#endif