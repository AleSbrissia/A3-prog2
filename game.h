#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <math.h> 
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>

#include "player.h"
#include "fase.h"

#define X_SCREEN 1280 
#define Y_SCREEN 720
#define H_GROUND 70 
#define Y_FLOOR (Y_SCREEN - H_GROUND) 

#define FPS 30.0 

#define GRAVITY 0.8f

#define MAX_OBSTACLES 5
#define OBSTACLE_SPAWN_INTERVAL 2.0f

typedef enum {
    MENU,
    GAMEPLAY
} game_state;

void draw_menu(ALLEGRO_BITMAP* bg_image, ALLEGRO_FONT *font, int selected_opt)  ;
void draw_gameplay(ALLEGRO_BITMAP *bg, player *p, square *floor) ; 
int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) ;

#endif 
