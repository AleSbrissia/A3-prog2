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
#define Y_GROUND 70 
#define Y_FLOOR (Y_SCREEN - Y_GROUND) - (PLAYER_H / 2)

#define FPS 30.0 

#define GRAVITY 0.8f

typedef enum {
    MENU,
    GAMEPLAY
} game_state;

void draw_menu(ALLEGRO_BITMAP* bg_image, ALLEGRO_FONT *font, int selected_opt)  ;
void draw_gameplay(ALLEGRO_BITMAP *bg, player *p, square *floor) ; 
int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) ;

#endif 
