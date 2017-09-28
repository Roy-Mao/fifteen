/**
 * fifteen.c
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

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    // set all the area which does not belong to the board to -1
    //Then place the corresponding value on the board
    for (int row = 0; row < DIM_MAX; row++)
    {
        for (int column = 0; column < DIM_MAX; column++)
        {
            board[row][column] = -1;
        }
    }
    int max_val = d * d - 1;
    for (int row = 0; row < d; row++)
    {
        for (int column = 0; column < d; column++)
        {
            board[row][column] = max_val;
            max_val--;
        }
    }
    if (d % 2 == 0)
    {
        board[d - 1][d - 2] = 2;
        board[d - 1][d - 3] = 1;
    }
    
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // draw the update board according to the change in move() function
    for (int row = 0; row < d; row++)
    {
        for (int column = 0; column < d; column++)
        {
            if (board[row][column] == 0)
            {
                printf("%4s", "_");
            }
            else
            {
                printf("%4d", board[row][column]);
            }
        }
        printf("\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    // find if 0 is adjacent to tile. if it is, the swap the value
    // becareful value 0 is equal to false in c. 
    // Therefore setting the blankspace to 0 is bit dangerous and difficult to implement;
    for (int row = 0; row < d; row++)
    {
        for (int column = 0; column < d; column++)
        {
            if (tile == board[row][column])
            {
                if (board[row][column - 1] == 0 && column - 1 >= 0)  //if 0 is on the left and within boundary
                {
                    board[row][column] = 0;
                    board[row][column - 1] = tile;
                    return true;
                }
                if (board[row][column + 1] == 0 && column + 1 <= 8) //if 0 is on the right and within boundary
                {
                    board[row][column] = 0;
                    board[row][column + 1] = tile;
                    return true;
                }
                if (board[row + 1][column] == 0 && row + 1 <= 8)  //if 0 is under and within boundary
                {
                    board[row][column] = 0;
                    board[row + 1][column] = tile;
                    return true;
                }
                if (board[row - 1][column] == 0 && row - 1 >= 0) //if 0 is above and within boundary
                {
                    board[row][column] = 0;
                    board[row - 1][column] = tile;
                    return true;
                }
            }
        } 
    }    
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    // TODO
    int num = 0;
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            num++;
            if (i == d - 1 && j == d - 1)
            {
                if (board[i][j] == 0)
                {
                    return true;
                }
            }
            else
            {
                if (board[i][j] != num)
                {
                    return false;
                }
            }
        }
    }
    return false;
}