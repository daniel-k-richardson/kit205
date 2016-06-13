/*  queue.c
 *  Purpose: used for topological sort of graph.
 *  Author: Daniel Richardson
 *  Date: 15th May 2016
 */

#include "queue.h"

/*  queue_init: creates a queue.
 *
 *  Params: the size the que needs to be.
 *
 *  Returns: an emtpy queue.
 */
Queue queue_init(int queue_size)
{
    Queue new_queue;
    new_queue.elements = malloc(queue_size * sizeof(int));
    new_queue.size = queue_size;
    new_queue.rear = 0;
    new_queue.front = 0;

    return new_queue;
}

/*  insert: inserts an item into the queue.
 *
 *  Params: the queue to add item to and the item to add.
 *
 *  Returns: void.
 */
void insert(Queue *self, int value)
{
    if (self->rear < self->size) {
        *(self->elements + self->front) = value;
        self->front++;
    }
}

/*  remove_from_queue: removes the first added item.
 *
 *  Params: the que to remove item from.
 *
 *  Returns: the item removed from the queue.
 */
int remove_from_queue(Queue *self)
{
    int value_removed;

    if (self->front != self->rear) {
        self->rear++;
        value_removed = self->elements[self->rear - 1];
    }

    return value_removed;
}

/*  display_queue: prints out all the items currently in the queue.
 *
 *  Params: the queue to display content of.
 *
 *  Returns: void.
 */
void display_queue(Queue self)
{
    printf("The current que is: ");
    for (int i = self.rear; i < self.front; i++) {
        printf("%d, ", *(self.elements + i));
    }
    printf(".\n");
}

/*  is_empty: check if the que is empty or not.
 *
 *  Params: queue to check.
 *
 *  Returns: true if empty false aotherwise.
 */
bool is_empty(Queue self)
{
    if (self.front == self.rear) {
        return true;
    }

    return false;
}

void destory_queue(Queue self)
{
	free(self.elements);
}