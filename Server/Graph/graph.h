#ifndef GRAPH
#define GRAPH

#include "../library.h"

typedef struct node{
	uint8_t* link;
	uint8_t index;
	struct node* next;
} *Graph;

Graph createGraph();

void removeGraph(Graph graph);

#endif
