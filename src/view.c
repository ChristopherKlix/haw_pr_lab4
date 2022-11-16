#include <stdio.h>
#include <stdlib.h>

#include "view.h"
#include "game.h"

void draw_board(BOARD board)
{
    clear_screen();

    printf("    C1  C2  C3\n");
    for (int lin = 0; lin < BOARD_LINES; lin++)
    {
        printf(" L%d ", lin + 1);

        for (int col = 0; col < BOARD_COLUMNS; col++)
        {

            printf(" %c ", player_to_char(board[col][lin]));

            if (col < 2) printf("|");

        }

        printf("\n");

        if (lin < 2) printf("    ---|---|---\n");
    }

    return;
}

void clear_screen(void)
{
#ifdef WIN32
    system("cls");
#else
    system("clear");
#endif
}

void draw_endscreen(PLAYER player)
{
    if (!player) printf("\n\n   TIE\n");
    else if (player == PLAYER_USER) printf("\n\n   YOU HAVE WON\n");
    else if (player == PLAYER_COMP) printf("\n\n   YOU HAVE LOST\n");

    return;
}