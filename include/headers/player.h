#ifndef PLAYER_H
#define PLAYER_H

struct player_object {
    int x;
    int y;
    int width;
    int height;
    int speed;
};

void draw_player(SDL_Renderer *renderer, struct player_object player);

void move_player_left(struct player_object* player);
void move_player_right(struct player_object* player);

#endif
