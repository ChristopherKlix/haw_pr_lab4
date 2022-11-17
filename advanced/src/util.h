#ifndef UTIL_H
#define UTIL_H

#if _WIN32
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#endif

#include <stdbool.h>

void clear_screen(void);

int msleep(long);

// short get_short(char *txt, short min, short max);

// void seed_rand(void);
// long get_rand(short min, short max);

void print_array(int *array, int arr_len);

void rotate_array(int*, int, int, bool);

int mod(int, int);

int max(int x, int y);

int min(int x, int y);


// void print_delimiter(char *delimiter, short count);

// void print_header(char *txt);

// bool number_is_unique(short number, short *array, short arr_len);

// bool number_in_array(short n, short *arr, short arr_len);

#endif