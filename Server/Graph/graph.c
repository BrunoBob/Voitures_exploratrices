#include "graph.h"

Node createNode(int index){
	Node node;
	node = (Node)malloc(sizeof(*node));
	node->index = index;
	return node;
}


Link createLink(int angle){
	Link link;
	link = (Link)malloc(sizeof(*link));
	link->angle = angle;
	return link;
}


void printGraph(Node graph){
	while(graph != NULL){
		printf("Sommet : %d", graph->index);
		Link link = graph->firstLink;
		while(link != NULL){
			printf(" -> %d : angle = %d", link->linked->index, link->angle);
		}
		printf("\n");
	}
}
