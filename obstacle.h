#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_

#include <stdio.h>
#include <stdbool.h>
#include <allegro5/allegro5.h>			
#include <allegro5/allegro_primitives.h>	
#include <allegro5/allegro_image.h>

#include "fase.h"
#include "game.h"
#include "player.h"

typedef struct {
    int w, h, x, y;
    float speed_x, speed_y;
    bool active;
    ALLEGRO_BITMAP* sprite;
    ALLEGRO_COLOR color;
} obstacle;

// Sistema de gerenciamento de múltiplos obstáculos
typedef struct {
    obstacle** obstacles;
    int count;
    int max_obstacles;
    float spawn_timer;
    float spawn_interval;
    float scroll_speed; // Para rolling background
} obstacle_manager;

// Funções básicas do obstáculo
obstacle* obstacle_create(int x, int y, int w, int h, float speed_x, float speed_y, const char* sprite_path);
void obstacle_destroy(obstacle* obs);
void obstacle_reset(obstacle* obs, int screen_width);

// Movimento e colisão
int obstacle_update_movement(obstacle* obs, int screen_width);
bool obstacle_check_collision(obstacle* obs, player* p);
int draw_obstacle(obstacle* obs);

// Sistema de gerenciamento
obstacle_manager* obstacle_manager_create(int max_obs, float spawn_interval, float scroll_speed);
void obstacle_manager_update(obstacle_manager* manager, float delta_time, player* player, int screen_width);
void obstacle_manager_draw(obstacle_manager* manager);
void obstacle_manager_destroy(obstacle_manager* manager);
void obstacle_manager_reset_all(obstacle_manager* manager, int screen_width);

#endif