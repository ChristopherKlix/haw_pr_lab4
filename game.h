#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

// Defining a PLAYER aliases
// 
// Will be used by the PLAYER type
#define PLAYER_USER  1
#define PLAYER_COMP -1
#define NOPLAYER     0

// Defining game BOARD dimensions to avoid magic numbers
#define BOARD_LINES   3
#define BOARD_COLUMNS 3


// 
// Definition of custom game-related types
// 

// Defining a PLAYER type (see defines above)
// Player 1 (X) := 1
// Player 2 (O) := -1
// NOPLAYER ( ) := 0
typedef int PLAYER;

// Defining a BOARD type to represent
// the 2 dimensional array
// 
// First index represent X or Column
// Second index represent Y or Line
typedef int** BOARD;


// Defining a MOVE type to store & return moves
// struct
// 		int col;
// 		int lin;
// 		PLAYER player;
// 
// Is used mainly by:
// 		get_next_move()
// 		get_user_move()
// 		get_comp_move()
typedef struct MOVE
{
	int col;
	int lin;
	PLAYER player; 
} MOVE;

// 
// Initial call function to kick of the game
// 
// Includes the main event loop that runs the game
// 
void play(void);

// 
// Move related functions
// 

// Proxy function to retrieve MOVE
// independet of current player
MOVE get_next_move(PLAYER, BOARD); 

// Called by get_next_move()
// 
// Retrieve a move from the user via input
MOVE get_user_move(void);

// Called by get_next_move()
// 
// Retrieve a move from the computer (random)
MOVE get_comp_move(void);

// Simple helper function to verify a valid move
bool move_is_valid(MOVE, BOARD); 


// Accepts a MOVE and applies it (make) to the BOARD
void make_move(MOVE, BOARD);



int get_int(char*);








PLAYER check_for_win(BOARD); 

bool check_for_tie(BOARD); 

// convert int representing player to char player char
// 0 -> 'n'
// 1 -> 'X'
// -1 -> 'O' 
char player_to_char(int);

#endif