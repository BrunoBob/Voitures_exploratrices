#ifndef QUEUE
#define QUEUE

#include "../library.h"

typedef struct queueCell{
	uint8_t* request;
	struct queueCell* next;
}*Queue;

Queue createQueue();

Queue enqueue(Queue Q, uint8_t* request);

Queue dequeue(Queue Q);

Queue emptyQueue(Queue Q);

void printQueue(Queue Q);

uint8_t* getTop(Queue Q);

#endif
