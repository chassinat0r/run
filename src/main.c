#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#include <SDL.h>

#include "./constants.h"
#include "./display.h"
#include "./input.h"
#include "./player.h"

bool running = false;
float last_frame_time = 0;

struct player_object player;

struct wall {
    int start_x;
    int end_x;
    float y;
};

struct wall walls[MAX_WALLS];

int get_num_of_walls() {
    for (int i = 0; i < MAX_WALLS; i++) {
        if (!(walls[i].start_x || walls[i].end_x || walls[i].y)) {
            return i;
        }
    }
    return MAX_WALLS;
}

void add_wall(int start_x, int end_x, float y) {
    int index = get_num_of_walls();

    walls[index].start_x = start_x;
    walls[index].end_x = end_x;
    walls[index].y = y;
}

void add_row(float y) {
    int wall_length_left = rand() % (WALL_COLUMNS - 1);
    int wall_length_right = WALL_COLUMNS - wall_length_left - 2;
    
    add_wall(0, wall_length_left, y);
    add_wall(WALL_COLUMNS - wall_length_right, WALL_COLUMNS, y);
}

void delete_wall(int index) {
    int number_of_walls = get_num_of_walls();

    for (int i = index; i < number_of_walls; i++) {
        walls[i-1] = walls[i];
    }

    walls[number_of_walls-1].start_x = '\0';
    walls[number_of_walls-1].end_x = '\0';
    walls[number_of_walls-1].y = '\0';
}

void move_wall_down(float change) {
    int number_indexes_deleted = 0;

    for (int i = 0; i < get_num_of_walls(); i++) {
        int index = i - number_indexes_deleted;
        walls[index].y -= change;
        if (walls[index].y < -1) {
            delete_wall(index);
            number_indexes_deleted++;
        }
    }

    if (WALL_ROWS - walls[get_num_of_walls() - 1].y >= 6) {
        add_row(WALL_ROWS + 1);
    }
}

void init_walls_array() {
    for (int i = 0; i < MAX_WALLS; i++) {
        walls[i].start_x = '\0';
        walls[i].end_x = '\0';
        walls[i].y = '\0';
    }

    for (int i = 12; i < WALL_ROWS; i += 7) {
        add_row(i);
    }
}

void draw_wall(SDL_Renderer *renderer, struct wall w) {
    for (int x = w.start_x; x < w.end_x; x++) {
        SDL_Rect block_rect = {
            (float)(x * WALL_BLOCK_SIZE) + BORDER_LEFT,
            (float)(WALL_ROWS - w.y) * WALL_BLOCK_SIZE,
            (int)WALL_BLOCK_SIZE,
            (int)WALL_BLOCK_SIZE
        };

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &block_rect);
    }
}

void update(SDL_Renderer *renderer) {
    float time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);
    
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
        SDL_Delay(time_to_wait);
    }

    last_frame_time = SDL_GetTicks();

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(renderer, BORDER_LEFT, 0, BORDER_LEFT, WINDOW_HEIGHT);
    SDL_RenderDrawLine(renderer, BORDER_RIGHT, 0, BORDER_RIGHT, WINDOW_HEIGHT);

    draw_player(renderer, player);

    for (int i = 0; i < get_num_of_walls(); i++) {
        draw_wall(renderer, walls[i]);
    }

    SDL_RenderPresent(renderer);

    move_wall_down((float)7 / FRAME_TARGET_TIME);

    if (player.left) {
        player.speed -= player.acceleration;
        if (player.speed < -player.max_speed * 0.5) {
            player.speed -= player.acceleration;
        }
    }
    if (player.right) {
        player.speed += player.acceleration;
        if (player.speed > player.max_speed * 0.5) {
            player.speed += player.acceleration;
        }
    }

    if (player.speed > player.max_speed) {
        player.speed = player.max_speed;
    }
    if (player.speed < -player.max_speed) {
        player.speed = -player.max_speed;
    }

    if (player.speed > 0) {
        player.speed -= player.friction;
    }
    if (player.speed < 0) {
        player.speed += player.friction;
    }

    if (fabs(player.speed) < player.friction) {
        player.speed = 0;
    }

    player.x += player.speed;

    if (player.x < BORDER_LEFT + (player.width / 2)) {
        player.x = BORDER_LEFT + (player.width / 2);
    }
    if (player.x > BORDER_RIGHT - (player.width / 2)) {
        player.x = BORDER_RIGHT - (player.width / 2);
    }
}

int main(int argc, char *argv[]) {
    SDL_Window *window = create_window("Walls");
    if (!window) {
        printf("Error occurred creating SDL window.\n");
        return;
    }
    
    SDL_Renderer *renderer = get_renderer(window);

    if (!renderer) {
        printf("Error loading SDL renderer.\n");
        return;
    }

    init_walls_array();

    running = true;

    player.x = WINDOW_WIDTH / 2;
    player.y = WINDOW_HEIGHT - 50;
    player.width = PLAYER_WIDTH;
    player.height = PLAYER_HEIGHT;
    player.speed = 0;
    player.max_speed = 8;
    player.acceleration = 0.4;
    player.friction = 0.05;

    while (running) {
        update(renderer);

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            running = false;
                            break; 
                        case SDLK_LEFT:
                        case SDLK_a:
                            if (player.speed > 0) {
                                player.speed = -player.friction;
                            }
                            player.left = true;
                            break;
                        case SDLK_RIGHT:
                        case SDLK_d:
                            if (player.speed < 0) {
                                player.speed = player.friction;
                            }
                            player.right = true;
                            break; 
                    }
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.sym) {
                        case SDLK_LEFT:
                        case SDLK_a:
                            player.left = false;
                            break;
                        case SDLK_RIGHT:
                        case SDLK_d:
                            player.right = false;
                            break;
                    }
                    break;
            }
        }
    }
}
