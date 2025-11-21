#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "player.h"
#include "joystick.h"
#include "game.h"
#include "fase.h"

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
	new_player->ground = true;	
	new_player->fall = 0 ;
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


// Atualiza o movimento e estado do jogador com base nos controles
void player_update_movement(player *p, float dt, square *floor) {

    if (!p)
		return;

	//const float move_dist = PLAYER_SPEED_PER_SEC * dt;
	int move_dist = PLAYER_STEP ;

    // --- LÓGICA DE MOVIMENTO FÍSICO ---
    // Apenas atualiza a posição e a direção do jogador
    if (p->control->right) {
        p->x += move_dist;
    } else if (p->control->left) {
        p->x -= move_dist;
    }
    if (p->x < p->w/2) {
        p->x = p->w/2;
    }
    if (p->control->up && p->ground) {
        p->fall = PLAYER_JUMP;
        p->ground = false;
    }
    if (!p->ground) {
        p->fall += GRAVITY;
        p->y += p->fall;
        if (p->y >= Y_FLOOR) {
            p->y = Y_FLOOR ;
            p->fall = 0;
            p->ground = true;
        }
    }
}
/*void player_update_movement(player *p, float dt, square *floor) {
    if (!p) return;

    // horizontal input -> dx (per frame). Use PLAYER_STEP as step per tick.
    int dx = 0;
    if (p->control) {
        if (p->control->right) dx += PLAYER_STEP;
        if (p->control->left)  dx -= PLAYER_STEP;
    }

    // --- Horizontal movement + collision resolution on X ---
    if (dx != 0) {
        int new_x = p->x + dx;

        // move on X
        p->x = new_x;

        // if collide with floor horizontally, resolve by pushing out on X
        if (floor && collision(p->x, p->y, p->w, p->h, floor->x, floor->y, floor->w, floor->h)) {
            if (dx > 0) {
                // moving right -> place player to left of floor
                int floor_left = floor->x - floor->w/2;
                p->x = floor_left - p->w/2;
            } else if (dx < 0) {
                // moving left -> place player to right of floor
                int floor_right = floor->x + floor->w/2;
                p->x = floor_right + p->w/2;
            }
        }
    }

    // --- Vertical movement (gravity + jump) ---
    // jump: when pressing up and standing on ground
    if (p->control && p->control->up && p->ground) {
        p->fall = -PLAYER_JUMP; // upward impulse (negative = up)
        p->ground = false;
    }

    // apply gravity to vertical velocity
    p->fall += GRAVITY;          // GRAVITY should be positive (pulls down)
    // integrate vertical movement
    int new_y = (int) (p->y + p->fall);

    // Move on Y
    p->y = new_y;

    // check collision with floor after vertical move
    if (floor && collision(p->x, p->y, p->w, p->h, floor->x, floor->y, floor->w, floor->h)) {
        // if we were falling (positive fall), land on top
        int floor_top = floor->y - floor->h/2;
        int floor_bottom = floor->y + floor->h/2;
        int player_top = p->y - p->h/2;
        int player_bottom = p->y + p->h/2;

        if (p->fall > 0) {
            // landed on top of floor
            p->y = floor_top - p->h/2;
            p->fall = 0;
            p->ground = true;
        } else if (p->fall < 0) {
            // hit head on bottom of floor -> place below
            p->y = floor_bottom + p->h/2;
            p->fall = 0;
            // remain airborne (ground=false)
            p->ground = false;
        } else {
            // zero vertical velocity but overlapping: push up
            p->y = floor_top - p->h/2;
            p->ground = true;
        }
    } else {
        // if not colliding with floor, player is airborne
        p->ground = false;
    }

    // clamp to screen bounds (optional)
    if (p->x - p->w/2 < 0) p->x = p->w/2;
    if (p->x + p->w/2 > X_SCREEN) p->x = X_SCREEN - p->w/2;
    if (p->y - p->h/2 < 0) p->y = p->h/2;
    if (p->y + p->h/2 > Y_SCREEN) {
        p->y = Y_SCREEN - p->h/2;
        p->fall = 0;
        p->ground = true;
    }
}*/


