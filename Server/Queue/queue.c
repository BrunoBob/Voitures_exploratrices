#include "queue.h"

Queue createQueue(){
	Queue Q;
	Q = (Queue)malloc(sizeof(*Q));
	return Q;
}

Queue enqueue(Queue Q, uint8_t* request){
	if(Q == NULL){
		Q = createQueue();
		Q->request = request;
	}
	else{
		Queue new = Q;
		while(new->next != NULL){
			new = new->next;
		}
		new->next = createQueue();
		new->next->request = request;
	}
	return Q;
}

Queue dequeue(Queue Q){
	if(Q != NULL){
		Queue temp = Q;
		Q = Q->next;
		temp->next = NULL;
		temp->request = NULL;
		free(temp->request);
		free(temp);
	}
	return Q;
}

Queue emptyQueue(Queue Q){
	while(Q != NULL){
		Q = dequeue(Q);
	}
	return NULL;
}

void printQueue(Queue Q){
	printf("Start liste :\n");
	while(Q != NULL){
		printf("\t%s\n", Q->request);
		Q = Q->next;
	}
	printf("Fin liste\n");
}

uint8_t* getTop(Queue Q){
	if(Q != NULL){
		return Q->request;
	}
	return NULL;
}
