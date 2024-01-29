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

float grid[GRID_MAX_ITEMS][2];

int get_grid_length() {
    for (int i = 0; i < GRID_MAX_ITEMS; i++) {
        if (!(grid[i][0] || grid[i][1])) {
            return i;
        }
    }
    return GRID_MAX_ITEMS;
}

void add_row(float y) {
    int wall_columns_left = rand() % (GRID_COLUMNS - 1);
    int wall_columns_right = GRID_COLUMNS - wall_columns_left - 1;

    for (int x = 0; x < wall_columns_left; x++) {
        int index = get_grid_length();

        grid[index][0] = x;
        grid[index][1] = y;
    }

    for (int x = GRID_COLUMNS - 1; x > GRID_COLUMNS - wall_columns_right; x--) {
        int index = get_grid_length();

        grid[index][0] = x;
        grid[index][1] = y;
    }
}

void delete_grid_space(int index) {
    int length = get_grid_length();
    for (int i = index; i < length; i++) {
        grid[i-1][0] = grid[i][0];
        grid[i-1][1] = grid[i][1];
    }

    grid[length-1][0] = '\0';
    grid[length-1][1] = '\0';
}

void move_grid_down(float change) {
    int number_indexes_deleted = 0;

    for (int i = 0; i < get_grid_length(); i++) {
        int index = i - number_indexes_deleted;
        grid[index][1] -= change;
        if (grid[index][1] < -1) {
            delete_grid_space(index);
            number_indexes_deleted++;
        }
    }

    if (GRID_ROWS - grid[get_grid_length() - 1][1] >= 6) {
        add_row(GRID_ROWS + 1);
    }

}

void init_grid() {
    for (int i = 0; i < GRID_MAX_ITEMS; i++) {
        grid[i][0] = '\0';
        grid[i][1] = '\0';
    }

    for (int i = 12; i < GRID_ROWS; i += 7) {
        add_row(i);
    }
}

void draw_block(SDL_Renderer *renderer, float x, float y) {
    SDL_Rect block_rect = {
        (float)(x * GRID_SPACE_SIZE) + BORDER_LEFT,
        (float)(GRID_ROWS - y) * GRID_SPACE_SIZE,
        (int)GRID_SPACE_SIZE,
        (int)GRID_SPACE_SIZE
    };
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &block_rect);
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

    for (int i = 0; i < get_grid_length(); i++) {
        draw_block(renderer, grid[i][0], grid[i][1]);
    }

    SDL_RenderPresent(renderer);

    move_grid_down((float)7 / FRAME_TARGET_TIME);

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
    SDL_Window *window = create_window("RUN");
    if (!window) {
        printf("Error occurred creating SDL window.\n");
        return;
    }
    
    SDL_Renderer *renderer = get_renderer(window);

    if (!renderer) {
        printf("Error loading SDL renderer.\n");
        return;
    }

    init_grid();

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
