#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "./constants.h"

struct player_object {
    float x;
    int y;
    int width;
    int height;
    float speed;
    float max_speed;
    float acceleration;
    float friction;
    bool left;
    bool right;
};

void draw_player(SDL_Renderer *renderer, struct player_object player) {
    SDL_Rect player_rect = {
        (int)player.x - (player.width / 2),
        (int)player.y - (player.height / 2),
        (int)player.width,
        (int)player.height
    };
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &player_rect);
}

void move_player_left(struct player_object* player) {
    player->speed -= player->acceleration;
    // if (player->x < BORDER_LEFT + (player->width / 2)) {
    //     player->x = BORDER_LEFT + (player->width / 2);
    // }
}

void move_player_right(struct player_object* player) {
    player->speed += player->acceleration;
    // if (player-> x > BORDER_RIGHT - (player->width / 2)) {
    //     player->x = BORDER_RIGHT - (player->width / 2);
    // }
}

