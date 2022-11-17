#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <errno.h>
#include <time.h>

// OS independet terminal clear function
#if _WIN32
void clear_screen(void)
{
    system("cls");
}
#else
void clear_screen(void)
{
    system("clear");
}
#endif


#if _WIN32
int msleep(long msec)
{
    Sleep(msec);
    return 0;
}
#else
int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}
#endif


#define PROGRAM_WIDTH 32

// // util function to ask for user input
// short get_short(char *txt, short min, short max)
// {
//     // declare working variables
//     short value;
//     bool finished = false;
//     char ch;
//     int ret_val;

//     // get user input
//     do
//     {
//         // get user input
//         printf("%s", txt);
//         ch = '\0';
//         ret_val = scanf("%hd%c", &value, &ch);


//         // check for valid user input
//         if (ret_val != 2) printf("Invalid input.\n");
//         else if (ch != '\n') printf("Incorrect scanf behavior\n");
//         else if (value < min) printf("Number out of bounce (min)\n");
//         else if (value > max) printf("Number out of bounce (max)\n");
//         else finished = true;
        
//         // clear input stream buffer
//         while (ch != '\n') scanf("%c", &ch);

//     } while (!finished);
    
//     // return value to caller function
//     return value;
// }

// void seed_rand(void)
// {
//     srand(time(NULL));

//     for (size_t i = 0; i < 100; i++)
//     {
//         long void_num = rand();
//     }
    
//     return;
// }

// long get_rand(short min, short max)
// {
//     return (rand() % max) + min;
// }

// helper function to print array inline
void print_array(int *array, int arr_len)
{
    for (int i = 0; i < arr_len; i++)
    {
        printf("|%3d|", array[i]);
    }

    printf("\n");

    return;
}

void rotate_array(int *arr, int arr_len, int direction, bool loop)
{
    int first_item = arr[0];
    int last_item = arr[arr_len-1];

    if (direction < 0)
    {
        for (size_t i = 0; i < arr_len-1; i++)
        {
            arr[i] = arr[i+1];
        }

        if (loop)
        {
            arr[arr_len-1] = first_item;
        }
        else
        {
            arr[arr_len-1] = 0;
        }
    }
    else
    {
        for (size_t i = arr_len-1; i > 0; i--)
        {
            arr[i] = arr[i-1];
        }

        if (loop)
        {
            arr[0] = last_item;
        }
        else
        {
            arr[0] = 0;
        }
    }

    return;
}

int mod(int x, int y)
{
   int t = x - ((x / y) * y);
   if (t < 0) t += y;
   return t;
}

int max(int x, int y)
{
    return (x > y) ? x : y;
}

int min(int x, int y)
{
    return (x < y) ? x : y;
}

// // print line
// void print_delimiter(char *delimiter, short count)
// {
//     for (size_t i = 0; i < count; i++)
//     {
//         printf("%s", delimiter);
//     }
    
//     printf("\n");

//     return;
// }

// // print a header in default formating
// void print_header(char *txt)
// {
//     size_t str_len = 0;
//     char tmp_chr = '\0';
//     size_t i = 0;

//     do
//     {
//         tmp_chr = txt[i];
//         i++;
//         if (tmp_chr != '\0') str_len++;
//     } while (tmp_chr != '\0');
    
//     const short SPACER_COUNT = round((PROGRAM_WIDTH - str_len) / 2.f);

//     for (size_t i = 0; i < SPACER_COUNT; i++)
//     {
//         printf(" ");
//     }

//     printf("%s", txt);
//     printf("\n");

//     return;
// }

// // check for uniqueness in array
// bool number_is_unique(short number, short *array, short arr_len)
// {
//     // Iterate over array and see if there are matching numbers
//     // If so, then return with false
//     for (int i = 0; i < arr_len; i++)
//     {
//         if (number == array[i])
//         {
//             return false;
//         }
//     }

//     // Since the for-loop completed and found no
//     // matching numbers, we return true
//     return true;
// }

// bool number_in_array(short n, short *arr, short arr_len)
// {
//     for (size_t i = 0; i < arr_len; i++)
//     {
//         if (n == arr[i]) return true;
//     }
    
//     return false;
// }
