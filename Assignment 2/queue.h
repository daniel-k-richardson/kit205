/*  queue.h
*  Purpose: used for topological sort of graph.
*  Author: Daniel Richardson
*  Date: 15th May 2016
*/

#ifndef queue_h
#define queue_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct queue {
    int size;
    int *elements;
    int rear;
    int front;
}Queue;

Queue queue_init(int queue_size);
void insert(Queue *self, int value);
int remove_from_queue(Queue *self);
void display_queue(Queue self);
bool is_empty(Queue self);
void destory_queue(Queue self);


#endif /* queue_h */
