# HAW PR Lab 4

Solution to HAW PR Lab 4

## main.c

Keeping it short and simple.
The main function simply invokes the game's initial call function `play()`.

```C
#include "game.h"

int main(void)
{
    play();

    return 0;
}
```

## game.c & game.h

The games main logic is contained in these two files.
Once the `play()` function is invoked the main event loop is started. This loop continues until `game_is_running` is set to false by checking end condition of the game.

Once the while-loop has been exited the `play()` function invokes `void draw_endscreen(PLAYER winner)` and returns to the caller function.

```C
void play(void)
{
    // initial configuration stuff
    // ...

    do
    {
        game_is_running = true;

        //draw current board
        draw_board(board);

        //get next move
        MOVE move = get_next_move(currant_player, board);

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
```
