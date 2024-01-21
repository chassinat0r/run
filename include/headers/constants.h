#ifndef CONSTANTS_H
#define CONSTANTS_H

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define GRID_COLUMNS 10
#define GRID_SPACE_SIZE 30

#define PLAYER_WIDTH 0.75 * GRID_SPACE_SIZE
#define PLAYER_HEIGHT PLAYER_WIDTH

#define BORDER_LEFT (WINDOW_WIDTH / 2) - (0.5 * GRID_COLUMNS * GRID_SPACE_SIZE)
#define BORDER_RIGHT (WINDOW_WIDTH / 2) + (0.5 * GRID_COLUMNS * GRID_SPACE_SIZE)

#endif
