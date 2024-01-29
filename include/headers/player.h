#ifndef PLAYER_H
#define PLAYER_H

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

void draw_player(SDL_Renderer *renderer, struct player_object player);

void move_player_left(struct player_object* player);
void move_player_right(struct player_object* player);

#endif
