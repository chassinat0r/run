#include <SDL.h>
#include <stdbool.h>

int get_key() {
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
