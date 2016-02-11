#ifndef QUEUE
#define QUEUE

#include <stdio.h>
#include <stdlib.h>

typedef struct cell{
	char* request;
	struct cell* next;
}*Queue;

Queue createQueue();

Queue enqueue(Queue Q, char* request);

Queue dequeue(Queue Q);

Queue emptyQueue(Queue Q);

void printQueue(Queue Q);

char* getTop(Queue Q);

#endif
