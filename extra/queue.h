



#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct node {
    int value;
    struct node *next;
};
typedef struct node node;

// queue structure 
struct queue {

    int node_count;
    node *front;
    node *rear;

};
typedef struct queue queue;

struct queue* initialize_queue();
bool empty_queue(queue *queue);
struct node* new_node(int num);
void enqueue(queue *queue, int num);
void dequeue(queue *queue);
void print_queue(queue *queue);
