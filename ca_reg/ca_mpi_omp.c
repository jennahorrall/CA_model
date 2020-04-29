/*
 *
 * Main Cellular Automata Function.
 *
 * Acknowledgements:
 * Game of Life transitions - https://natureofcode.com/book/chapter-7-cellular-automata/
 *
 * Dr. Lam: file "timer.h", also used in p3 to calculate runtimes for specific code segments.
 *
 * to run: mpirun -np 4 ./ca_mpi <rows> <cols> <timesteps>
 *
 * Authors: Paul Bailey, Jenna Horrall, Callan Hand
 *
 * ca_mpi: Parallel implementation of the synchronous ca model using MPI. The global
 * matrix is split up among processes and gathered into the global array at the 
 * end of each timestep.
 *
 * ca_mpi_omp: Hybrid parallel implementation of the synchronous ca model using MPI and OpenMP.
 * The global matrix is distributed among procs and within each proc, the updates are
 * parallelized using OpenMP.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include "timer.h"
#include <mpi.h>

#ifdef _OPENMP
#include <omp.h>
#endif

int ROWS;
int COLS;

int MAX_ROWS;
int MAX_COLS;

int my_rank;
int nprocs;

bool debug = false;

int *global_cells;
int *local_cells;

int timesteps;

void initialize();
bool transition(int, int);
void print_cellspace(int*, int, int, int);
void print_full_cellspace(int*, int, int, int);
void ca_routine();
void err_check();

/*
 * Randomly generate a matrix of MAX_ROWS x MAX_COLS.
 * Each cell has two possible states: 0 (inactive) or 1 (active).
 *
 */
void initialize()
{

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
bool transition(int x, int y)
{

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
 * Print the cellspace.
 *
 * Notice the for loop is from [1, maxrows-1]:
 * this is because of the ghost border.
 */
void print_cellspace(int* p, int timestep, int rows, int cols)
{

    printf("TIMESTEP # %d\n", timestep);
    for (int x = 1; x < rows-1; x++) {
        for (int y = 1; y < cols-1; y++) {
            printf(" %d ", *(p + x*(cols) + y)) ;
        }
        printf("\n");
    }
    printf("\n");

}


/*
 * Print the entire cellspace including the ghost border.
 *
 * 
 */
void print_full_cellspace(int* p, int timestep, int rows, int cols)
{

    printf("TIMESTEP # %d\n", timestep);
    for (int x = 0; x < rows; x++) {
        for (int y = 0; y < cols; y++) {
            printf(" %d ", *(p + x*(cols) + y)) ;
        }
        printf("\n");
    }
    printf("\n");

}

/*
 * Perform the main cellular automata transition loop.
 */
void ca_routine()
{

    // distribute cells among procs
    if (MPI_Scatter(global_cells, ((MAX_ROWS / nprocs) * MAX_COLS), MPI_INT,
                    local_cells, ((MAX_ROWS / nprocs) * MAX_COLS), MPI_INT,
                    0, MPI_COMM_WORLD) != MPI_SUCCESS) {
        perror("Scatter error in CA routine");
        exit(1);
    }


    int begin_time = 0;
    int start_rows = 0;
    int end_rows = 0;
    int index_rows_in_local = 0;

    while (begin_time < timesteps) {

        // compute indices in global array
        if (my_rank == 0) {
            start_rows = 1;
            end_rows = MAX_ROWS / nprocs;
            index_rows_in_local = 1;
        } else if (my_rank == nprocs - 1) {
            start_rows = my_rank * (MAX_ROWS / nprocs);
            end_rows = start_rows + (MAX_ROWS / nprocs) - 1;
            index_rows_in_local = 0;
        } else {
            index_rows_in_local = 0;
            start_rows = my_rank * (MAX_ROWS / nprocs);
            end_rows = start_rows + (MAX_ROWS / nprocs);
        }

        # pragma omp parallel for
        for (int i = start_rows; i < end_rows; i++) {
            for (int j = 1; j < MAX_COLS-1; j++) {
                if (transition(i,j)) {
                    *(local_cells + index_rows_in_local*(MAX_COLS) + j) = 1;
                } else {
                    *(local_cells + index_rows_in_local*(MAX_COLS) + j) = 0;
                }

            }
            index_rows_in_local++;
        }

        MPI_Barrier(MPI_COMM_WORLD);

        if (MPI_Allgather(local_cells, ((MAX_ROWS / nprocs) * MAX_COLS), MPI_INT,
                          global_cells, ((MAX_ROWS / nprocs) * MAX_COLS), MPI_INT,
                          MPI_COMM_WORLD) != MPI_SUCCESS) {

            perror("Gather error");
            exit(1);
        }

        // print matrix if debug mode is on
        if (my_rank == 0 && begin_time % 10 == 0 && debug) {
            print_cellspace(global_cells, begin_time, MAX_ROWS, MAX_COLS);
        }

        MPI_Barrier(MPI_COMM_WORLD);

        begin_time++;


    }


}

/*
 * Check to make sure num rows is valid.
 */
void err_check() {

    if ((ROWS+2) % nprocs != 0) {
        if (my_rank == 0) {
            printf("\n------------------\n");
            printf("ERROR: To ensure that the matrix is distributed evenly between the procs, please enter a valid rows amount.\n");
            printf("REASON: Boundary conditions cause a ghost border to be added around the entire matrix, therefore adding two extra rows.\n");
            printf("\n We recommend taking a number that is a multiple of 2 (1024, 2048, etc), and subtract two from that number. Note that any columns number is valid.\n");
            printf("\n------------------\n");
        }
        MPI_Finalize();
        exit(EXIT_SUCCESS);

    }
}



/*
 * Main routine.
 */
int main(int argc, char* argv[])
{
    // check and parse command line options
    if (argc != 4) {
        printf("Usage: ./ca_model <rows> <cols> <timesteps> \n");
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

    // allocate memory for global cell matrix
    global_cells = (int*) calloc((MAX_ROWS * MAX_COLS), sizeof(int));

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    err_check();

    // allocate mem for local arrays
    local_cells = (int*) calloc(((MAX_ROWS / nprocs) * MAX_COLS), sizeof(int));

    // start time and perform main CA loop
    START_TIMER(ca);
    initialize();
    ca_routine();
    MPI_Barrier(MPI_COMM_WORLD);
    STOP_TIMER(ca);

    // clean up, print timing results, and return
    free(local_cells);
    free(global_cells);
    MPI_Finalize();
    if (my_rank == 0) {
        #ifdef _OPENMP
        printf("time for synchronous MPI/OpenMP hybrid program: %4.4fs\n", GET_TIMER(ca));
        #else
        printf("time for synchronous MPI program: %4.4fs\n", GET_TIMER(ca));
        #endif
    }
    return (EXIT_SUCCESS);
}

