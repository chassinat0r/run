#include <SDL.h>
#include <stdbool.h>

int key_down() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                return SDLK_ESCAPE;
                break;
            case SDL_KEYDOWN:
                return event.key.keysym.sym;
                break;
        }
    }
    return false;
}

int key_up() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_KEYUP:
                return event.key.keysym.sym;
                break;
        }
    }
    return false;
}

