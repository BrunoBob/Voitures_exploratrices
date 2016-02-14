#ifndef QUEUE
#define QUEUE

#include "../library.h"

typedef struct cell{
	uint8_t* request;
	struct cell* next;
}*Queue;

Queue createQueue();

Queue enqueue(Queue Q, uint8_t* request);

Queue dequeue(Queue Q);

Queue emptyQueue(Queue Q);

void printQueue(Queue Q);

uint8_t* getTop(Queue Q);

#endif
