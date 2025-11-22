#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>	

#include "player.h"
#include "fase.h"
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

void draw_menu(ALLEGRO_BITMAP* bg_image, ALLEGRO_FONT *font, int selected_opt) {

    // Obtém dimensões da tela
    int screen_w = al_get_display_width(al_get_current_display());
    int screen_h = al_get_display_height(al_get_current_display());

    // Desenhar a imagem de fundo proporcional e centralizada
        if (bg_image) {
        // draw_scaled_background(bg_image, 0);
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
    al_clear_to_color(al_map_rgb(0, 0, 0));
    
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

    // Desenha o player e o chão (sem mudanças)
    al_draw_filled_rectangle(p->x - p->w/2, p->y - p->h/2,
                            p->x + p->w/2, p->y + p->h/2,
                            al_map_rgb(255, 0, 0));
    /*al_draw_filled_rectangle(floor->x - floor->w/2, floor->y - floor->h/2,
                            floor->x + floor->w/2, floor->y + floor->h/2, 
                            al_map_rgb(0, 255, 0));*/
}