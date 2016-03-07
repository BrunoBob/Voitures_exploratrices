#include "graph.h"

Graph createGraph(){
	Graph graph = NULL;
	graph = (Graph) malloc(sizeof(*graph));
	graph->times = (uint8_t*) malloc(4 * sizeof(uint8_t));
	return graph;
}

void removeGraph(Graph graph){
	free(graph->times);
	free(graph);
}
