#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <allegro5/allegro_primitives.h>	
#include <allegro5/allegro5.h>			
#include <allegro5/allegro_image.h>

#include "player.h"
#include "joystick.h"
#include "game.h"
#include "fase.h"
#include "platform.h"

player* player_create(int xside, int yside, int x, int y, int max_x, int max_y, ALLEGRO_BITMAP *bg){	

	if ((x - xside/2 < 0) || (x + xside/2 > max_x))
	    return NULL;
	if ((y - yside/2 < 0) || (y + yside/2 > max_y))
	    return NULL;

	player *new_player = (player*) malloc(sizeof(player));	
	new_player->w = xside;
	new_player->h = yside;
	new_player->x = x;	
	new_player->y = y;	
    new_player->visual_w = xside ;
    new_player->visual_h = yside ;

	new_player->ground = true;	
	new_player->fall = 0 ;
    new_player->state = STILL_R;

    new_player->max_health = PLAYER_MAX_HEALTH ;
	new_player->health = new_player->max_health ;
	new_player->alive = true ;
	new_player->win = false ;
	new_player->damage_dalay = 0 ;

    set_player_scroll(bg, new_player) ;

	new_player->control = joystick_create();
    new_player->sprites = malloc(sizeof(ALLEGRO_BITMAP*) * PLAYER_STATES);
    
    // Carrega cada sprite
    new_player->sprites[STILL_R] = al_load_bitmap("assets/sprites/player/still.png");
    new_player->sprites[STILL_L] = al_load_bitmap("assets/sprites/player/still_left.png");
    new_player->sprites[WALKING_R] = al_load_bitmap("assets/sprites/player/run.png");
    new_player->sprites[WALKING_L] = al_load_bitmap("assets/sprites/player/run_left.png");
    new_player->sprites[JUMPING_R] = al_load_bitmap("assets/sprites/player/jump.png");
    new_player->sprites[JUMPING_L] = al_load_bitmap("assets/sprites/player/jump_left.png");
    new_player->sprites[CROUCHING] = al_load_bitmap("assets/sprites/player/crouched2.png");
    
    // Verifica se carregou
    for (int i = 0; i < PLAYER_STATES; i++) {
        if (!new_player->sprites[i]) {
            printf("ERRO: Não carregou sprite %d\n", i);
        }
    }

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

void player_destroy(player *element){
	joystick_destroy(element->control);	
	free(element);
}

// Atualiza o movimento e estado do jogador com base nos controles
void player_update_movement(player *p, float dt, square *floor, platform_manager *plat_manager) {

    if (!p || !p->control) {
		fprintf(stderr, "NULL pointer player_update \n") ;
		return;
	}

    //ARRUMAR
    //colisão com plataformas 
    bool on_platform = false;
    if (plat_manager) {

        for (int i = 0; i < plat_manager->max_platforms; i++) 
            if (plat_manager->platforms[i] && plat_manager->platforms[i]->active) 
                if (platform_check_collision(plat_manager->platforms[i], p)) {

                    platform_handle_collision(plat_manager->platforms[i], p);
                    on_platform = true;
                    break;
                }
    }

	int move_dist = PLAYER_STEP ;
    player_state old_st = p->state ;

	if(p->damage_dalay != 0)
		p->damage_dalay-- ;

    //controla os estados

	//agachado
    if (p->control->down && (p->ground || on_platform)) {
 
		p->state = CROUCHING ;
    }
	//andando 
    if ((p->control->right) && (p->ground || on_platform)) {
        p->state = WALKING_R ;
    }
    if ((p->control->left) && (p->ground || on_platform)) {
        p->state = WALKING_L ;
    }
	//parado
    if(!p->control->left && !p->control->right && !p->control->up && !p->control->down && (p->ground || on_platform)) {

        if (old_st == JUMPING_R || old_st == WALKING_R || old_st == STILL_R )
            p->state = STILL_R;
        if (old_st == JUMPING_L || old_st == WALKING_L || old_st == STILL_L )
            p->state = STILL_L;
    }
	//pulando 
    if (p->control->up && (p->ground || on_platform)) {
        
        if (old_st == WALKING_R || old_st == STILL_R )
            p->state = JUMPING_R ;
        if (old_st == WALKING_L || old_st == STILL_L )
            p->state = JUMPING_L;
    }
	//caindo
    if(!p->ground && !on_platform) {

        if (p->control->right) 
            p->state = JUMPING_R ;
        
        if (p->control->left) 
            p->state = JUMPING_L ;
    }
    //atualiza estado 
    if (p->state != old_st)
        player_update_state(p, old_st) ;

    // logica de movimento 
    if (p->control->right) {
        p->x += move_dist;
    } 
    else if (p->control->left) {
        p->x -= move_dist;
    }

    //borda esquerda
    if (p->x < p->w/2) {
        p->x = p->w/2;
    }

    //CONDICAO DE VITORIA
	if (p->x + p->w/2 > X_SCREEN) {
		p->x = X_SCREEN - p->w/2;  // Encosta na borda direita
        p->win = true ;
    }
    if (p->y - p->h/2 < 0) {
        p->y = p->h/2;  // Encosta no topo
    }

    //Logica de pulo 
    if (p->control->up && (p->ground || on_platform)) {

        p->fall = PLAYER_JUMP;
        p->ground = false;
        on_platform = false;
    }

    //Logica de queda
    if (!p->ground && !on_platform) {

        p->fall += GRAVITY;
        p->y += p->fall;

        if (p->y >= Y_FLOOR -  p->h/2) {

            p->y = Y_FLOOR - p->h/2;
            p->fall = 0;
            p->ground = true;
        }
    }
    else if (on_platform) p->fall = 0 ;

}

void player_draw_health(player *p) {
    if (!p) return;
    
    int start_x = HEALTH_BAR_X;
    int start_y = HEALTH_BAR_Y;
    
    for (int i = 0; i < p->max_health; i++) {
        int x = start_x + i * (HEALTH_BAR_W + HEALTH_BAR_SPACE);
        int y = start_y;
        
        ALLEGRO_COLOR fill_color, border_color;
        
        if (i < p->health) {
            fill_color = al_map_rgb(255, 0, 0);    // Vermelho - vida cheia
            border_color = al_map_rgb(200, 0, 0);  // Vermelho escuro
        } else {
            fill_color = al_map_rgb(60, 60, 60);   // Cinza - vida vazia
            border_color = al_map_rgb(100, 100, 100); // Cinza escuro
        }
        
        // Desenha um "coração" simplificado (losango)
        al_draw_filled_rectangle(x, y, x + HEALTH_BAR_W, y + HEALTH_BAR_H, fill_color);
        al_draw_rectangle(x, y, x + HEALTH_BAR_W, y + HEALTH_BAR_H, border_color, 2);
    }
}

void draw_player(player *p) {
    if(!p) return ;

    if (p->state != CROUCHING) {

        if(p->sprites[p->state]) {

            al_draw_scaled_bitmap(p->sprites[p->state],
                                0, 0, 
                                al_get_bitmap_width(p->sprites[p->state]),
                                al_get_bitmap_height(p->sprites[p->state]),
                                p->x - p->visual_w/2,
                                p->y - p->visual_h/2,
                                p->visual_w,
                                p->visual_h,
                                0);

            // Desenha a hitbox (vermelha) e área visual (azul)
            al_draw_rectangle(p->x - p->w/2, p->y - p->h/2,
                            p->x + p->w/2, p->y + p->h/2,
                            al_map_rgb(255, 0, 0), 3);  // Hitbox vermelha

            al_draw_rectangle(p->x - p->visual_w/2, p->y - p->visual_h/2,
                            p->x + p->visual_w/2, p->y + p->visual_h/2,
                            al_map_rgb(0, 0, 255), 1);  // Área visual azul
        }
        else 
            al_draw_filled_rectangle(p->x - p->w/2, p->y - p->h/2,
                                    p->x + p->w/2, p->y + p->h/2,
                                    al_map_rgb(255, 0, 0));

    }

    if (p->state == CROUCHING) {

        if(p->sprites[p->state]) {

            al_draw_scaled_bitmap(p->sprites[p->state],
                                0, 0, 
                                al_get_bitmap_width(p->sprites[p->state]),
                                al_get_bitmap_height(p->sprites[p->state]),
                                p->x - p->visual_w/2,
                                p->y - p->visual_h/2,
                                p->visual_w,
                                p->visual_h,
                                0);

            // Desenha a hitbox (vermelha) e área visual (azul)
            al_draw_rectangle(p->x - p->w/2, p->y - p->h/2,
                            p->x + p->w/2, p->y + p->h/2,
                            al_map_rgb(255, 0, 0), 3);  // Hitbox vermelha

            al_draw_rectangle(p->x - p->visual_w/2, p->y - p->visual_h/2,
                            p->x + p->visual_w/2, p->y + p->visual_h/2,
                            al_map_rgb(0, 0, 255), 1);  // Área visual azul
        }
        else 
            al_draw_filled_rectangle(p->x - p->w/2, p->y - p->h/2,
                                    p->x + p->w/2, p->y + p->h/2,
                                    al_map_rgb(0, 0, 255));

    }

}

void player_update_state(player *p, player_state old_st) {
    if(!p) return ;

    if (p->state != CROUCHING)
    {
        p->w = PLAYER_W ;
        p->h = PLAYER_H ;

        if(old_st == CROUCHING) {
            p->y = p->y -(PLAYER_H/2 -PLAYER_H_CROUCHED/2) ;
        }
    }

    if (p->state == CROUCHING && old_st != CROUCHING)
    {
        p->w = PLAYER_W_CROUCHED ;
        p->h = PLAYER_H_CROUCHED ;
        p->y = p->y + (PLAYER_H/2 -PLAYER_H_CROUCHED/2) ;
    }
}

//trecho copiado da funcao player_draw para pegar o scrool
void set_player_scroll(ALLEGRO_BITMAP *bg, player *p) {
    
    if (!p) return ;

    if (bg) {
        int img_w = al_get_bitmap_width(bg);
        int img_h = al_get_bitmap_height(bg);
        int disp_h = al_get_display_height(al_get_current_display());
        
        // Escala para preencher a altura da tela
        float scale = (float)disp_h / img_h;
        float draw_w = img_w * scale;
        
        // Calcula a posição de scroll baseada na posição do player
        p->scroll_x = fmod(p->x * SCROLL_SPEED, draw_w) ;
    }
}