#include "obstacle.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

obstacle* obstacle_create(int x, int y, int w, int h, float speed_x, float speed_y, obstacle_type type, ALLEGRO_BITMAP *sprite) {

    obstacle* obs = malloc(sizeof(obstacle));
    if (!obs) return NULL;
    
    obs->w = w;
    obs->h = h;
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
        if (obs->sprite) {
            al_destroy_bitmap(obs->sprite);
        }
        free(obs);
    }
}

void obstacle_reset(obstacle* obs, int screen_width, int y_floor) {
    if (!obs) return;
    
    // Reposiciona o obstáculo à direita da tela com variação aleatória
    obs->x = screen_width + (rand() % 200) + 100;
    obs->y = y_floor - obs->h/2; // Posiciona no chão
    obs->active = true;
    
    // Variação na velocidade para tornar mais interessante
    obs->speed_x = -((rand() % 3) + 2); // Velocidade entre -2 e -4
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
        // Desenha sprite centralizado
        al_draw_bitmap(obs->sprite, obs->x - obs->w/2, obs->y - obs->h/2, 0);
    } else {
        // Fallback: desenha retângulo
        al_draw_filled_rectangle(obs->x - obs->w/2, obs->y - obs->h/2,
                               obs->x + obs->w/2, obs->y + obs->h/2,
                               obs->color);
        
        // Borda para melhor visualização
        /*al_draw_rectangle(obs->x - obs->w/2, obs->y - obs->h/2,
                         obs->x + obs->w/2, obs->y + obs->h/2,
                         al_map_rgb(255, 255, 255), 1);*/
    }

    return 0;
}

// ========== SISTEMA DE GERENCIAMENTO DE MÚLTIPLOS OBSTÁCULOS ==========

obstacle_manager* obstacle_manager_create(int max_obs, float spawn_interval, float scroll_speed) {
    obstacle_manager* manager = malloc(sizeof(obstacle_manager));
    if (!manager) return NULL;
    
    manager->obstacles = malloc(sizeof(obstacle*) * max_obs);
    
    // CORREÇÃO: Aloca e carrega os sprites DIRETAMENTE
    manager->obstacles_sprites = malloc(sizeof(ALLEGRO_BITMAP*) * DIFFERENT_OBSTACLES);
    
    // Carrega cada sprite
    manager->obstacles_sprites[stem] = al_load_bitmap("assets/sprites/stem.png");
    manager->obstacles_sprites[arrow] = al_load_bitmap("assets/sprites/arrow.png"); 
    manager->obstacles_sprites[stone] = al_load_bitmap("assets/sprites/stone.png");
    
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
    int width, height, i, aleat;
                
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
                //aleat = rand() % DIFFERENT_OBSTACLES;
                //obs_type = aleat ;

                obs_type = arrow ;
                printf("TIPO %d\n", obs_type) ;
                
                switch (obs_type) {
                    case stem: 

                        width = 20; height = 30; 
                        sprite = manager->obstacles_sprites[stem] ;
                        printf("Criando STEM - sprite: %p\n", sprite);

                    break; 

                    case arrow: 
                    
                        width = 30; height = 40; 
                        sprite = manager->obstacles_sprites[arrow] ;
                        printf("Criando ARROW - sprite: %p\n", sprite);
                    
                    break; 

                    case stone: 
                    
                        width = 30; height = 40; 
                        sprite = manager->obstacles_sprites[stone] ;
                        printf("Criando STONE - sprite: %p\n", sprite);
                    
                    break; 

                    case spike: 
                    
                        width = 30; height = 40; 
                        sprite = NULL ;
                    
                    break; 
                }
                
                manager->obstacles[i] = obstacle_create(
                    screen_width + 100, 
                    y_floor - height/2, 
                    width, height, 
                    -((rand() % 3) + 2), // Velocidade aleatória
                    0, 
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

                // Aqui você pode implementar lógica de game over ou dano
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
