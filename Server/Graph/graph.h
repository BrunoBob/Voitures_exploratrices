#ifndef GRAPH
#define GRAPH

#include "../library.h"

typedef struct node{
	uint8_t* times;
	uint8_t numNode;
	struct node* next;
} *Graph;

Graph createGraph();

void removeGraph(Graph graph);

#endif
