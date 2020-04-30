/* 
 *
 * Main Cellular Automata Function using Pthreads
 *
 * Acknowledgements:
 * Game of Life transitions - https://natureofcode.com/book/chapter-7-cellular-automata/
 * 
 * Dr. Lam: file "timer.h", also used in p3 to calculate runtimes for specific code segments.
 *
 * Authors: Paul Bailey, Jenna Horrall, Callan Hand
 *
 * ca_pthreads: Parallel implementation of the synchronous ca model using pthreads. 
 * The updates of the global cellspace are split among worker threads.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <pthread.h>
#include "timer.h"

int ROWS;
int COLS;
int MAX_ROWS;
int MAX_COLS;

bool debug_mode = false;
int timesteps;

/*global matrix*/
int *cells;
/*buffer matrix*/
int *next_transition;

int thread_count;
pthread_mutex_t nextTran_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_barrier_t barrier_p;

void initialize();
bool transition(int, int);
void print_cellspace(int*, int);
void *worker(void* rank);
void lock(pthread_mutex_t *mut);
void unlock(pthread_mutex_t *mut);
void destroy(pthread_mutex_t *mut);
void barrier_wait(pthread_barrier_t *bar);
void destroy_barrier(pthread_barrier_t *bar);


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
    long thread;
    pthread_t* thread_handles;

    // check and parse command line options
    if (argc != 5) {
        printf("Usage: ./ca_pthreads <rows> <cols> <timesteps> <threads>\n");
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

    cells = (int*) calloc((MAX_ROWS * MAX_COLS), sizeof(int));
    next_transition = (int*) calloc((MAX_ROWS * MAX_COLS), sizeof(int));


    // determine number of threads
    thread_count = strtol(argv[4], NULL, 10);
    if (thread_count < 1) {
        printf("ERROR: thread_count must be greater than 0\n");
        exit(EXIT_FAILURE);
    } else if ((MAX_ROWS % thread_count) != 0) {
	printf("ERROR: please use thread_count that divides Rows\n");
	exit(EXIT_FAILURE);
    }
    thread_handles = malloc (thread_count*sizeof(pthread_t));

    // set up barrier for threads to use between timesteps.
    printf("thread_count : %d\n", thread_count);
    if (pthread_barrier_init(&barrier_p, NULL, thread_count) != 0) {
        printf("ERROR: could not initialize the barrier\n");
        exit(EXIT_FAILURE);
    }

    START_TIMER(ca);
    initialize();
    for (thread = 0; thread < thread_count; thread++) {
        pthread_create(&thread_handles[thread], NULL, worker, (void*) thread);
    }
    for (thread = 0; thread < thread_count; thread++) {
        pthread_join(thread_handles[thread], NULL);
    }
    STOP_TIMER(ca);

    // print results, clean up, and exit
    printf("time for synchronous pthreads program: %4.4fs\n", GET_TIMER(ca));
    free(cells);
    free(thread_handles);
    destroy_barrier(&barrier_p);
    destroy(&nextTran_mutex);    
    return (EXIT_SUCCESS);
	
}


/* ================== THREAD FUNCTION =============== */

void* worker(void* rank) {

    int my_rank;
    my_rank = (long)rank;

    int myStart = (my_rank * (ROWS/thread_count)) + 1;
    int myEnd = myStart + (ROWS/thread_count);
    int timestep = 0;

        
    // loop for x time steps
    while (timestep <= timesteps) {

       for (int i = myStart; i < myEnd; i++) {
           for (int j = 1; j < MAX_COLS-1; j++) {
               // if cell can move, perform transition else keep previous value
               if (transition(i,j)) {
		   *(next_transition + i*(MAX_COLS) + j) = 1;
	       } else {
                   *(next_transition + i*(MAX_COLS) + j) = 0;
               }
    	   }
       }

       // wait for all threads to finish updates
       barrier_wait(&barrier_p);

       // set the cells grid to the next transition
       if (rank == 0) {
	   lock(&nextTran_mutex);
       	   cells = next_transition;
           unlock(&nextTran_mutex);

           if (timestep % 10 == 0 && debug_mode) {
               print_cellspace(cells, timestep);
           }
       }
       timestep++;

    }

    return NULL;
}



/* ================= Function Wrappers =============== */

// lock mutex
void lock(pthread_mutex_t *mut) {
    if (pthread_mutex_lock(mut) != 0) {
        printf("ERROR: could not acquire mutex\n");
        exit(EXIT_FAILURE);
    }
}

// unlock mutex
void unlock(pthread_mutex_t *mut) {
    if (pthread_mutex_unlock(mut) != 0) {
        printf("ERROR: could not acquire mutex\n");
        exit(EXIT_FAILURE);
    }
}

// barrier wait
void barrier_wait(pthread_barrier_t *bar) {
    int returnVal = pthread_barrier_wait(bar);
    if (!(returnVal != 0 || returnVal != PTHREAD_BARRIER_SERIAL_THREAD)) {
        printf("ERROR: could not wait on the barrier\n");
        exit(EXIT_FAILURE);
    }
}

// destroy mutex
void destroy(pthread_mutex_t *mut) {
    if (pthread_mutex_destroy(mut) != 0) {
        printf("ERROR: could not destroy the mutex\n");
        exit(EXIT_FAILURE);
    }
}

// destroy barrier
void destroy_barrier(pthread_barrier_t *bar) {
    if (pthread_barrier_destroy(bar) != 0) {
        printf("ERROR: could not destroy the barrier\n");
        exit(EXIT_FAILURE);
    }
}

