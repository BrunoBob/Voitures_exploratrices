#include "graphTools.h"

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
void createGraph(Graph *graph){
	int i=0;
	graph->nbSom = 0;
	graph->visited = malloc(MAXNODE*sizeof(int));
	for(i=0;i<MAXNODE;i++)
		graph->visited[i] = 0;

	graph->tab = malloc(MAXNODE*sizeof(struct cell*));
	for(i=0;i<MAXNODE;i++){
		graph->tab[i] = malloc(4*sizeof(List));
	}
}

/*
 * Add node in graph
 * */
Graph addNode(Graph *graph, Information info){
	int i;
	info = convertAngle(info);
	/*Update the value for the road that we went to take*/
	graph->tab[info.currentNode][info.previousAngleTaken].time = info.time;
		
	//The last angle taken become the angle that we arrive
	info.previousAngleTaken = (info.previousAngleTaken+2)%4;

	/*Update the node that we take*/
	graph->tab[info.nextNode][info.previousAngleTaken].time = info.time;

	if(graph->visited[info.nextNode]){
	}
	else{
		graph->visited[info.nextNode] = 1;
		graph->tab[info.nextNode][info.previousAngleTaken].somSucc = info.currentNode;
		for(i=0;i<4;i++){
			/*
			 * For each new road discover, we look if we can add a node
			 * we add him and put his time to -1
			 */
			if(info.angles[i]){
				graph->tab[info.nextNode][i].somSucc = graph->nbSom;
				graph->tab[info.nextNode][i].time = -1;
				graph->nbSom++;
			}
			else{
				if(i != info.previousAngleTaken){
					graph->tab[info.nextNode][i].somSucc = -1;
				}
			}
		}
	}

	return *graph;
}

void printGraph(Graph graph){
	List list;
	int i=0,j;
	for(i=0;i<graph.nbSom;i++){
		printf("Node number : %d\n",i);
		for(j=0;j<4;j++){
			list = graph.tab[i][j];
			printf("\tAngle:%d [ ",j);
			if(list.somSucc > 0){
				printf("%d ",list.somSucc);
			}
			puts("]");
		}
		puts("\n");
	}
}

/*
 * Function that calculate what is the angle that robot have to take
 * and return the number of the angle
 */
//void goToTheNextNode(Graph *graph, Information *info){
	/* 
	 * If one angle near the current isn't visited we go in
	 * else PP
	 *
	 */
			
//}
