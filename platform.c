#include "platform.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

platform* platform_create(int x, int y, int w, int h) {
    platform* plat = malloc(sizeof(platform));
    if (!plat) return NULL;
    
    plat->x = x;
    plat->y = y;
    plat->w = w;
    plat->h = h;
    plat->active = true;
    plat->sprite = NULL;
    
    return plat;
}

void platform_destroy(platform* plat) {
    if (plat) {
        if (plat->sprite) {
            al_destroy_bitmap(plat->sprite);
        }
        free(plat);
    }
}

void platform_reset(platform* plat, int screen_width) {
    if (!plat) return;
    
    // Reposiciona a plataforma à direita da tela
    plat->x = screen_width + (rand() % 300) + 100;
    plat->y = (rand() % 300) +300 ; // Posição Y aleatória
    plat->active = false;
}

bool platform_check_collision(platform* plat, player* p) {
    if (!plat || !plat->active || !p) return false;
    
    bool collision_x = (p->x + p->w/2 >= plat->x) && 
                      (p->x - p->w/2 <= plat->x + plat->w);
    
    bool collision_y = (p->y + p->h/2 >= plat->y) && 
                      (p->y - p->h/2 <= plat->y + plat->h);
    
    return collision_x && collision_y;
}

/*ARRUMAR*/
void platform_handle_collision(platform* plat, player* p) {
    if (!plat || !p || !plat->active) return;
    
    // Verifica se o player está caindo e está acima da plataforma
    if (p->fall >= 0 && 
        p->y + p->h/2 >= plat->y && 
        p->y + p->h/2 <= plat->y + 10 && // Margem pequena para detectar "pouso"
        p->x + p->w/2 >= plat->x && 
        p->x - p->w/2 <= plat->x + plat->w) {
        
        // Coloca o player em cima da plataforma
        p->y = plat->y - p->h/2;
        p->fall = 0;
        //p->ground = true;
    }
}

void draw_platform(platform* plat) {
    if (!plat || !plat->active) return;

    if (plat->sprite) {
        al_draw_scaled_bitmap(plat->sprite,
                             0, 0, 
                             al_get_bitmap_width(plat->sprite),
                             al_get_bitmap_height(plat->sprite),
                             plat->x,
                             plat->y,
                             plat->w,
                             plat->h,
                             0);
    } else {
        // Desenha plataforma como retângulo
        al_draw_filled_rectangle(plat->x, plat->y,
                               plat->x + plat->w, plat->y + plat->h,
                               al_map_rgb(255, 0, 0));
        
    }
}

// PLATFORM MANAGER 

platform_manager* platform_manager_create(int max_platforms, float spawn_interval) {

    platform_manager* manager = malloc(sizeof(platform_manager));
    if (!manager) return NULL;
    
    manager->platforms = malloc(sizeof(platform*) * max_platforms);
    manager->count = 0;
    manager->max_platforms = max_platforms;
    manager->spawn_timer = 0;
    manager->spawn_interval = spawn_interval;
    
    // Inicializa plataformas como NULL
    for (int i = 0; i < max_platforms; i++) {
        manager->platforms[i] = NULL;
    }

    srand(time(NULL));
    return manager;
}

void platform_manager_update(platform_manager* manager, float delta_time, int screen_width) {
    if (!manager) return;
    
    manager->spawn_timer += delta_time;
    
    // Spawn de novas plataformas
    if (manager->spawn_timer >= manager->spawn_interval) {
        for (int i = 0; i < manager->max_platforms; i++) {
            if (!manager->platforms[i] || !manager->platforms[i]->active) {
                // Cria nova plataforma se slot vazio ou inativo
                if (manager->platforms[i]) {
                    platform_destroy(manager->platforms[i]);
                }
                
                // Aleatoriza tamanho e cor da plataforma
                int width = (rand() % 100) + 80;  // Largura entre 80-180
                int height = 30; // Altura fixa
                int spawn_x = screen_width + (rand() % 100) ;
                int spawn_y = (rand() % 300) + 300; // Posição Y aleatória
                                
                manager->platforms[i] = platform_create(spawn_x, spawn_y, width, height);
                manager->spawn_timer = 0;
                break;
            }
        }
    }
    
    // Atualiza plataformas ativas
    for (int i = 0; i < manager->max_platforms; i++) {
        if (manager->platforms[i] && manager->platforms[i]->active) {
            // Move plataformas para a esquerda (scroll)
            manager->platforms[i]->x -= 3; // Velocidade de scroll
            
            // Reset quando sair da tela pela esquerda
            if (manager->platforms[i]->x + manager->platforms[i]->w < 0) {
                platform_reset(manager->platforms[i], screen_width);
            }
        }
    }
}

void platform_manager_draw(platform_manager* manager) {
    if (!manager) return;
    
    for (int i = 0; i < manager->max_platforms; i++) {
        if (manager->platforms[i] && manager->platforms[i]->active) {
            draw_platform(manager->platforms[i]);
        }
    }
}

void platform_manager_destroy(platform_manager* manager) {
    if (!manager) return;
    
    for (int i = 0; i < manager->max_platforms; i++) {
        if (manager->platforms[i]) {
            platform_destroy(manager->platforms[i]);
        }
    }
    free(manager->platforms);
    free(manager);
}

void platform_manager_reset_all(platform_manager* manager, int screen_width) {
    if (!manager) return;
    
    for (int i = 0; i < manager->max_platforms; i++) {
        if (manager->platforms[i]) {
            platform_reset(manager->platforms[i], screen_width);
        }
    }
    manager->spawn_timer = 0;
}
