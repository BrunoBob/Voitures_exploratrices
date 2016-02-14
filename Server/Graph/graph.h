#ifndef GRAPH
#define GRAPH

#include "../library.h"

typedef struct node1{
	struct node* next;
	struct node2* linked;
	int angle, value;
}*Link;

typedef struct node2{
	struct node2 *next;
	Link firstLink;
	int index;
}*Node;

Node createNode(int index);

Link createLink(int angle);

void printGraph(Node graph);

#endif
