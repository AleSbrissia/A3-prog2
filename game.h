#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>

#include "player.h"
#include "fase.h"

#define X_SCREEN 1280
#define Y_SCREEN 720

typedef enum {
    MENU,
    GAMEPLAY
} game_state;

void draw_menu(ALLEGRO_BITMAP* bg_image, ALLEGRO_FONT *font, int selected_opt)  ;
void draw_game(player *p, square *floor) ; 
int collision_2D(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) ;
void update_position(player *player_1) ;

#endif 
