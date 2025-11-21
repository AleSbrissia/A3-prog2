#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>	

#include "player.h"
#include "fase.h"
#include "game.h"

#define X_SCREEN 1280											
#define Y_SCREEN 720

// Checagem AABB simples e correta
int collision_2D(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {

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

//nao conta colisao
void update_position(player *player_1){
    if (!player_1) return;

    /* cópias temporárias para calcular posições pretendidas usando square_move */
    player temp1 = *player_1;

    /* aplicar movimentos pretendidos nas cópias (preserva validações de square_move) */
    if (player_1->control){
        if (player_1->control->left)  player_move(&temp1, 1, 0, X_SCREEN, Y_SCREEN);
        if (player_1->control->right) player_move(&temp1, 1, 1, X_SCREEN, Y_SCREEN);
        if (player_1->control->up)    player_move(&temp1, 1, 2, X_SCREEN, Y_SCREEN);
        if (player_1->control->down)  player_move(&temp1, 1, 3, X_SCREEN, Y_SCREEN);
    }
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

void draw_gameplay(player *p, square *floor) {
    update_position(p);
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_filled_rectangle(p->x -p->w/2, p->y -p->h/2,
                            p->x +p->w/2, p->y +p->h/2,
                            al_map_rgb(255, 0, 0));
    al_draw_filled_rectangle(floor->x -floor->w/2, floor->y -floor->h/2,
                            floor->x +floor->w/2, floor->y +floor->h/2, 
                            al_map_rgb(0, 255, 0));
    al_flip_display();
}
