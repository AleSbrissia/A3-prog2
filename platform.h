#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include <stdio.h>
#include <stdbool.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "joystick.h"

#define MAX_PLATFORMS 3
#define PLATFORM_SPAWN_INTERVAL 3.0f

typedef enum {

    STILL_R,
    STILL_L,
	WALKING_R,
	WALKING_L,
	JUMPING_R,
	JUMPING_L,
	CROUCHING
} player_state;

typedef struct player {

	int w, h, x, y;
	int visual_w, visual_h ;	

	joystick *control;	

	bool ground;
	float fall;

	int health, max_health ; 
	int damage_dalay ;
	bool alive, win ;

	player_state state;
	ALLEGRO_BITMAP **sprites; 

} player;		

typedef struct platform {
    int x, y, w, h;
    bool active;
    ALLEGRO_BITMAP* sprite;
} platform;

typedef struct {
    platform** platforms;
    int count;
    int max_platforms;
    float spawn_timer;
    float spawn_interval;
} platform_manager;

// Funções básicas da plataforma
platform* platform_create(int x, int y, int w, int h);
void platform_destroy(platform* plat);
void platform_reset(platform* plat, int screen_width);

// Sistema de gerenciamento
platform_manager* platform_manager_create(int max_platforms, float spawn_interval);
void platform_manager_update(platform_manager* manager, float delta_time, int screen_width);
void platform_manager_draw(platform_manager* manager);
void platform_manager_destroy(platform_manager* manager);
void platform_manager_reset_all(platform_manager* manager, int screen_width);

// Colisão com player
bool platform_check_collision(platform* plat, player* p);
void platform_handle_collision(platform* plat, player* p);

#endif
