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
#include <stdbool.h>

#define GENERATION 10
#define DEFAULT_SIZE 10

int ROWS;
int COLS;

int MAX_ROWS;
int MAX_COLS;

/*matrix*/
int *cells;

bool transition(int, int);

/*
 * Randomly generate a matrix of ROWS x COLS.
 * Each cell has two possible states: 0 (inactive) or 1 (active).
 * 
 * (this can be parallelized)
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

    // max number of timesteps to run for
    int timestep = 20;
    
    while (timestep > 0) {
   
       // STEP 1: select cell randomly (# between 1 and MAX_ROWS-1)
		// (not currently random, starting out in row major order)
       
       for (int i = 1; i < MAX_ROWS-1; i++) {
           for (int j = 1; j < MAX_COLS-1; j++) {
       
               // STEP 2: determine from transition rule if cell can move using another function - transition()
               if (transition(i,j)) {
	 
	           // STEP 3: if it can move, add it to queue *dont actually move it yet*
 		   // queue.enqueue(*(cells + x*MAX_COLS +y));
	       } 
           }
       }


       //STEP 4: select cell randomly from queue and do the actual transition
               // (this is kinda confusing, how de we select from queue randomly, dont we have to pop the front?)
               // Here is psuedocode for now
       /*
       while (queue is not empty) {
	   cell = queue.dequeue;
           move cell;
       }
       */

       // print cellspace every 10 timesteps.
       if (timestep % 10 == 0) {
           print_cellspace();
       }
       timestep--;
    }

    free(cells);
    return (EXIT_SUCCESS);
}

