#include "obstacle.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

obstacle* obstacle_create(int x, int y, int w, int h, int v_w, int v_h, float speed_x, float speed_y, obstacle_type type, ALLEGRO_BITMAP *sprite) {

    obstacle* obs = malloc(sizeof(obstacle));
    if (!obs) return NULL;
    
    obs->w = w;
    obs->h = h;
    obs->visual_h = v_h;
    obs->visual_w = v_w;
    obs->x = x;
    obs->y = y;
    obs->speed_x = speed_x;
    obs->speed_y = speed_y;
    obs->active = true;
    obs->color = al_map_rgb(0, 255, 0); // Cor padrão verde 
    obs->type = type ;
    obs->sprite = sprite ;
    
    return obs;
}
void obstacle_destroy(obstacle* obs) {
    if (obs) {
        free(obs);
    }
}

void obstacle_reset(obstacle* obs, int screen_width, int y_floor) {
    if (!obs) return;
    
    // Reposiciona o obstáculo à direita da tela com variação aleatória
    obs->x = screen_width + (rand() % 200) + 100;

    obs->active = false;
    
}

int obstacle_update_movement(obstacle* obs, int screen_width, int y_floor, int gravity) {
    if (!obs || !obs->active) return 0;

    // Movimento horizontal com rolling background
    obs->x += obs->speed_x;

    // Reset quando sair da tela pela esquerda
    if (obs->x + obs->w/2 < 0) {
        obstacle_reset(obs, screen_width, y_floor);
        return 1; // Obstáculo resetado
    }

    // Física de gravidade se aplicável
    if (obs->speed_y != 0) {
        obs->speed_y += gravity;
        obs->y += obs->speed_y;
        
        // Colisão com o chão
        if (obs->y + obs->h/2 >= y_floor) {
            obs->y = y_floor - obs->h/2;
            obs->speed_y = 0;
            return 2; // Obstáculo no chão
        }
    }
    
    return 0; // Movimento normal
}

bool obstacle_check_collision(obstacle* obs, player* p) {
    if (!obs || !obs->active || !p) return false;
    
    // Colisão AABB (Axis-Aligned Bounding Box)
    bool collision_x = (p->x + p->w/2 >= obs->x - obs->w/2) && 
                      (p->x - p->w/2 <= obs->x + obs->w/2);
    
    bool collision_y = (p->y + p->h/2 >= obs->y - obs->h/2) && 
                      (p->y - p->h/2 <= obs->y + obs->h/2);
    
    return collision_x && collision_y;
}

int draw_obstacle(obstacle* obs) {
    if (!obs || !obs->active) return -1;

    if (obs->sprite) {
        // Obtém as dimensões reais do sprite
 
        al_draw_scaled_bitmap(obs->sprite,
                             0, 0, 
                             al_get_bitmap_width(obs->sprite),
                             al_get_bitmap_height(obs->sprite),
                             obs->x - obs->visual_w/2,
                             obs->y - obs->visual_h/2,
                             obs->visual_w,
                             obs->visual_h,
                             0);
        
        // Desenha a hitbox 
        al_draw_rectangle(obs->x - obs->w/2, obs->y - obs->h/2,
                         obs->x + obs->w/2, obs->y + obs->h/2,
                         al_map_rgb(255, 0, 0), 2);

        al_draw_rectangle(obs->x - obs->visual_w/2, obs->y - obs->visual_h/2,
                obs->x + obs->visual_w/2, obs->y + obs->visual_h/2,
                al_map_rgb(0, 0, 255), 1);  // Área visual azul
                         
    } else {
        // Fallback: desenha retângulo
        al_draw_filled_rectangle(obs->x - obs->w/2, obs->y - obs->h/2,
                               obs->x + obs->w/2, obs->y + obs->h/2,
                               obs->color);
    }

    return 0;
}

// ========== SISTEMA DE GERENCIAMENTO DE MÚLTIPLOS OBSTÁCULOS ==========

obstacle_manager* obstacle_manager_create(int max_obs, float spawn_interval, float scroll_speed) {
    obstacle_manager* manager = malloc(sizeof(obstacle_manager));
    if (!manager) return NULL;
    
    manager->obstacles = malloc(sizeof(obstacle*) * max_obs);
    
    manager->obstacles_sprites = malloc(sizeof(ALLEGRO_BITMAP*) * DIFFERENT_OBSTACLES);
    
    // Carrega cada sprite
    manager->obstacles_sprites[stem] = al_load_bitmap("assets/sprites/obstacles/stem.png");
    manager->obstacles_sprites[arrow] = al_load_bitmap("assets/sprites/obstacles/arrow.png"); 
    manager->obstacles_sprites[stone] = al_load_bitmap("assets/sprites/obstacles/stone.png");
    
    // Verifica se carregou
    for (int i = 0; i < DIFFERENT_OBSTACLES; i++) {
        if (!manager->obstacles_sprites[i]) {
            printf("ERRO: Não carregou sprite %d\n", i);
        }
    }
    
    manager->count = 0;
    manager->max_obstacles = max_obs;
    manager->spawn_timer = 0;
    manager->spawn_interval = spawn_interval;
    manager->scroll_speed = scroll_speed;
    
    // Inicializa obstáculos como NULL
    for (int i = 0; i < max_obs; i++) {
        manager->obstacles[i] = NULL;
    }
    
    srand(time(NULL));
    //srand(0) ;
    return manager;
}

void obstacle_manager_update(obstacle_manager* manager, float delta_time, player* player, int screen_width, int y_floor, int gravity) {

    obstacle_type obs_type ; 
    ALLEGRO_BITMAP *sprite = NULL ;
    int i, aleat, width, height, spawn_x, spawn_y;
    int visual_w, visual_h ;
    float speed_x, speed_y;
                
    if (!manager) return;
    
    manager->spawn_timer += delta_time;
    
    // Spawn de novos obstáculos
    if (manager->spawn_timer >= manager->spawn_interval) {
        for (i = 0; i < manager->max_obstacles; i++) {
            if (!manager->obstacles[i] || !manager->obstacles[i]->active) {
                // Cria novo obstáculo se slot vazio ou inativo
                if (manager->obstacles[i]) {
                    obstacle_destroy(manager->obstacles[i]);
                }
                
                // Aleatoriza tipo/tamanho do obstáculo
                aleat = rand() % DIFFERENT_OBSTACLES;
                obs_type = aleat ;

                //obs_type = arrow ;
                //obs_type = stone ;

                printf("TIPO %d\n", obs_type) ;
                
                switch (obs_type) {
                    case stem: 

                        visual_w = 107; visual_h = 85; 
                        width = 71; height = 64; 
                        sprite = manager->obstacles_sprites[stem] ;

                        spawn_x = screen_width +100 ;
                        spawn_y = y_floor -height/7; //spawna no chao

                        speed_x = (float) -(rand() % 9) -7 ;
                        speed_y = 0 ;

                        printf("Criando STEM - sprite: %p\n", sprite);

                    break; 

                    case arrow: 
                    
                        visual_w = 300, visual_h = 150; 
                        width = 47; height = 27; 
                        sprite = manager->obstacles_sprites[arrow] ;

                        spawn_x = screen_width +100 ;
                        spawn_y = y_floor -height/2 -(PLAYER_H/2) - (rand() % 100); //spawna no ar 

                        speed_x = (float) -(rand() % 10) -15 ;
                        speed_y = 0 ;

                        printf("Criando ARROW - sprite: %p\n", sprite);
                    
                    break; 

                    case stone: 
                    
                        visual_w = 50; visual_h = 50; 
                        width = 40; height = 40; 
                        sprite = manager->obstacles_sprites[stone] ;

                        spawn_x = screen_width +100 ;
                        spawn_y = y_floor -height -200 -(rand()%200) ; //spawna no ar

                        speed_x = (float) -(rand() % 10) -10 ;
                        speed_y =  0.5f;

                        printf("Criando STONE - sprite: %p\n", sprite);
                    
                    break; 

                    case spike: 
                    
                        width = 30; height = 40; 
                        sprite = NULL ;
                    
                    break; 
                }
                
                manager->obstacles[i] = obstacle_create(
                    spawn_x, spawn_y, 
                    width, height, 
                    visual_w, visual_h,
                    speed_x, speed_y, 
                    obs_type,
                    sprite
                );
                
                manager->spawn_timer = 0;
                break;
            }
        }
    }
    
    // Atualiza obstáculos ativos e verifica colisões
    for (int i = 0; i < manager->max_obstacles; i++) {
        if (manager->obstacles[i] && manager->obstacles[i]->active) {
            obstacle_update_movement(manager->obstacles[i], screen_width, y_floor, gravity);
            
            // Verifica colisão com player
            if (obstacle_check_collision(manager->obstacles[i], player)) {

                if (player->damage_dalay == 0) {

                    player->health-- ;
                    player->damage_dalay = PLAYER_DAMAGE_DALAY ;
                }
            }
        }
    }
}

void obstacle_manager_draw(obstacle_manager* manager) {
    if (!manager) return;
    
    for (int i = 0; i < manager->max_obstacles; i++) {
        if (manager->obstacles[i] && manager->obstacles[i]->active) {
            draw_obstacle(manager->obstacles[i]);
        }
    }
}

void obstacle_manager_destroy(obstacle_manager* manager) {
    if (!manager) return;
    
    for (int i = 0; i < manager->max_obstacles; i++) {
        if (manager->obstacles[i]) {
            obstacle_destroy(manager->obstacles[i]);
            manager->obstacles[i] = NULL ;
        }
    }
    free(manager->obstacles);

    if (manager->obstacles_sprites) {
        for (int i = 0; i < DIFFERENT_OBSTACLES; i++) {
            if (manager->obstacles_sprites[i]) {
                al_destroy_bitmap(manager->obstacles_sprites[i]);
            }
        }
        free(manager->obstacles_sprites);
    }

    free(manager);
}

void obstacle_manager_reset_all(obstacle_manager* manager, int screen_width, int y_floor) {
    if (!manager) return;
    
    for (int i = 0; i < manager->max_obstacles; i++) {
        if (manager->obstacles[i]) {
            obstacle_reset(manager->obstacles[i], screen_width, y_floor);
        }
    }
    manager->spawn_timer = 0;
}
