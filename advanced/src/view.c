// 
// VIEW
// 
#include "view.h"
#include "engine.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void redraw(void)
{
    // char *target = "game_map";

    // if (strcmp(target, "main_menu") == 0)
    // {
    //     draw_main_menu();
    // }
    // else if (strcmp(target, "game_map") == 0)
    // {
    //     draw_gamemap();
    // }
}


void draw_gamemap(GAME_SETUP game_setup, GAME_STATE game_state)
{
    clear_screen();

    printf("╔═══════════════════════════════════╗\n");
    printf("║            \033[1mTIC TAC TOE\033[0m            ║\n");
    printf("╠═══════════════════════════════════╣\n");
    printf("║                                   ║\n");
    printf("║               │   │               ║\n");
    printf("║            ───┼───┼───            ║\n");
    printf("║               │   │               ║\n");
    printf("║            ───┼───┼───            ║\n");
    printf("║               │   │               ║\n");
    printf("║                                   ║\n");
    printf("╠═══════════════════════════════════╣\n");
    printf("║   PLAYER _ (_) it's your turn!    ║\n");
    printf("╚═══════════════════════════════════╝\n");

    // Place fields
    for (size_t y = 0; y < 3; y++)
    {
        for (size_t x = 0; x < 3; x++)
        {
            FIELD field = { .x = x, .y = y};
            move_cursor_to_field(field);
            char player_char[100];
            strcpy(player_char, player_to_char(game_state.board.STATE[x][y], game_setup));
            fprintf(stderr, "%s", player_char);
        }
    }

    // Print note
    draw_note(game_state);

    fprintf(stderr, "\033[14;0H");

    if (game_state.user_cursor_visible)
    {
        draw_user_cursor(game_setup, game_state);
    }

    return;
}

void draw_user_cursor(GAME_SETUP game_setup, GAME_STATE game_state)
{
    // Define field
    FIELD field = game_state.user_cursor_position;
    move_cursor_to_field(field);

    bool field_is_free;
    int game_map_value = game_state.board.STATE[field.x][field.y];

    char *player = player_to_char(game_state.current_player, game_setup);
    char s[100] = "";
    strcat(s, "\033[0;5m");
    strcat(s, "[");
    if (game_map_value == 0)
    {
        strcat(s, "\033[0;2;32m");
        strcat(s, player);
    }
    else
    {
        strcat(s, "\033[0;31m");
        strcat(s, player_to_char(game_map_value, game_setup));
    }
    strcat(s, "\033[0;5m");
    strcat(s, "]");
    strcat(s, "\033[0m");

    fprintf(stderr, "\033[1D");
    fprintf(stderr, "%s", s);

    fprintf(stderr, "\033[14;0H");

    return;
}

void draw_note(GAME_STATE game_state)
{
    fprintf(stderr, "\033[12;2H");

    char note[100] = "";
    int note_len = 0;
    int _i = 0;

    while (game_state.note[_i] != '\0')
    {
        note_len++;
        _i++;
    }

    int white_space = 35;

    if (note_len < 35+1)
    {
        white_space -= note_len;
        white_space = (int) floor(white_space / 2.f);
        strcpy(note, game_state.note);
    }
    else
    {
        white_space = 0;
        for (size_t i = 0; i < 32; i++)
        {
            note[i] = game_state.note[i];
        }
        strcat(note, "...");
    }

    for (size_t i = 0; i < white_space; i++)
    {
        fprintf(stderr, " ");
    }
    
    fprintf(stderr, "%s", note);
    
    for (size_t i = 0; i < white_space; i++)
    {
        fprintf(stderr, " ");
    }

    fprintf(stderr, "\033[14;0H");

    return;
}

void draw_endscreen(GAME_STATE game_state)
{
    // clear_screen();

    printf("\n");
    printf("\n");
    printf("\033[1m");
    if (game_state.winner == PLAYER1)
    {
        printf("          PLAYER X won!\n");
    }
    else if (game_state.winner == PLAYER2)
    {
        printf("          PLAYER O won!\n");
    }
    else if (game_state.winner == 0)
    {
        printf("         Looks like a tie!\n");
    }
    else
    {
        printf("         Looks like... uhm...\n");
        printf("         ERRORRRRRRRRRRRRR\n");
    }
    printf("\033[0m");

    printf("\n");
    printf("    \033[2mPress any key to continue...\033[0m\n");
    printf("\n");

    printf("\033[0m");

    return;
}

void draw_bye(void)
{
    clear_screen();

    printf("╔═══════════════════════════════════╗\n");
    printf("║            \033[1mTIC TAC TOE\033[0m            ║\n");
    printf("╠═══════════════════════════════════╣\n");
    printf("║                                   ║\n");
    printf("║               │   │               ║\n");
    printf("║            ───┼───┼───            ║\n");
    printf("║             B │ Y │ E             ║\n");
    printf("║            ───┼───┼───            ║\n");
    printf("║               │   │               ║\n");
    printf("║                                   ║\n");
    printf("╠═══════════════════════════════════╣\n");
    printf("║                                   ║\n");
    printf("╚═══════════════════════════════════╝\n");

    for (size_t i = 0; i < 35; i++)
    {
        fprintf(stderr, "\033[2F");
        fprintf(stderr, "\033[%zuC", i+1);
        fprintf(stderr, "#");
        fprintf(stderr, "\033[2E");
        msleep(50);
    }

    return;
}

void draw_main_menu(int *selection, GAME_STATE game_state)
{
    clear_screen();

    printf("╔═══════════════════════════════════╗\n");
    printf("║            \033[1mTIC TAC TOE\033[0m            ║\n");
    printf("╠═══════════════════════════════════╣\n");
    printf("║                                   ║\n");
    printf("║                                   ║\n");

    // if (selection[M_M_START]) printf("        >   \033[5mSTART\033[0m   <\n");
    if (selection[M_M_START]) printf("║           >   \033[1mSTART\033[0m   <           ║\n");
    else printf("║               START               ║\n");

    if (selection[M_M_OPTIONS]) printf("║           >  \033[1mOPTIONS\033[0m  <           ║\n");
    else printf("║              OPTIONS              ║\n");

    if (selection[M_M_EXIT]) printf("║           >   \033[1mEXIT!\033[0m   <           ║\n");
    else printf("║               EXIT!               ║\n");

    printf("║                                   ║\n");
    printf("║                                   ║\n");
    printf("╠═══════════════════════════════════╣\n");
    printf("║   PLAYER _ (_) it's your turn!    ║\n");
    printf("╚═══════════════════════════════════╝\n");

    printf("\033[0m");

    // Print note
    draw_note(game_state);

    return;
}

void draw_options_menu(int *selection, GAME_SETUP game_setup)
{
    clear_screen();
    char *p_1_s = game_setup.game_style.player_1;
    char *p_2_s = game_setup.game_style.player_2;

    char *i_p = (game_setup.initial_player == 1) ? p_1_s : p_2_s;
    int p_c = game_setup.player_count;

    char ai_mode[100];
    switch (game_setup.ai_mode)
    {
        case AI_MODE_RANDOM: strcpy(ai_mode, "RANDOM"); break;
        case AI_MODE_MINIMAX: strcpy(ai_mode, "MINIMAX"); break;
        default: strcpy(ai_mode, "ERROR");
    }
    // char *p_1_s = "X";
    // char *p_2_s = "O";

    printf("╔═══════════════════════════════════╗\n");
    printf("║            \033[1mTIC TAC TOE\033[0m            ║\n");
    printf("╠═══════════════════════════════════╣\n");
    printf("║                                   ║\n");

    if (selection[O_M_GAME_STYLE])
    {
        printf("║    >   \033[1mGAME STYLE:        %s|%s\033[0m     ║\n", p_1_s, p_2_s);
    }
    else
    {
        printf("║        GAME STYLE:        %s|%s     ║\n", p_1_s, p_2_s);
    }

    if (selection[O_M_INITIAL_PLAYER])
    {
        if (game_setup.initial_player == PLAYER2)
        {
            printf("║    >   \033[1mINITIAL PLAYER:      %s\033[0m     ║\n", i_p);
        }
        else
        {
            printf("║    >   \033[1mINITIAL PLAYER:    %s\033[0m       ║\n", i_p);
        }
    }
    else
    {
        if (game_setup.initial_player == PLAYER2)
        {
            printf("║        INITIAL PLAYER:      %s     ║\n", i_p);
        }
        else
        {
            printf("║        INITIAL PLAYER:    %s       ║\n", i_p);
        }
    }

    if (selection[O_M_PLAYER_COUNT])
    {
        printf("║    >   \033[1mPLAYER COUNT:      %d\033[0m       ║\n", p_c);
    }
    else
    {
        printf("║        PLAYER COUNT:      %d       ║\n", p_c);
    }

    printf("║");
    if (p_c == 2) printf("\033[2m");
    if (selection[O_M_AI_MODE] && (game_setup.ai_mode == AI_MODE_RANDOM))
    {
        printf("    >   └ \033[1mAI MODE:         %s\033[0m  ║\n", ai_mode);
    }
    else if (selection[O_M_AI_MODE] && (game_setup.ai_mode == AI_MODE_MINIMAX))
    {
        printf("    >   └ \033[1mAI MODE:         %s\033[0m ║\n", ai_mode);
    }
    else if (game_setup.ai_mode == AI_MODE_MINIMAX)
    {
        printf("        └ \033[1mAI MODE:         %s\033[0m ║\n", ai_mode);
    }
    else
    {
        printf("        └ AI MODE:         %s\033[0m  ║\n", ai_mode);
    }
    printf("\033[0m");

    if (selection[O_M_BACK])
    {
        printf("║    >   \033[1mBACK\033[0m                       ║\n");
    }
    else
    {
        printf("║        BACK                       ║\n");
    }

    printf("\033[0m");

    printf("║                                   ║\n");
    printf("╠═══════════════════════════════════╣\n");
    printf("║   (?) Press 'enter' to toggle.    ║\n");
    printf("╚═══════════════════════════════════╝\n");

    printf("\033[0m");
    printf("\033[0m");

    return;
}

void move_cursor_to_field(FIELD field)
{
    int init_x = 14;
    int init_y = 4;

    int x_offset = init_x + (field.x * 4);
    int y_offset = init_y + (field.y * 2);

    fprintf(stderr, "\033[0;0H");

    for (size_t i = 0; i < x_offset; i++)
    {
        fprintf(stderr, "\033[1C");
    }

    for (size_t i = 0; i < y_offset; i++)
    {
        fprintf(stderr, "\033[1B");
    }
    
    return;
}