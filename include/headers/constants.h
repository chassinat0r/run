#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <math.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define FPS 30
#define FRAME_TARGET_TIME (1000 / FPS)

#define WALL_COLUMNS 10
#define WALL_BLOCK_SIZE 30
#define MAX_WALLS 64

#define PLAYER_WIDTH 0.75 * WALL_BLOCK_SIZE
#define PLAYER_HEIGHT PLAYER_WIDTH

#define BORDER_LEFT (WINDOW_WIDTH / 2) - (0.5 * WALL_COLUMNS * WALL_BLOCK_SIZE)
#define BORDER_RIGHT (WINDOW_WIDTH / 2) + (0.5 * WALL_COLUMNS * WALL_BLOCK_SIZE)

#define WALL_ROWS (WINDOW_HEIGHT / WALL_BLOCK_SIZE)

#endif
