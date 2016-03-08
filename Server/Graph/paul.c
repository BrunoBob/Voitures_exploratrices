#include "paul.h"

/*
 * If the robot takes :
 * the angle 2 : next angles doesn't change
 * The angle 3 : next angles + 1
 * The angle 0 : next angles + 2
 * The angle 1 : next angles + 3
 *
 * This function put a way in the good direction
 */
Information convertAngle(Information info){
	// AbsoluteAngle = (relativeAngle+2)%4;
	int tmp[4];
	int i, add;
	add = (info.previousAngleTaken+2)%4;
	for(i=0;i<4;i++){
		tmp[(i+add)%4] = info.angles[i];
	}
	for(i=0;i<4;i++){
		info.angles[i] = tmp[i];
	}
	return info;
}

/*
 * Setup the graph the first time
 */
void createGraphe(Graph *graph){
	int i=0;
	graph->tab = malloc(MAXNODE*sizeof(List));
	nbSom = 0;
	graph->visited = malloc(MAXNODE*sizeof(int));
	for(i=0;i<MAXNODE;i++)
		graph->visited[i] = 0;
}

/*
 * Add node in graph
 * */
void addNode(Graph *graph, Information info){
	int i,j;
	info = convertAngle(info);
	if(graph->visited[info.currentNode]){
		/*Update the value for the road that we went to take*/
		graph->tab[info.currentNode][info.previousAngleTaken]->time = info.time;
		//The last angle taken become the angle that we arrive
		info.previousAngleTaken = (info.previousAngleTaken+2)%4;

		/*update the road that robot went to take*/
		graph->tab[info.currentNode][info.previousAngleTaken]->time = info.time;
		//graph->tab[info.currentNode][info.previousAngleTaken]->somSucc = info.currentNode;
	}else{
		graph->visited[info.currentNode] = 1;
		for(i=0;i<4;i++){
			/*
			 * For each new road discover, we look if we can add a node
			 * we add him and put his time to 0
			 */
			if(info.angles[i] && graph->nbSom < MAXNODE){
				graph->tab[info.nextNode][i] = graph->nbSom;
				graph->tab[][i]
				graph->nbSom++;
			}
		}
	}
}

void printGraph(Graph graph){
	List list;
	int i=0,j;
	for(i=0;i<graph.nbSom;i++){
		list = graph.tab[i];
		printf("Node i : %d ",i);
		for(j=0;j<4;j++){
			if(list->time > 0){
				printf(" %d",list->somSucc);
			}
		}
		puts("\n");
	}
}

/*
 * Function that calculate what is the angle that robot have to take
 * and return the number of the angle
 */
int goToTheNextNode(Graph *graph, Information info){
	/* If one angle near the current isn't visited we go in
	 * else PP
	 */
			
}
