#ifndef VIEW_H
#define VIEW_H

#include "game.h"

// Draw function to draw the board onto the screen
void draw_board(BOARD);

// OS-independet function to clear terminal
// UNIX and WIN32
void clear_screen(void);

// Draw function to draw the endscreen of the game
void draw_endscreen(PLAYER);

#endif