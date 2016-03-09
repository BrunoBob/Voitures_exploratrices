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
	int i, nextNode, goalNode = -2;
	int list[MAXNODE];

	createGraph(&graph);

	for(i=0;i<4;i++){
		graph.tab[0][i].somSucc = -1;
	}
	graph.tab[0][2].somSucc = 1;
	graph.tab[0][2].time = -1;
	graph.visited[0] = 1;
	graph.nbSom = 2;

	i=0;
	while(goalNode != -1){
		if(list[i] == -1){
			/*First we search the next goal*/
			goalNode = nodeToGo(graph, info.nextNode);

			/*Then we search road to go*/
			list = roadToGo(graph, goalNode, info.nextNode, list);
		}
		serverInformationUpdate(graph, &info, list[i++]);

		/*Next we send datas ie:info.previousAngleTaken to the robot*/


		/*Then we wait new datas send by the robot and addNode to the graph*/


		graph = addNode(&graph, info);

	printGraph(graph);


	return 0;
}
