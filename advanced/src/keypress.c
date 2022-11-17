#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdbool.h>
#include "keypress.h"

char keypress(void)
{
    const bool echo_input = true;
    
    struct termios saved_state;
    struct termios new_state;
    unsigned char echo_bit;

    char c;

    if (-1 == tcgetattr(STDIN_FILENO, &saved_state))
    {
        // error on tcgetattr
        return EOF;
    }

    new_state = saved_state;

    if (echo_input)
    {
        // echo bit set to disable echo
        echo_bit = ECHO;
    }
    else
    {
        // echo bit cleared to enable echo
        echo_bit = 0;
    }

    // use canonical input and set echo. set minimal input to 1.
    new_state.c_lflag &= ~(echo_bit | ICANON);
    new_state.c_cc[VMIN] = 1;

    if (tcsetattr(STDIN_FILENO, TCSANOW, &new_state) == (-1))
    {
        // error on tcsetattr
        return EOF;
    }

    // block (without spinning) until we get a key press
    c = getchar();

    // restore the saved state
    if (tcsetattr(STDIN_FILENO, TCSANOW, &saved_state) == (-1))
    {
        // error on tcsetattr
        return EOF;
    }

    return c;
}
