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

#define GENERATION 10
#define DEFAULT_SIZE 10

int ROWS;
int COLS;

/*matrix*/
int *cells;


/*
 * Randomly generate a matrix of SIZE x SIZE.
 * Each cell has two possible states: 0 (inactive) or 1 (active).
 *
 */
void initialize() {

    for (int x = 0; x < ROWS; x++) {
        for (int y = 0; y < COLS; y++) {
               *(cells + x*COLS + y) = (rand() % (11 - 10 + 1) + 10) - 10;  
        }
    }

}

/*
// TODO: COMPUTE NEXT GENERATION OF CELL
int[][] update() {

    int next_step[SIZE][SIZE] = new int[SIZE][SIZE];

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            next_step[i][j] = check_neighbors(
    
        }

    }


}
*/

/*
 * Print the cellspace.
 */
void print_cellspace() {

    printf("\n");
    for (int x = 0; x < ROWS; x++) {
        for (int y = 0; y < COLS; y++) {
            printf(" %d ", *(cells + x*COLS + y)) ;
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

    cells = (int*) calloc((ROWS * COLS), sizeof(int));

    initialize();

    // max number of timesteps to run for.
    int timestep = 100;
    
    while (timestep > 0) {
   
       //update();

       // print cellspace every 10 timesteps.
       if (timestep % 10 == 0) {
           print_cellspace();
       }
       timestep--;
    }

    free(cells);
    return (EXIT_SUCCESS);
}

