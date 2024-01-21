#include <stdio.h>
#include <stdbool.h>

#include <SDL.h>

#include "./constants.h"
#include "./display.h"
#include "./input.h"
#include "./player.h"

bool running = false;

struct player_object player;

void update(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(renderer, BORDER_LEFT, 0, BORDER_LEFT, WINDOW_HEIGHT);
    SDL_RenderDrawLine(renderer, BORDER_RIGHT, 0, BORDER_RIGHT, WINDOW_HEIGHT);

    draw_player(renderer, player);

    SDL_RenderPresent(renderer);
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

    running = true;

    player.x = WINDOW_WIDTH / 2;
    player.y = WINDOW_HEIGHT - 50;
    player.width = PLAYER_WIDTH;
    player.height = PLAYER_HEIGHT;
    player.speed = 5;

    while (running) {
        update(renderer);

        int key = get_key();
        switch (key) {
            case SDLK_ESCAPE:
                running = false;
                break; 
            case SDLK_LEFT:
            case SDLK_a:
                move_player_left(&player);
                break;
            case SDLK_RIGHT:
            case SDLK_d:
                move_player_right(&player);
                break;
        }
    }
}
