#include "graphTools.h"

/*
 * Main loop :
 * Create Graph
 * Fill the first node manually (TODO)
 * BEGIN LOOP
 *  Choose next node
 *  Send the angle to the robot
 *  Wait The answer then update informations
 *  Add the node to the graph
 * END LOOP
 *
 */
int main(int argc, char **argv){

	Information info;
	Graph graph;
	int i;

	createGraph(&graph);

	for(i=0;i<4;i++){
		graph.tab[0][i].somSucc = -1;
	}
	graph.tab[0][2].somSucc = 1;
	graph.tab[0][2].time = -1;
	graph.visited[0] = 1;
	graph.visited[1] = 1;
	graph.nbSom = 2;

	printGraph(graph);


	return 0;
}
