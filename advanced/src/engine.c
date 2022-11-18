//
// ENGINE
//
// This file includes all logic
// related to the execution
// of the game.
//
// This file does not draw any
// content to the console
// which makes it independent
// of view

#include "engine.h"
#include "view.h"
#include "util.h"
#include "keypress.h"
#include "minimax.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// string library that allows for
// manipulation of strings
// e.g., concatenation of strings
#include <string.h>

// simple math logic
// e.g., sqrt, pow, etc.
#include <math.h>

// time-related logic
#include <time.h>

// provides insights into
// min and max values
// closest to (+/-) infinity
#include <limits.h>


// ------------------------------------------------
// play
//
// param:  void
// return: void
// ------------------------------------------------
// primary proxy function that
// invokes the main game execution
void play(void)
{
    clear_screen();

    // start engine
    engine();

    return;
}

// ------------------------------------------------
// GAME STYLE definitions
// ------------------------------------------------
// pre-defined player icons
//
// note: the usage of global variables should be
//       avoided. game development commonly
//       utilizes OOP, which proves C
//       unelegant for game development,
//       hence the use of global vars.
GAME_STYLE G_S_CLASSIC = {
    .player_1 = "X",
    .player_2 = "O"
};

GAME_STYLE G_S_BIT = {
    .player_1 = "1",
    .player_2 = "0"
};

// ------------------------------------------------
// engine
//
// param:  void
// return: void
// ------------------------------------------------
// main execution event-loop
//
// this function runs for the entirety
// of the game. It controls the main menu
// and allows for consecutive games.
void engine(void)
{
    // ------------------------------------------------
    // Initializing game setup
    // ------------------------------------------------
    GAME_SETUP game_setup;

    // Set board width
    game_setup.board.HEIGHT = 3;
    game_setup.board.WIDTH = 3;

    // Set initial player
    // can be overwritten by the main menu options
    game_setup.initial_player = PLAYER1;

    // Set player count
    // can be overwritten by the main menu options
    game_setup.player_count = 1;

    // Set AI mode
    // can be overwritten by the main menu options
    game_setup.ai_mode = AI_MODE_RANDOM;


    // Set game styles
    game_setup.game_style = G_S_CLASSIC;


    // ------------------------------------------------
    // Initializing gaming state
    // ------------------------------------------------
    GAME_STATE game_state;

    // Generate an empty game board
    game_state.board.STATE = generate_board(game_setup);

    // Set current player to initial player
    game_state.current_player = game_setup.initial_player;

    // Set score
    game_state.score.player_1 = 0;
    game_state.score.player_2 = 0;

    // Set note
    game_state.note = "";

    game_state.user_cursor_visible = false;
    FIELD default_user_cursor_field = { .x = 1, .y = 1 };
    game_state.user_cursor_position = default_user_cursor_field;


    // ------------------------------------------------
    // Launch game
    // ------------------------------------------------
    do
    {
        // start event loop
        game_state.game_is_running = true;

        // update score
        char score[100] = "";
        char score_1[10];
        char score_2[10];
        strcat(score, "PLAYER 1 \\\\ ");
        sprintf(score_1, "%d", game_state.score.player_1);
        strcat(score, score_1);
        strcat(score, " : ");
        sprintf(score_2, "%d", game_state.score.player_2);
        strcat(score, score_2);
        strcat(score, " // PLAYER 2");
        game_state.note = score;

        // open main menu
        MAIN_MENU_SELECTION m_m_selection;
        m_m_selection = main_menu(&game_setup, &game_state);

        // act on main menu selection
        if (m_m_selection == M_M_START)
        {
            start_game(&game_setup, &game_state);
        }
        else if (m_m_selection == M_M_OPTIONS)
        {
            options_menu(&game_setup, &game_state);
        }
        else if (m_m_selection == M_M_EXIT)
        {
            game_state.game_is_running = false;
        }
        else
        {
            // main menu returned an invalid selection
            // so quit the program
            exit(-1);
        }
    } while (game_state.game_is_running);


    // ------------------------------------------------
    // Bye
    // ------------------------------------------------
    draw_bye();

    // Stop game
    clear_screen();


    // ------------------------------------------------
    // Free board memory
    // ------------------------------------------------
    for (int i = 0; i < game_setup.board.WIDTH; i++) {
        free(game_state.board.STATE[i]);
    }

    free(game_state.board.STATE);

    return;
}

// ------------------------------------------------
// main menu
//
// param:  MAIN_MENU_SELECTION
// return: GAME_SETUP *game_setup,
//         GAME_STATE *game_state
// ------------------------------------------------
// execution of the main menu event-loop
//
// this function runs for the entirety
// of the main menu and returns a selection
// to the engine event-loop.
MAIN_MENU_SELECTION main_menu(GAME_SETUP *game_setup, GAME_STATE *game_state)
{
    int selection[3] = { false };
    selection[M_M_START] = true;

    bool show_main_menu = true;

    char user_input = '\0';

    const int buffer_len =  40;
    char buffer[buffer_len] = { 0 };

    bool escape_seq = false;

    do
    {
        draw_main_menu(selection, *game_state);

        char c = keypress();

        buffer[20] = c;

        if (escape_seq)
        {
            switch (buffer[20])
            {
                case 'A':
                    rotate_array(selection, 3, (-1), true);
                    escape_seq = false;
                    break;
                case 'B':
                    rotate_array(selection, 3, 1, true);
                    escape_seq = false;
                    break;
                case 'C':
                case 'D':
                default:
                    escape_seq = false;
            }
        }
        else
        {
            switch (buffer[20])
            {
                case 's':
                case 'q':
                case 'e':
                case 'x':
                case 'o':
                    user_input = buffer[20];
                    show_main_menu = false;
                    break;
                case '\r':
                case '\n':
                    if (selection[0])
                    {
                        user_input = 's';
                        show_main_menu = false;
                    }
                    else if (selection[1])
                    {
                        user_input = 'o';
                        show_main_menu = false;
                    }
                    else if (selection[2])
                    {
                        user_input = 'q';
                        show_main_menu = false;
                    }
                    break;
                default:
                    show_main_menu = true;
            }
        }

        if (buffer[20] == '[')
        {
            escape_seq = true;
        }

        for (size_t i = 0; i < buffer_len-1; i++)
        {
            buffer[i] = buffer[i+1];
        }
    } while (show_main_menu);

    if (user_input == 's') return M_M_START;
    else if (user_input == 'o') return M_M_OPTIONS;
    else if (user_input == 'q') return M_M_EXIT;
    else if (user_input == 'e') return M_M_EXIT;
    else if (user_input == 'x') return M_M_EXIT;
    else return (-1);
}

// ------------------------------------------------
// options menu
//
// param:  OPTIONS_MENU_SELECTION
// return: GAME_SETUP *game_setup,
//         GAME_STATE *game_state
// ------------------------------------------------
// execution of the options menu event-loop
//
// this function runs for the entirety
// of the options menu and returns a selection
// to the main menu event-loop.
OPTIONS_MENU_SELECTION options_menu(GAME_SETUP *game_setup, GAME_STATE *game_state)
{
    int selection[5] = { false };
    selection[O_M_GAME_STYLE] = true;

    bool show_options_menu = true;

    char user_input = '\0';

    const int buffer_len =  40;
    char buffer[buffer_len] = { 0 };

    bool escape_seq = false;

    do
    {
        // Draw view
        draw_options_menu(selection, *game_setup);

        // Await keypress
        buffer[20] = keypress();

        // ESCAPE SEQUENCE action
        if (escape_seq)
        {
            switch (buffer[20])
            {
                case 'A':
                    rotate_array(selection, 5, (-1), true);
                    escape_seq = false;
                    break;
                case 'B':
                    rotate_array(selection, 5, 1, true);
                    escape_seq = false;
                    break;
                case 'C':
                case 'D':
                default:
                    escape_seq = false;
            }
        }
        else if (buffer[20] == '[') escape_seq = true;



        switch (buffer[20])
        {
            case 'b':
            case 'q':
            case 'x':
                user_input = buffer[20];
                show_options_menu = false;
                break;
            case '\r':
            case '\n':
                if (selection[0])
                {
                    toggle_game_style(game_setup);
                }
                else if (selection[1])
                {
                    toggle_initial_player(game_setup);
                }
                else if (selection[2])
                {
                    toggle_player_count(game_setup);
                }
                else if (selection[3])
                {
                    toggle_ai_mode(game_setup);
                }
                else if (selection[4])
                {
                    user_input = 'b';
                    show_options_menu = false;
                }

                break;
            default:
                show_options_menu = true;
        }


        // Move buffer
        for (size_t i = 0; i < buffer_len-1; i++)
        {
            buffer[i] = buffer[i+1];
        }
    } while (show_options_menu);

    if (user_input == 'b') return O_M_BACK;
    else if (user_input == 'q') return O_M_BACK;
    else if (user_input == 'x') return O_M_BACK;
    else return (-1);
}

// ------------------------------------------------
// toggle options menu options
// ------------------------------------------------
// execution of the options menu event-loop
//
// this function runs for the entirety
// of the options menu and returns a selection
// to the main menu event-loop.

void toggle_game_style(GAME_SETUP *game_setup)
{
    if (strcmp(game_setup->game_style.player_1, "X") == 0)
    {
        game_setup->game_style = G_S_BIT;
    }
    else if (strcmp(game_setup->game_style.player_1, "1") == 0)
    {
        game_setup->game_style = G_S_CLASSIC;
    }

    return;
}

void toggle_initial_player(GAME_SETUP *game_setup)
{
    if (game_setup->initial_player == PLAYER1)
    {
        game_setup->initial_player = PLAYER2;
    }
    else
    {
        game_setup->initial_player = PLAYER1;
    }

    return;
}

void toggle_player_count(GAME_SETUP *game_setup)
{
    if (game_setup->player_count == 1)
    {
        game_setup->player_count = 2;
    }
    else if (game_setup->player_count == 2)
    {
        game_setup->player_count = 0;
    }
    else
    {
        game_setup->player_count = 1;
    }

    return;
}

void toggle_ai_mode(GAME_SETUP *game_setup)
{
    if (game_setup->ai_mode == AI_MODE_RANDOM)
    {
        game_setup->ai_mode = AI_MODE_MINIMAX;
    }
    else
    {
        game_setup->ai_mode = AI_MODE_RANDOM;
    }

    return;
}



int start_game(GAME_SETUP *game_setup, GAME_STATE *game_state)
{
    srand(time(NULL));

    PLAYER winner;

    game_state->current_player = game_setup->initial_player;

    game_state->board.STATE = generate_board(*game_setup);

    do
    {
        game_state->game_completed = false;

        // draw game map
        draw_gamemap(*game_setup, *game_state);

        // await input
        MOVE move = get_next_move(*game_setup, game_state);

        // assign input
        make_move(game_state, move);

        // draw game map
        draw_gamemap(*game_setup, *game_state);

        // check for win
        winner = check_for_win(game_setup, game_state);

        if (winner == PLAYER1 || winner == PLAYER2)
        {
            game_state->game_completed = true;
        }

        if (check_for_tie(game_setup, game_state))
        {
            game_state->game_completed = true;
        }

        // toggle player
        game_state->current_player = (game_state->current_player == PLAYER1) ? PLAYER2 : PLAYER1;

    } while (!game_state->game_completed);


    // ------------------------------------------------
    // Set winner
    // ------------------------------------------------
    game_state->winner = winner;

    if (winner == PLAYER1) game_state->score.player_1++;
    if (winner == PLAYER2) game_state->score.player_2++;

    draw_endscreen(*game_state);
    keypress();

    return 0;
}



MOVE get_next_move(GAME_SETUP game_setup, GAME_STATE *game_state)
{
    MOVE move;

    char note[100];
    char *player_char = player_to_char(game_state->current_player, game_setup);
    char *player_number = (game_state->current_player == PLAYER1) ? "1" : "2";
    strcat(note, "PLAYER ");
    strcat(note, player_number);
    strcat(note, " (");
    strcat(note, player_char);
    strcat(note, ") it's your turn!");
    game_state->note = note;
    draw_gamemap(game_setup, *game_state);

    // Guard if for PLAYER 1 (always user) &
    //          for second user PLAYER 2
    if (game_state->current_player == PLAYER1 || game_setup.player_count == 2)
    {
        // Get move
        if (game_setup.player_count == 0)
        {
            move = get_best_move(game_state->board, PLAYER1);
        }
        else
        {
            move = get_move_from_user(game_setup, game_state);
        }

        move.player = PLAYER1;
        return move;
    }


    // Get AI move based on Game Setup

    int rand_time_delay = (rand() % 500) + 500;
    msleep(rand_time_delay);

    if (game_setup.ai_mode == AI_MODE_RANDOM)
    {
        // Get random number out of [0, 1, 2]
        move = get_random_move(*game_state);
        move.player = PLAYER2;
        return move;
    }
    else if (game_setup.ai_mode == AI_MODE_MINIMAX)
    {
        fprintf(stderr, "Getting best move\n");
        move = get_best_move(game_state->board, PLAYER2);
        move.player = PLAYER2;
        return move;
    }
    else
    {
        fprintf(stderr, "Exception thrown at get_next_move()\n");
        exit(1);
    }
}

MOVE get_random_move(GAME_STATE game_state)
{
    MOVE move;

    do
    {
        move.x = rand() % 3;
        move.y = rand() % 3;
    } while (!is_valid_move(game_state, move));

    return move;
}

bool is_valid_move(GAME_STATE game_state, MOVE move)
{
    return (game_state.board.STATE[move.x][move.y] == 0);
}

MOVE get_move_from_user(GAME_SETUP game_setup, GAME_STATE *game_state)
{
    MOVE move;
    char k;
    bool move_is_valid;

    FIELD default_user_cursor_field = { .x = 1, .y = 1 };
    game_state->user_cursor_position = default_user_cursor_field;

    // user_cursor_position[game_state->user_cursor_position.x][game_state->user_cursor_position.y];

    bool wait_for_input = false;
    char user_input = '\0';
    const int buffer_len =  40;
    char buffer[buffer_len] = { 0 };
    bool escape_seq = false;

    do
    {
        do
        {
            /* code */
            wait_for_input = true;

            // Set user cursor
            game_state->user_cursor_visible = true;
            draw_gamemap(game_setup, *game_state);

            // Await keypress
            buffer[20] = keypress();

            // ESCAPE SEQUENCE action
            if (escape_seq)
            {
                int y;
                int x;

                switch (buffer[20])
                {
                    case 'A':
                        // move up
                        y = game_state->user_cursor_position.y;
                        y -= 1;
                        printf("y: %d\n", y);
                        y = mod(y, 3);
                        printf("y%%: %d\n", y);
                        game_state->user_cursor_position.y = y;
                        escape_seq = false;
                        break;
                    case 'B':
                        // move down
                        y = game_state->user_cursor_position.y;
                        y += 1;
                        y = mod(y, 3);
                        game_state->user_cursor_position.y = y;
                        escape_seq = false;
                        break;
                    case 'C':
                        // move left
                        x = game_state->user_cursor_position.x;
                        x += 1;
                        x = mod(x, 3);
                        game_state->user_cursor_position.x = x;
                        escape_seq = false;
                        break;
                    case 'D':
                        // move right
                        x = game_state->user_cursor_position.x;
                        x -= 1;
                        x = mod(x, 3);
                        game_state->user_cursor_position.x = x;
                        escape_seq = false;
                        break;
                    default:
                        escape_seq = false;
                }
            }
            else if (buffer[20] == '[') escape_seq = true;

            switch (buffer[20])
            {
                case '\r':
                case '\n':
                    // confirm selection
                    move.x = game_state->user_cursor_position.x;
                    move.y = game_state->user_cursor_position.y;
                    wait_for_input = false;
                    break;
                default:
                    wait_for_input = true;
            }

            // Move buffer
            for (size_t i = 0; i < buffer_len-1; i++)
            {
                buffer[i] = buffer[i+1];
            }
        } while (wait_for_input);

        move_is_valid = is_valid_move(*game_state, move);

        if (!move_is_valid)
        {
            char note[100] = "This field is already occupied.";
            game_state->note = note;
            draw_gamemap(game_setup, *game_state);
        }
    } while (!move_is_valid);

    move.player = game_state->current_player;

    game_state->user_cursor_visible = false;
    draw_gamemap(game_setup, *game_state);

    return move;
}

void make_move(GAME_STATE *game_state, MOVE move)
{
    game_state->board.STATE[move.x][move.y] = move.player;
    return;
}

PLAYER check_for_win(GAME_SETUP *game_setup, GAME_STATE *game_state)
{
    // printf("Checking for win...\n");
    int sum = 0;

    // Check rows
    for (size_t row = 0; row < game_setup->board.HEIGHT; row++)
    {
        sum = 0;
        for (size_t col = 0; col < game_setup->board.WIDTH; col++)
        {
            sum += game_state->board.STATE[row][col];
        }

        // printf("Row: %i\n", sum);

        if (sum == 3) return PLAYER1;
        if (sum == (-3)) return PLAYER2;
    }

    // Check columns
    for (size_t col = 0; col < game_setup->board.WIDTH; col++)
    {
        sum = 0;
        for (size_t row = 0; row < game_setup->board.HEIGHT; row++)
        {
            sum += game_state->board.STATE[row][col];
        }

        // printf("Col: %i\n", sum);

        if (sum == 3) return PLAYER1;
        if (sum == (-3)) return PLAYER2;
    }

    // Check diagonals
    sum = 0;
    for (size_t i = 0; i < 3; i++)
    {
        sum += game_state->board.STATE[i][i];
    }
    if (sum == 3) return PLAYER1;
    if (sum == (-3)) return PLAYER2;

    sum = 0;
    for (size_t i = 0; i < 3; i++)
    {
        sum += game_state->board.STATE[2-i][i];
    }
    if (sum == 3) return PLAYER1;
    if (sum == (-3)) return PLAYER2;

    return NOPLAYER;
}

bool check_for_tie(GAME_SETUP *game_setup, GAME_STATE *game_state)
{
    int sum = 0;

    for (size_t row = 0; row < game_setup->board.HEIGHT; row++)
    {
        for (size_t col = 0; col < game_setup->board.WIDTH; col++)
        {
            int val = pow(game_state->board.STATE[row][col], 2);
            sum += val;
        }
    }

    if (sum == 9) return true;
    else return false;
}

char *player_to_char(PLAYER player, GAME_SETUP game_setup)
{
    if (player == PLAYER1) return game_setup.game_style.player_1;
    else if (player == PLAYER2) return game_setup.game_style.player_2;
    else return " ";
}

int player_to_int(PLAYER player)
{
    if (player == PLAYER1) return 1;
    else if (player == PLAYER2) return 2;
    else return 0;
}

int **generate_board(GAME_SETUP game_setup)
{
    // int (*board)[BOARD_HEIGHT] = malloc(sizeof(int[BOARD_WIDTH][BOARD_HEIGHT]));

    // Declare 2D Array in Heap
    int **board;

    // Initializing 2D Array with dimensions of the board
    board = malloc(game_setup.board.WIDTH * sizeof(*board));
    for (int i = 0; i < game_setup.board.WIDTH; i++) {
        board[i] = malloc(game_setup.board.HEIGHT * sizeof(board[0]));
    }

    // Setting all values to 0 (--> no player)
    for (size_t i = 0; i < game_setup.board.WIDTH; i++)
    {
        for (size_t j = 0; j < game_setup.board.HEIGHT; j++)
        {
            board[i][j] = NOPLAYER;
        }

    }

    return board;
}
