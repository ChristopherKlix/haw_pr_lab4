#ifndef VIEW_H
#define VIEW_H

#include "engine.h"

#include <stdbool.h>

// ------------------------------------------------
// Draw gamemap
//
// param: board[][]
// return: board[][]
// ------------------------------------------------
void draw_gamemap(GAME_SETUP, GAME_STATE);

// ------------------------------------------------
// draw board selection cursor
//
// param:  GAME_SETUP, GAME_STATE
// return: void
// ------------------------------------------------
void draw_user_cursor(GAME_SETUP, GAME_STATE);

// ------------------------------------------------
// draw note to the console
//
// param:  GAME_STATE
// return: void
// ------------------------------------------------
// the note is the text displayed
// at the bottom of the frame
// max character: 35
// overshoot will be terminated by a ...
//
// note can be set via GAME_STATE's note field
// and will be updated via draw_gamemap()
void draw_note(GAME_STATE);

// ------------------------------------------------
// move cursor in terminal
//
// param:  FIELD
// return: void
// ------------------------------------------------
void move_cursor_to_field(FIELD);

// ------------------------------------------------
// draw endscreen
//
// param:  GAME_STATE
// return: void
// ------------------------------------------------
void draw_endscreen(GAME_STATE);

// ------------------------------------------------
// draw bye
//
// param:  void
// return: void
// ------------------------------------------------
void draw_bye(void);

// ------------------------------------------------
// draw main menu
//
// param:  int*, GAME_STATE
// return: void
// ------------------------------------------------
void draw_main_menu(int*, GAME_STATE);

// ------------------------------------------------
// draw options menu
//
// param:  int*, GAME_SETUP
// return: void
// ------------------------------------------------
void draw_options_menu(int*, GAME_SETUP);

#endif