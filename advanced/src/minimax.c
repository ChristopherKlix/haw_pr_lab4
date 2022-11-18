#include "minimax.h"
#include "engine.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <limits.h>

MOVE get_best_move(BOARD board, PLAYER player)
{
    MOVE possible_moves[100];

    // Count total empty fields
    int total_empty_fields = 0;

    for (size_t row = 0; row < 3; row++)
    {
        for (size_t col = 0; col < 3; col++)
        {
            if (board.STATE[row][col] == NOPLAYER) total_empty_fields++;
        }
    }

    // If first move, place center
    MOVE center_move = { .x = 1, .y = 1, .player = player };
    MOVE center_topleft_corner = { .x = 0, .y = 0, .player = player };
    MOVE center_topright_corner = { .x = 2, .y = 0, .player = player };
    MOVE center_bottomright_corner = { .x = 2, .y = 2, .player = player };
    MOVE center_bottomleft_corner = { .x = 0, .y = 2, .player = player };
    MOVE corner_moves[] = {
        center_topleft_corner,
        center_topright_corner,
        center_bottomright_corner,
        center_bottomleft_corner
    };
    int corner = rand() % 4;
    if (total_empty_fields == 9) return corner_moves[corner];


    int best_eval = INT_MIN;

    // loop over all possible next moves
    for (size_t row = 0; row < 3; row++)
    {
        for (size_t col = 0; col < 3; col++)
        {
            // printf("Test: { %zu, %zu }\n", i, j);
            // msleep(500);

            // Create a move and check if possible
            MOVE tmp_move = { .x = row, .y = col, .player = player };
            MOVE undo_move = { .x = row, .y = col, .player = NOPLAYER };

            bool is_a_possible_move = is_valid_move_minimax(board.STATE, tmp_move);

            // If possible move, make it, and minimax it
            if (is_a_possible_move)
            {
                printf("Found empty field: { %zu, %zu }\n", row, col);
                msleep(200);

                // Make tmp copy of the board
                make_move_minimax(board.STATE, tmp_move);

                // Get eval of move
                int eval = minimax(board.STATE, 1, false, get_opponent(player));

                // printf("Current Top Level Eval: %d\n", eval);

                // Check if move is better than current best
                if (eval > best_eval)
                {
                    // Push move on stack at first slot
                    push_move_on_possible_moves_stack(tmp_move, possible_moves);
                }
                // Update max_eval to new max eval
                best_eval = max(eval, best_eval);


                // undo move
                make_move_minimax(board.STATE, undo_move);
            }
        }
    }

    // return best possible move
    printf("Max eval: %d\n", best_eval);
    return possible_moves[0];
}

void push_move_on_possible_moves_stack(MOVE move, MOVE *possible_moves)
{
    // Create new array to tmp copy array
    MOVE tmp_array[100];

    // Copy array into tmp
    for (size_t i = 0; i < 100; i++)
    {
        tmp_array[i] = possible_moves[i];
    }

    // Copy tmp copy into array offset by 1
    // i.e., shifting the array by 1 to the right
    for (size_t i = 0; i < 99; i++)
    {
        possible_moves[i] = tmp_array[i+1];
    }

    // Store move on first slot in array
    possible_moves[0] = move;

    return;
}

void copy_board(BOARD board, BOARD *buffer)
{
    // Declare 2D Array in Heap
    int **tmp_board_state;

    // Initializing 2D Array with dimensions of the board
    tmp_board_state = malloc(3 * sizeof(*tmp_board_state));
    for (int i = 0; i < 3; i++)
    {
        tmp_board_state[i] = malloc(3 * sizeof(tmp_board_state[0]));
    }

    // Setting all values to 0 (--> no player)
    for (size_t i = 0; i < 3; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            tmp_board_state[i][j] = board.STATE[i][j];
        }
    }

    buffer->STATE = tmp_board_state;

    return;
}

bool game_is_completed(int **state)
{
    PLAYER winner = check_for_win_minimax(state);
    bool is_tie = check_for_tie_minimax(state);
    bool completed = false;

    if (winner == PLAYER1 || winner == PLAYER2 || is_tie)
    {
        completed = true;
    }

    return completed;
}

int minimax(int **state, int depth, bool is_maximizing, PLAYER player)
{
    // Static evaluate of position
    if (game_is_completed(state))
    {
        int eval = evaluate_position(state, depth, player);
        return eval;
    }

    // Maximizing player
    if (is_maximizing)
    {
        int best_eval = INT_MIN;

        // get next linear move
        for (size_t y = 0; y < 3; y++)
        {
            for (size_t x = 0; x < 3; x++)
            {
                MOVE move = { .x = x, .y = y, .player = player };
                MOVE undo_move = { .x = x, .y = y, .player = NOPLAYER };

                if (is_valid_move_minimax(state, move))
                {
                    // make move
                    make_move_minimax(state, move);

                    // call minimax on in
                    int eval;
                    // if (depth == 3) eval = 13;
                    eval = minimax(state, depth + 1, false, get_opponent(player));

                    // set max_eval
                    best_eval = max(best_eval, eval);

                    // undo move
                    make_move_minimax(state, undo_move);
                }
            }
        }

        return best_eval;
    }
    else
    {
        int best_eval = INT_MAX;

        // get next linear move
        for (size_t y = 0; y < 3; y++)
        {
            for (size_t x = 0; x < 3; x++)
            {
                MOVE move = { .x = x, .y = y, .player = player };
                MOVE undo_move = { .x = x, .y = y, .player = NOPLAYER };

                if (is_valid_move_minimax(state, move))
                {
                    // make next move
                    make_move_minimax(state, move);

                    // call minimax on in
                    int eval = minimax(state, depth + 1, true, get_opponent(player));

                    // set min_eval
                    best_eval = min(best_eval, eval);

                    // undo move
                    make_move_minimax(state, undo_move);
                }
            }
        }

        return best_eval;
    }
}

PLAYER get_opponent(PLAYER player)
{
    if (player == PLAYER1) return PLAYER2;
    if (player == PLAYER2) return PLAYER1;
    
    return NOPLAYER;
}

// For minimax
PLAYER check_for_win_minimax(int **state)
{
    int sum = 0;

    // Check rows
    for (size_t row = 0; row < 3; row++)
    {
        sum = 0;
        for (size_t col = 0; col < 3; col++)
        {
            sum += state[row][col];
        }

        // printf("Row: %i\n", sum);

        if (sum == 3) return PLAYER1;
        if (sum == (-3)) return PLAYER2;
    }

    // Check columns
    for (size_t col = 0; col < 3; col++)
    {
        sum = 0;
        for (size_t row = 0; row < 3; row++)
        {
            sum += state[row][col];
        }

        // printf("Col: %i\n", sum);

        if (sum == 3) return PLAYER1;
        if (sum == (-3)) return PLAYER2;
    }

    // Check diagonals
    sum = 0;
    for (size_t i = 0; i < 3; i++)
    {
        sum += state[i][i];
    }
    if (sum == 3) return PLAYER1;
    if (sum == (-3)) return PLAYER2;

    sum = 0;
    for (size_t i = 0; i < 3; i++)
    {
        sum += state[2-i][i];
    }
    if (sum == 3) return PLAYER1;
    if (sum == (-3)) return PLAYER2;

    return NOPLAYER;
}

bool check_for_tie_minimax(int **state)
{
    // printf("Checking for tie...\n");

    int sum = 0;

    for (size_t row = 0; row < 3; row++)
    {
        for (size_t col = 0; col < 3; col++)
        {
            int val = pow(state[row][col], 2);
            sum += val;
        }
    }

    if (sum == 9) return true;
    else return false;
}

void make_move_minimax(int **state, MOVE move)
{
    state[move.x][move.y] = move.player;
    return;
}

int evaluate_position(int **state, int depth, PLAYER player)
{
    for (size_t i = 0; i < 3; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            int a = state[j][i];
            char c = (a == (-1)) ? 'O' : 'X';
            printf("%c", c);
        }
        printf("\n");
    }

    // printf("Eval path\n");
    int eval = 0;
    // int depth_punishment = max(0, 1000 - (100 * depth));

    PLAYER winner = check_for_win_minimax(state);
    printf("Winner: %d\n", winner);

    if (winner == get_opponent(player))
    {
        // printf("Player 1 wins\n");
        eval -= 1000 + (10 * depth);
        // eval = -10;
    }
    else if (winner == player)
    {
        // printf("Player 2 wins\n");
        eval += 1000 - (10 * depth);
        // eval = 10;
    }

    return eval;
}

bool is_valid_move_minimax(int **state, MOVE move)
{
    return (state[move.x][move.y] == 0);
}
