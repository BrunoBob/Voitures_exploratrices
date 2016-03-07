#include "graph.h"

Graph createGraph(){
	Graph graph = NULL;
	graph = (Graph) malloc(sizeof(*graph));
	return graph;
}

void removeGraph(Graph graph){
	free(graph);
}

Graph addNode(Graph graph, Graph node){


	return graph;
}
