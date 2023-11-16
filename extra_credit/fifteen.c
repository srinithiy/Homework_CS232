/**
 * fifteen.c
 *
 * Refer to Computer Science 50
 * Problem Set 3
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */

#define _XOPEN_SOURCE 500

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define MIN 3
#define MAX 9

// Prototypes
void greet(void);
void clear(void);
void init(int** board, int d);
void draw(int** board, int d);
bool move(int** board, int d, int tile, int* x, int* y);
bool won(int** board, int d);
int get_int(void);

int main(int argc, char* argv[])
{
    greet();
    if (argc != 2)
    {
        printf("Usage: ./fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    int d = atoi(argv[1]);
    if (d < MIN || d > MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            MIN, MIN, MAX, MAX);
        return 2;
    }

    // initialize the board
    int** board = (int**)malloc(d * sizeof(int*));
    for (int i = 0; i < d; i++)
    {
        board[i] = (int*)malloc(d * sizeof(int));
    }

    // initialize x and y here so 0 does not reset every time you call the move function
    int x = d - 1;
    int y = d - 1;

    init(board, d);

    // accept moves until the game is won
    while (true)
    {
        clear();
        draw(board, d);

        // check for win
        if (won(board, d))
        {
            printf("Win!\n");
            break;
        }

        printf("Tile to move: ");
        int tile = get_int();

        // Check move
        if (!move(board, d, tile, &x, &y))
        {
            printf("\nIllegal move.\n");
            usleep(200000);
        }
        usleep(500000);
    }

    // free allocated memory
    for (int i = 0; i < d; i++)
    {
        free(board[i]);
    }
    free(board);

    return 0;
}

// Function to clear screen
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

// Implementations of the missing functions

void init(int** board, int d)
{
    int dsquared = (d * d);
    int n = 0;

    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            n = n + 1;
            board[i][j] = (dsquared - n);
        }
    }
    if ((d % 2) == 0)
    {
        int temp = board[d - 1][d - 2];
        board[d - 1][d - 2] = board[d - 1][d - 3];
        board[d - 1][d - 3] = temp;
    }
}

void draw(int** board, int d)
{
    int n = 0;

    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            n = (n + 1);
            if (board[i][j] > 0)
            {
                printf("| %2d ", board[i][j]);
            }
            if (board[i][j] == 0)
            {
                printf("| __ ");
            }
        }
        printf("|\n\n");
    }
}

bool move(int** board, int d, int tile, int* x, int* y)
{
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (tile == board[i][j])
            {
                int blankspace = 0;
                if (((*x == (i - 1)) && (j == *y)) || ((*x == (i + 1)) && (j == *y)) ||
                    ((i == *x) && (*y == (j - 1))) || ((i == *x) && (*y == (j + 1))))
                {
                    board[*x][*y] = tile;
                    board[i][j] = blankspace;
                    *x = i;
                    *y = j;
                    return true;
                }
            }
        }
    }
    return false;
}

bool won(int** board, int d)
{
    int n = -1;
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            n = n + 1;
            if (board[i][j] != n)
            {
                return false;
            }
        }
    }
    return true;
}

int get_int(void)
{
    int input = 0;
    short invalid = 0;
    char c = getchar();

    if (c == '\n')
        return INT_MAX;

    while (c != '\n')
    {
        if ((c >= '0') && (c <= '9'))
        {
            input = input * 10 + (c - '0');
        }
        else
        {
            invalid = 1;
        }
        c = getchar();
    }

    if (invalid)
        return INT_MAX;
    else
        return input;
}

void greet(void)
{
    clear();
    printf("GAME OF FIFTEEN\n");
    usleep(2000000);
}

