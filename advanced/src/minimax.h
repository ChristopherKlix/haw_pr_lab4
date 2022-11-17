#ifndef MINIMAX_H
#define MINIMAX_H

#include "engine.h"
#include <stdbool.h>

MOVE get_best_move(BOARD, PLAYER);
void push_move_on_possible_moves_stack(MOVE, MOVE*);
void copy_board(BOARD, BOARD*);
bool game_is_completed(int**);
int minimax(int**, int, bool, PLAYER);
PLAYER get_opponent(PLAYER);

#endif