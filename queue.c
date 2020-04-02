#include "queue.h"

/* 
* Queue Implementation 
*/

// creating a queue 
struct queue* initialize_queue()
{
    struct queue* queue = (struct queue*)malloc(sizeof(struct queue));
    queue -> front = queue -> rear = NULL;
    return queue;
}

// seeing if the queue is empty 
bool empty_queue(queue *queue)
{
    return queue->rear == NULL && queue->front == NULL;
}

// helper function for creating a new node 
struct node* new_node(int num)
{
    struct node* tmp = (struct node*)malloc(sizeof(struct node)); 
    tmp->value = num; 
    tmp->next = NULL; 
    return tmp; 
}

// adding an element to the queue 
void enqueue(queue *queue, int num)
{

    struct node *tmp = new_node(num);


  if (queue->rear == NULL) { 
        queue->front = queue->rear = tmp; 
        return; 
    } 
 
    queue->rear->next = tmp; 
    queue->rear = tmp;

    queue->node_count++;
}

// removing an element form the queue 
void dequeue(queue *queue)
{
    if (empty_queue(queue)) {
        printf("ERROR: Attempting to delete an element from an empty queue.\n");
    }

    struct node* tmp = queue->front;

    queue->front = queue->front->next; 

    queue->node_count--;

    free(tmp);

    if (queue->front == NULL) {
        queue->rear = NULL;
    }
}

// printing out the entire queue 
void print_queue(queue *queue)
{
    printf("Queue: ");

    struct node *current_node = queue->front;

    while(current_node != NULL)
    {

        printf("%d ", current_node->value);
        current_node = current_node-> next; 
    }
    
}
