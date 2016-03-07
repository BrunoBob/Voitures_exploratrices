#ifndef GRAPH
#define GRAPH

#include "../library.h"

typedef struct node{
	uint8_t linkNode[4];
	uint8_t linkTime[4];
	uint8_t numNode;
	struct node* next;
} *Graph;

Graph createGraph();

void removeGraph(Graph graph);

Graph addNode(Graph graph, Graph node);

#endif
