/*
 *
 * Main Cellular Automata Function.
 *
 * Acknowledgements:
 * Game of Life transitions - https://natureofcode.com/book/chapter-7-cellular-automata/
 * 
 * Dr. Lam: file "timer.h", also used in p3 to calculate runtimes for specific code segments.
 *
 * Authors: Paul Bailey, Jenna Horrall, Callan Hand
 *
 * ca_serial: Serial implementation of the synchronous ca model. The matrix
 * is updated serially and there is no randomization applied to this method.
 * Cellspace updates are stored in a temp array and updated at the end of
 * each timestep.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

#include "timer.h"

int ROWS;
int COLS;

int MAX_ROWS;
int MAX_COLS;

int timesteps;
bool debug = false;

/*global matrix*/
int *global_cells;
int *next_transition;

void initialize();
bool transition(int, int);
void ca_routine();
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
                   *(global_cells + x*(COLS+2) + y) = 1;
               } else {
                   *(global_cells + x*(COLS+2) + y) = (rand() % (11 - 10 + 1) + 10) - 10;  
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
		if (*(global_cells + nRows*MAX_COLS + nCols) == 1) { 
		    livingNeighbors++;
		}
	    }
	}

        //subtract current cell
        livingNeighbors -=  *(global_cells + x*(MAX_COLS) + y);

	if (*(global_cells + x*MAX_COLS + y) == 1) {           //Decide if cell will live or perish
	    return (livingNeighbors == 2 || livingNeighbors == 3);
	} else {
	    return (livingNeighbors == 3);
	}

}

/*
 * Perform the cellular automata transitions.
 */ 
void ca_routine() {

    int time = 0;
    while (time < timesteps) {
 
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

       global_cells = next_transition;

       // print cellspace every 10 timesteps.
       if (time % 10 == 0 && debug) {
           print_cellspace(global_cells, time);
       }

       time++;

    }

}




/*
 * Print the cellspace.
 *
 * Notice the for loop is from [1, maxrows-1]:
 * this is because of the ghost border.
 */
void print_cellspace(int* p, int timestep) {

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
    if (argc != 4) {
        printf("Usage: ./ca_serial <rows> <cols> <timesteps> \n");
        exit(EXIT_FAILURE);
    }
   
    ROWS = atoi(argv[1]);
    COLS = atoi(argv[2]);
    timesteps = atoi(argv[3]);

    if (ROWS < 0 || COLS < 0) {
        printf("ERROR: please enter a positive number for rows and cols.\n");
        exit(EXIT_FAILURE);
    }

    /* set max rows/cols for ghost border */
    MAX_ROWS=ROWS+2;
    MAX_COLS=COLS+2;

    global_cells = (int*) calloc((MAX_ROWS * MAX_COLS), sizeof(int));
    next_transition = (int*) calloc((MAX_ROWS * MAX_COLS), sizeof(int));

    START_TIMER(ca);
    initialize();
    ca_routine();
    STOP_TIMER(ca);

    printf("time for synchronous serial program: %4.4fs\n", GET_TIMER(ca));
    free(global_cells);
    return (EXIT_SUCCESS);
}

