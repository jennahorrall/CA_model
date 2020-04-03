/*
 *
 * Main Cellular Automata Function.
 *
 * Acknowledgements:
 * Game of Life transitions - https://natureofcode.com/book/chapter-7-cellular-automata/
 * 
 * Dr. Lam: file "timer.h", also used in p3 to calculate runtimes for specific code segments.
 *
 * Implemented by Paul Bailey, Callan Hand, Jenna Horrall to 
 * model a stochastic sand dune model similar to ReSCAL.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include "queue.h"

#include "timer.h"

int ROWS;
int COLS;

int MAX_ROWS;
int MAX_COLS;

/*global matrix*/
int *cells;

void initialize();
bool transition(int, int);
void print_cellspace(int*, int);

/*
 * Randomly generate a matrix of MAX_ROWS x MAX_COLS.
 * Each cell has two possible states: 0 (inactive) or 1 (active).
 * 
 *
 */
void initialize() {

    srand(time(0));
    for (int x = 0; x < MAX_ROWS; x++) {
        for (int y = 0; y < MAX_COLS; y++) {
               // set to outer layer to all 1's to account for border cell transitions
               if (x == ROWS+1 || x == 0 || y == 0 || y == COLS+1) {
                   *(cells + x*(COLS+2) + y) = 1;
               } else {
                   *(cells + x*(COLS+2) + y) = (rand() % (11 - 10 + 1) + 10) - 10;  
               }
        }
    }

}


/*
 * Returns true if a cell (x,y) in the cells matrix would survive to the next transition.
 *     Doesn't currently adjust any border cells.
 *
 */
bool transition(int x, int y) {

	int livingNeighbors = 0;

	for (int nRows = x - 1; nRows <= x + 1; nRows++) {     //Count Living Neighbors
	    for (int nCols = y - 1; nCols <= y + 1; nCols++) {
		if (*(cells + nRows*MAX_COLS + nCols) == 1) { 
		    livingNeighbors++;
		}
	    }
	}

        //subtract current cell
        livingNeighbors -=  *(cells + x*(MAX_COLS) + y);

	if (*(cells + x*MAX_COLS + y) == 1) {           //Decide if cell will live or perish
	    return (livingNeighbors == 2 || livingNeighbors == 3);
	} else {
	    return (livingNeighbors == 3);
	}

}



/*
 * Print the cellspace.
 *
 * Notice the for loop is from [1, maxrows-1]:
 * this is because of the ghost border.
 */
void print_cellspace(int* p, int timestep) {

    printf("TIMESTEP # %d\n", timestep);
    for (int x = 1; x < MAX_ROWS-1; x++) {
        for (int y = 1; y < MAX_COLS-1; y++) {
            printf(" %d ", *(p + x*(MAX_COLS) + y)) ;
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

    int timestep = 0;
    int* next_transition = (int*) calloc((MAX_ROWS * MAX_COLS), sizeof(int));

    START_TIMER(ca);
    initialize();

    // loop for 50 time steps
    while (timestep <= 50) {
 
       for (int i = 1; i < MAX_ROWS-1; i++) {
           for (int j = 1; j < MAX_COLS-1; j++) {       
               // if cell can move, perform transition else keep previous value
               if (transition(i,j)) {
                   *(next_transition + i*(MAX_COLS) + j) = 1;
	       } else {
                   *(next_transition + i*(MAX_COLS) + j) = 0;
               }
           }
       }

       cells = next_transition;

       // print cellspace every 10 timesteps.
       if (timestep % 10 == 0) {
           //print_cellspace(cells, timestep);
       }
       timestep++;

    }
    STOP_TIMER(ca);

    printf("time for serial program: %4.4fs\n", GET_TIMER(ca));
    free(cells);

    return (EXIT_SUCCESS);
}

