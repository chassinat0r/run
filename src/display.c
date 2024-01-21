#include <SDL.h>
#include <stdbool.h>

#include "./constants.h"

SDL_Window *create_window(char title[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        return false;
    }

    return SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0
    );
}

SDL_Renderer *get_renderer(SDL_Window *window) {
    return SDL_CreateRenderer(window, -1, 0);
}

