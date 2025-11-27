#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <math.h> 
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>

#include "platform.h"
#include "obstacle.h"
#include "player.h"
#include "fase.h"

#define X_SCREEN 1280 
#define Y_SCREEN 720
#define H_GROUND 70 
#define Y_FLOOR (Y_SCREEN - H_GROUND) 

#define PHASE_LENGTH (X_SCREEN)*3

#define FPS 30.0 

#define GRAVITY 1.2f
#define SCROLL_SPEED 2.0f

#define MAX_OBSTACLES 10 
#define OBSTACLE_SPAWN_INTERVAL 1.0f

typedef enum {
    MENU,
    GAMEPLAY,
    GAMEOVER,
    VICTORY

} game_state;

void draw_menu(ALLEGRO_BITMAP* bg_image, ALLEGRO_FONT *font, int selected_opt)  ;
void draw_gameover(ALLEGRO_BITMAP* bg, ALLEGRO_FONT *font, int selected_opt) ;
void draw_gameplay(ALLEGRO_BITMAP *bg, player *p, square *floor, platform_manager *plat_manager) ; 
int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) ;
int wasd_keys(player *p1, ALLEGRO_EVENT *ev) ;
int game_set(player **p, square **floor, obstacle_manager **obs_manager, platform_manager **plat_manager, ALLEGRO_BITMAP **bg) ;
int game_clean(player *p, square *floor, obstacle_manager *obs_manager, platform_manager *plat_manager) ;
void draw_victory(ALLEGRO_BITMAP* bg, ALLEGRO_FONT *font, int selected_opt) ;

#endif