#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_

#include <stdio.h>
#include <stdbool.h>
#include <allegro5/allegro5.h>			
#include <allegro5/allegro_primitives.h>	
#include <allegro5/allegro_image.h>

#include "fase.h"
#include "player.h"

#define DIFFERENT_OBSTACLES 6
typedef enum {
    stem,
    arrow,
    stone,
    spike_down,
    spike_up,
    spear
    
} obstacle_type;

typedef struct {
    int w, h, x, y;
    int visual_w, visual_h ;
    float speed_x, speed_y;
    bool active;
    obstacle_type type;
    ALLEGRO_BITMAP* sprite;
    ALLEGRO_COLOR color;
} obstacle;

// Sistema de gerenciamento de múltiplos obstáculos
typedef struct {
    obstacle** obstacles;
    ALLEGRO_BITMAP** obstacles_sprites ; 
    int count;
    int max_obstacles;
    float spawn_timer;
    float spawn_interval;
    float scroll_speed; // Para rolling background
} obstacle_manager;

// Funções básicas do obstáculo
obstacle* obstacle_create(int x, int y, int w, int h, int v_w, int v_h, float speed_x, float speed_y, obstacle_type type, ALLEGRO_BITMAP *sprite) ;
void obstacle_destroy(obstacle* obs);
void obstacle_reset(obstacle* obs, int screen_width, int y_floor) ;

// Movimento e colisão
int obstacle_update_movement(obstacle* obs, player *p, int screen_width, int y_floor, int gravity) ;
float obstacle_relative_speed(obstacle* obs, player* p) ;
bool obstacle_check_collision(obstacle* obs, player* p);
int draw_obstacle(obstacle* obs);

// Sistema de gerenciamento
obstacle_manager* obstacle_manager_create(int max_obs, float spawn_interval, float scroll_speed) ;
void obstacle_manager_update(obstacle_manager* manager, float delta_time, player* player, int screen_width, int y_floor, int gravity) ;
void obstacle_manager_draw(obstacle_manager* manager);
void obstacle_manager_destroy(obstacle_manager* manager);
void obstacle_manager_reset_all(obstacle_manager* manager, int screen_width, int y_floor) ;

#endif