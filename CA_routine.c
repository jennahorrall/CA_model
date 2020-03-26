/*
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE 10
#define GENERATION 10


int cells[SIZE][SIZE];


// INITIALIZE EACH CELL TO 0 OR 1
void initialize() {

    for (int i = 0; i < SIZE; i++) {

        for (int j = 0; j < SIZE; j++) {

               cells[i][j] = (rand() % (11 - 10 + 1) + 10) - 10;  

        }

    }

}
/*
// COMPUTE NEXT GENERATION OF CELL
int[][] update() {

    int next_step[SIZE][SIZE] = new int[SIZE][SIZE];

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            next_step[i][j] = check_neighbors(
    
        }

    }


}
*/


void print() {

    printf("\n");
    for (int x = 0; x < SIZE; x++) {
        for (int y = 0; y < SIZE; y++) {
            printf(" %d ", cells[x][y]);
   
       }
    printf("\n");
    }

}




int main(int argc, char* argv[])
{
    //TODO: READ IN SIZE FROM COMMAND LINE
   // cells = new int[SIZE][SIZE];
    initialize();

    print();
    // clean up and return
    return (EXIT_SUCCESS);
}

