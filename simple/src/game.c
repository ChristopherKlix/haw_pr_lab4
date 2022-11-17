#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "game.h"
#include "view.h"

void play(void)
{
    srand(time(NULL));

    bool game_is_running = false;

    PLAYER current_player = PLAYER_USER;
    PLAYER winner = NOPLAYER;
    BOARD board;

    board = malloc(BOARD_COLUMNS * sizeof(*board));

    for (int col = 0; col < BOARD_COLUMNS; col++)
    {
        board[col] = calloc(BOARD_LINES, sizeof(board[0]));
    }

    do
    {
        game_is_running = true;

        //draw current board
        draw_board(board);

        //get next move
        MOVE move = get_next_move(current_player, board);

        //make next move
        make_move(move, board);

        //draw updated board
        draw_board(board);

        //check for winner
        winner = check_for_win(board);
        if (winner)
        {

            game_is_running = false;
        }

        //check for tie
        if (check_for_tie(board))
        {
            game_is_running = false;
        }

        //toggle player
        current_player = (current_player == PLAYER_USER) ? PLAYER_COMP : PLAYER_USER;

    } while (game_is_running);

    draw_endscreen(winner);

    return;
}

MOVE get_next_move(PLAYER player, BOARD board)
{
    MOVE move;
    bool is_valid = false;
    do
    {

        if (player == PLAYER_USER)
        {
            move = get_user_move();
            is_valid = move_is_valid(move, board);
            if (!is_valid) printf("Invalid move!");
        }
        else
        {
            move = get_comp_move();
            is_valid = move_is_valid(move, board);
        }



    } while (!is_valid);

    return move;

}

MOVE get_user_move(void)
{
    MOVE move;
    move.player = PLAYER_USER;
    char* col = "column";
    char* lin = "line";

    move.col = get_int(col);

    move.lin = get_int(lin);

    return move;
}

MOVE get_comp_move(void)
{
    MOVE move;
    move.player = PLAYER_COMP;
    move.col = rand() % BOARD_COLUMNS;
    move.lin = rand() % BOARD_LINES;

    return move;
}

bool move_is_valid(MOVE move, BOARD board)
{
    return (board[move.col][move.lin] == NOPLAYER);
}

void make_move(MOVE move, BOARD board)
{
    board[move.col][move.lin] = move.player;

    return;
}

PLAYER check_for_win(BOARD board)
{
    int sum;

    // check for 3 in a column
    for (int col = 0; col < BOARD_COLUMNS; col++)
    {
        sum = 0;
        for (int lin = 0; lin < BOARD_LINES; lin++)
        {
            sum += board[col][lin];
        }

        if (sum == 3) return PLAYER_USER;
        if (sum == -3) return PLAYER_COMP;
    }

    // check for 3 in a line
    for (int lin = 0; lin < BOARD_COLUMNS; lin++)
    {
        sum = 0;
        for (int col = 0; col < BOARD_LINES; col++)
        {
            sum += board[col][lin];
        }

        if (sum == 3) return PLAYER_USER;
        if (sum == -3) return PLAYER_COMP;
    }

    // check for 3 in -45 degree diagonal
    sum = 0;
    for (int i = 0; i < BOARD_COLUMNS; i++)
    {
        sum += board[i][i];
    }

    if (sum == 3) return PLAYER_USER;
    if (sum == -3) return PLAYER_COMP;

    // check for 3 in 45 degree diagonal
    sum = 0;
    for (int i = 0; i < BOARD_COLUMNS; i++)
    {
        sum += board[BOARD_COLUMNS-i-1][i];
    }

    if (sum == 3) return PLAYER_USER;
    if (sum == -3) return PLAYER_COMP;

    return NOPLAYER;
}

bool check_for_tie(BOARD board)
{
    if (check_for_win(board))
    {
        return false;
    }

    int sum = 0;

    for (int col = 0; col < BOARD_COLUMNS; col++)
    {
        for (int lin = 0; lin < BOARD_LINES; lin++)
        {
            if (!(board[col][lin]==NOPLAYER)) sum++;
        }
    }

    return (sum == 9);
}

char player_to_char(int player)
{
    switch (player)
    {
        case PLAYER_USER: return 'X';
        case PLAYER_COMP: return 'O';
        default: return ' ';
    }

}

int get_int(char* text)
{
    int x;
    int retval;
    char ch;
    bool finished = false;


    do
    {
        ch = '\0';
        printf("\nEnter %s: ", text);
        retval = scanf("%d%c", &x, &ch);
        x -= 1;

        if (retval != 2) printf("Not a correct number");
        else if (ch != '\n') printf("Not a correct number");
        else if (x < 0) printf("Number too small");
        else if (x > 2) printf("Number too large");
        else finished = true;

        while (ch != '\n') scanf("%c", &ch);

    } while (!finished);

    return x;
}
