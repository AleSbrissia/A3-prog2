#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <stdbool.h>
#include "joystick.h"
#include "fase.h"
#include "platform.h"

#define PLAYER_W 50	
#define PLAYER_H 100	
#define PLAYER_H_CROUCHED 50	
#define PLAYER_W_CROUCHED 30	
#define PLAYER_STATES 7

#define PLAYER_START_X PLAYER_W/2	

#define HEALTH_BAR_X 20
#define HEALTH_BAR_Y 20
#define HEALTH_BAR_W 25
#define HEALTH_BAR_H 25
#define HEALTH_BAR_SPACE 5
#define PLAYER_MAX_HEALTH 5
#define PLAYER_DAMAGE_DALAY 30 //1s sem dano 

#define PLAYER_STEP 3 
#define PLAYER_JUMP -20

player* player_create(int xside, int yside, int x, int y,int max_x, int max_y);
void player_move(player *element, char steps, int trajectory, int max_x, int max_y);
void player_destroy(player *element);
void player_update_movement(player *p, float dt, square *floor, platform_manager *plat_manager) ;
void player_draw_health(player *p) ;
void draw_player(player *p) ;
void player_update_state(player *p, player_state old_st) ;

#endif
