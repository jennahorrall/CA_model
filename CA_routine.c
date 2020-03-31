/*
 *
 * Main Cellular Automata Function.
 *
 * Source: The Nature of Code, Daniel Shiffman.
 * Link: https://natureofcode.com/book/chapter-7-cellular-automata/
 *
 * Implemented by Paul Bailey, Callan Hand, Jenna Horrall to 
 * model a stochastic sand dune model similar to ReSCAL.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define GENERATION 10
#define DEFAULT_SIZE 10

int ROWS;
int COLS;

int MAX_ROWS;
int MAX_COLS;

/*matrix*/
int *cells;


/*
 * Randomly generate a matrix of ROWS x COLS.
 * Each cell has two possible states: 0 (inactive) or 1 (active).
 *
 */
void initialize() {

    srand(time(0));
    for (int x = 0; x < MAX_ROWS; x++) {
        for (int y = 0; y < MAX_COLS; y++) {
               // set to outer layer to all 1's to account for border cell transitions
               if (x == ROWS+1 || x == 0 || y == 0 || y == COLS+1) {
                   *(cells + x*(COLS+2) + y) = 26;
               } else {
                   *(cells + x*(COLS+2) + y) = (rand() % (11 - 10 + 1) + 10) - 10;  
               }
        }
    }

}




/*
 * Print the cellspace.
 */
void print_cellspace() {

    printf("\n");
    for (int x = 1; x < MAX_ROWS-1; x++) {
        for (int y = 1; y < MAX_COLS-1; y++) {
            printf(" %d ", *(cells + x*(MAX_COLS) + y)) ;
       }
    printf("\n");
    }
    printf("\n");

}



/*
 * Main routine.
 */
int main(int argc, char* argv[])
{
    // check and parse command line options
    if (argc != 3) {
        printf("Usage: ./ca_model <rows> <cols>\n");
        exit(EXIT_FAILURE);
    }
   
    ROWS = atoi(argv[1]);
    COLS = atoi(argv[2]);

    if (ROWS < 0 || COLS < 0) {
        printf("ERROR: please enter a positive number for rows and cols.\n");
        exit(EXIT_FAILURE);
    }

    /* set max rows/cols for ghost border */
    MAX_ROWS=ROWS+2;
    MAX_COLS=COLS+2;

    cells = (int*) calloc((MAX_ROWS * MAX_COLS), sizeof(int));

    initialize();

    // max number of timesteps to run for.
    int timestep = 20;
    
    while (timestep > 0) {
   
       // step 1: select cell randomly (# between 1 and MAX_ROWS-1)
       // step 2: determine from transition rule if cell can move using another function - transition()
       // step 3: if it can move, add it to queue *dont actually move it yet*
       // step 4: select cell randomly from queue and do the actual transition


       // print cellspace every 10 timesteps.
       if (timestep % 10 == 0) {
           print_cellspace();
       }
       timestep--;
    }

    free(cells);
    return (EXIT_SUCCESS);
}

