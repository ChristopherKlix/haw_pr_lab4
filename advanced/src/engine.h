#ifndef ENGINE_H
#define ENGINE_H

#include <stdbool.h>

void engine(void);
void play(void);

typedef struct BOARD
{
    int WIDTH;
    int HEIGHT;
    int **STATE;
} BOARD;

typedef enum PLAYER
{
    PLAYER1 = 1,
    PLAYER2 = -1,
    NOPLAYER = 0
} PLAYER;

typedef struct MOVE
{
    int x;
    int y;
    PLAYER player;
} MOVE;

typedef struct FIELD
{
    int x;
    int y;
} FIELD;

typedef struct SCORE
{
    int player_1;
    int player_2;
} SCORE;

typedef struct GAME_STYLE
{
    char *player_1;
    char *player_2;
} GAME_STYLE;

struct _GAME_STYLES {
    GAME_STYLE CLASSIC;
    GAME_STYLE BIT;
} GAME_STYLES;

extern GAME_STYLE G_S_CLASSIC;
extern GAME_STYLE G_S_BIT;

typedef enum AI_MODE
{
    AI_MODE_RANDOM = 1,
    AI_MODE_MINIMAX = 2
} AI_MODE;

typedef struct GAME_SETUP
{
    BOARD board;
    PLAYER initial_player;
    GAME_STYLE game_style;
    int player_count;
    AI_MODE ai_mode;
} GAME_SETUP;

typedef struct GAME_STATE
{
    BOARD board;
    PLAYER current_player;
    PLAYER winner;
    bool game_is_running;
    bool game_completed;
    char *note;
    bool user_cursor_visible;
    FIELD user_cursor_position;
    SCORE score;
} GAME_STATE;

typedef enum MAIN_MENU_SELECTION
{
    M_M_START = 0,
    M_M_OPTIONS = 1,
    M_M_EXIT = 2
} MAIN_MENU_SELECTION;

typedef enum OPTIONS_MENU_SELECTION
{
    O_M_GAME_STYLE = 0,
    O_M_INITIAL_PLAYER = 1,
    O_M_PLAYER_COUNT = 2,
    O_M_AI_MODE = 3,
    O_M_BACK = 4
} OPTIONS_MENU_SELECTION;

char *player_to_char(PLAYER, GAME_SETUP);
int player_to_int(PLAYER);
int start_game(GAME_SETUP*, GAME_STATE*);
MOVE get_next_move(GAME_SETUP, GAME_STATE*);
MOVE get_move_from_user(GAME_SETUP, GAME_STATE*);
MOVE get_random_move(GAME_STATE);
MOVE get_minimax_move(FIELD, int, bool);
int evaluate_position(int**, int, PLAYER);

bool is_valid_move(GAME_STATE, MOVE);
bool is_valid_move_minimax(int**, MOVE);

void make_move(GAME_STATE*, MOVE);
void make_move_minimax(int**, MOVE);

PLAYER check_for_win(GAME_SETUP*, GAME_STATE*);
bool check_for_tie(GAME_SETUP*, GAME_STATE*);

PLAYER check_for_win_minimax(int**);
bool check_for_tie_minimax(int**);

MAIN_MENU_SELECTION main_menu(GAME_SETUP*, GAME_STATE*);
OPTIONS_MENU_SELECTION options_menu(GAME_SETUP*, GAME_STATE*);
void toggle_game_style(GAME_SETUP*);
void toggle_initial_player(GAME_SETUP*);
void toggle_player_count(GAME_SETUP*);
void toggle_ai_mode(GAME_SETUP*);
int **generate_board(GAME_SETUP);

#endif