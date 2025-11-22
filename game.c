#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>	
#include <allegro5/allegro_image.h>	

#include "game.h"

// Checagem AABB simples e correta
int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {

    if ((x1 + w1) < x2)  // 1 está a esquerda 2 
        return 0; 
    if ((x2 + w2) < x1)  // 1  está a direita do 2
        return 0; 
    if ((y1 + h1) < y2)  // 1 está acima de 2
        return 0 ;
    if ((y2 + h2) < y1)  // 1 está abaixo de 2
        return 0 ;
    // colide
    return 1;
}

void draw_menu(ALLEGRO_BITMAP* bg, ALLEGRO_FONT *font, int selected_opt) {

    // Obtém dimensões da tela
    int screen_w = al_get_display_width(al_get_current_display());
    int screen_h = al_get_display_height(al_get_current_display());

    // Desenhar a imagem de fundo proporcional e centralizada
    if (bg) {
        int img_w = al_get_bitmap_width(bg);
        int img_h = al_get_bitmap_height(bg);

        // Calcula a escala para preencher a tela mantendo a proporção
        float scale_x = (float)screen_w / img_w;
        float scale_y = (float)screen_h / img_h;
        float scale = (scale_x > scale_y) ? scale_x : scale_y; // Preenche toda a tela
        
        float draw_w = img_w * scale;
        float draw_h = img_h * scale;
        
        // Centraliza a imagem na tela
        float draw_x = (screen_w - draw_w) / 2;
        float draw_y = (screen_h - draw_h) / 2;
        
        al_draw_scaled_bitmap(bg, 0, 0, img_w, img_h, 
                              draw_x, draw_y, draw_w, draw_h, 0);
        
    } else {
        // Fallback: fundo preto
        al_clear_to_color(al_map_rgb(0, 0, 0));
    }

    // Cores para os textos
    ALLEGRO_COLOR color_unselected = al_map_rgb(255, 255, 255); // Branco
    ALLEGRO_COLOR color_selected = al_map_rgb(255, 215, 0);     // Dourado

    // Desenhar o título do jogo
    al_draw_text(font,
                 color_unselected,
                 screen_w / 2,
                 screen_h / 4,
                 ALLEGRO_ALIGN_CENTER,
                 "JOGO DO ALEXANDRE");

    // Opção "Iniciar Jogo"
    al_draw_text(font,
                 (selected_opt == 1) ? color_selected : color_unselected,
                 screen_w / 2,
                 screen_h / 2,
                 ALLEGRO_ALIGN_CENTER,
                 "Iniciar Jogo");

    // Opção "Sair do Jogo"
    al_draw_text(font,
                 (selected_opt == 2) ? color_selected : color_unselected,
                 screen_w / 2,
                 screen_h / 2 + 50,
                 ALLEGRO_ALIGN_CENTER,
                 "Sair do Jogo");

}

void draw_gameplay(ALLEGRO_BITMAP *bg, player *p, square *floor) {
    
    if (bg) {
        int img_w = al_get_bitmap_width(bg);
        int img_h = al_get_bitmap_height(bg);
        int disp_w = al_get_display_width(al_get_current_display());
        int disp_h = al_get_display_height(al_get_current_display());
        
        // Escala para preencher a altura da tela
        float scale = (float)disp_h / img_h;
        float draw_w = img_w * scale;
        float draw_h = img_h * scale;
        
        // Calcula quantas cópias precisamos para cobrir a tela + margem
        int copies_needed = (disp_w / draw_w) + 2;
        
        // Calcula a posição de scroll baseada na posição do player
        float scroll_x = fmod(p->x * 0.3f, draw_w);
        
        // Garante que o scroll seja positivo
        if (scroll_x < 0) scroll_x += draw_w;
        
        // Desenha múltiplas cópias para criar o efeito infinito
        for (int i = 0; i < copies_needed; i++) {
            al_draw_scaled_bitmap(bg, 0, 0, img_w, img_h, 
                                 -scroll_x + (i * draw_w), 0, draw_w, draw_h, 0);
        }
    }
    player_draw_health(p) ;

    // Desenha o player 
    al_draw_filled_rectangle(p->x - p->w/2, p->y - p->h/2,
                            p->x + p->w/2, p->y + p->h/2,
                            al_map_rgb(255, 0, 0));
    /*al_draw_filled_rectangle(floor->x - floor->w/2, floor->y - floor->h/2,
                            floor->x + floor->w/2, floor->y + floor->h/2, 
                            al_map_rgb(0, 255, 0));*/
}

int wasd_keys(player *p1, ALLEGRO_EVENT *ev){
    
    if(!p1 || !p1->control)
        return -1 ;

    ALLEGRO_EVENT event = *ev;

    if (event.keyboard.keycode == 1) 
        joystick_left(p1->control);
    else if (event.keyboard.keycode == 4) 
        joystick_right(p1->control);	
    else if (event.keyboard.keycode == 23) 
        joystick_up(p1->control);
    else if (event.keyboard.keycode == 19)
        joystick_down(p1->control);	

    return 0 ; 
}

void draw_gameover(ALLEGRO_BITMAP* bg, ALLEGRO_FONT *font, int selected_opt) {

    // Obtém dimensões da tela
    int screen_w = al_get_display_width(al_get_current_display());
    int screen_h = al_get_display_height(al_get_current_display());

    // Desenhar a imagem de fundo proporcional e centralizada
    if (bg) {
        int img_w = al_get_bitmap_width(bg);
        int img_h = al_get_bitmap_height(bg);

        // Calcula a escala para preencher a tela mantendo a proporção
        float scale_x = (float)screen_w / img_w;
        float scale_y = (float)screen_h / img_h;
        float scale = (scale_x > scale_y) ? scale_x : scale_y; // Preenche toda a tela
        
        float draw_w = img_w * scale;
        float draw_h = img_h * scale;
        
        // Centraliza a imagem na tela
        float draw_x = (screen_w - draw_w) / 2;
        float draw_y = (screen_h - draw_h) / 2;
        
        al_draw_scaled_bitmap(bg, 0, 0, img_w, img_h, 
                              draw_x, draw_y, draw_w, draw_h, 0);
        
    } else {
        // Fallback: fundo preto
        al_clear_to_color(al_map_rgb(0, 0, 0));
    }

    // Cores para os textos
    ALLEGRO_COLOR color_unselected = al_map_rgb(255, 255, 255); // Branco
    ALLEGRO_COLOR color_selected = al_map_rgb(255, 215, 0);     // Dourado

    // Desenhar o título do jogo
    al_draw_text(font,
                 color_unselected,
                 screen_w / 2,
                 screen_h / 4,
                 ALLEGRO_ALIGN_CENTER,
                 "GAME OVER");

    // Opção "Iniciar Jogo"
    al_draw_text(font,
                 (selected_opt == 1) ? color_selected : color_unselected,
                 screen_w / 2,
                 screen_h / 2,
                 ALLEGRO_ALIGN_CENTER,
                 "Reiniciar Jogo");

    // Opção "Sair do Jogo"
    al_draw_text(font,
                 (selected_opt == 2) ? color_selected : color_unselected,
                 screen_w / 2,
                 screen_h / 2 + 50,
                 ALLEGRO_ALIGN_CENTER,
                 "Sair do Jogo");

}

//NECESSITA DE PONTEIROS NULOS
int game_set(player **p, square **floor, obstacle_manager **obs_manager) {

	if(!p || !floor || !obs_manager) {
        fprintf(stderr, "ERRO GAME SET\n") ;
		return -1 ;
    }

	int floor_center_y = Y_SCREEN - H_GROUND / 2 ;
	int player_start_y = Y_FLOOR - PLAYER_H / 2 ; 

	*p = player_create(PLAYER_W, PLAYER_H, PLAYER_START_X, player_start_y, X_SCREEN, Y_SCREEN);
	*floor = square_create(X_SCREEN, H_GROUND, X_SCREEN/2, floor_center_y, X_SCREEN, Y_SCREEN);
    *obs_manager = obstacle_manager_create(5, 2.0f, 1.0f) ;

	if(!*p || !*floor || !*obs_manager) {
        fprintf(stderr, "ERRO GAME SET, NAO ALOCOU\n") ;
		return -1 ;
    }
    return 0 ;
}

int game_clean(player *p, square *floor, obstacle_manager *obs_manager) {

    if(!p || !floor || !obs_manager) {
        fprintf(stderr, "ERRO GAME CLEAN\n") ;
        return -1 ;
    }

    free(p) ;
    free(floor) ;
    free(obs_manager) ;

    return 0 ;
}
