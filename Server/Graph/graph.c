#include "graph.h"

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
	puts("\nformat : Angles :n°[somSucc (timeToGo)]\n");
	for(i=0;i<graph.nbSom;i++){
		printf("Node number : %d\n",i);
		for(j=0;j<4;j++){
			list = graph.tab[i][j];
			printf("\tAngle:%d [ ",j);
			if(list.somSucc >= 0){
				printf("%d (%d)",list.somSucc, list.time);
			}
			puts("]");
		}
		printf("Visited : %d\n\n", graph.visited[i]);
	}
}

void printInformation(Information info){
	printf("The car is in the node %d, will go to the node %d, by the angle %d\n",info.currentNode, info.nextNode, info.previousAngleTaken);
}

/*
 * Function that calculate what is the angle that robot have to take
 * and return the number of the angle
 */
int serverInformationUpdate(Graph graph, Information *info, int nextNode){
	int i;
	/* If one angle near the current isn't visited we go in
	 * else PP*/
	info->currentNode = info->nextNode;
	info->nextNode = nextNode;
	for(i=0;i<4;i++){
		if(graph.tab[info->currentNode][i].somSucc == nextNode){
			info->previousAngleTaken = i;
		}
	}

	if(i == 4){
		perror("Error, the nextNode doesn't exist near to the current");
		exit(1);
	}
	return 0;
}

int nodeToGo(Graph graph, int currentNode){
	int node;
	int i;
		node = -1;
	for(i=0;i<4;i++){
		if(graph.tab[currentNode][i].time == -1 && graph.tab[currentNode][i].somSucc != -1){
			node = graph.tab[currentNode][i].somSucc;
		}
	}
	if(node != -1) return node;
	node = searchUnexplore(graph, currentNode);
	return node;

}

int searchUnexplore(Graph graph, int node){
	int i = 0, j;
	int nodeToGo;
	int flag = 0;
	/*
	 * Pour chaque noeud supérieur on regarde si un noeud n'a pas un chemin non emprunté
	 * */
	for(i=node+1;i<graph.nbSom;i++){
		for(j=0;j<4;j++){
			if(graph.tab[i][j].somSucc != -1 && graph.tab[i][j].time == -1){
				nodeToGo = i;
				j = 4;
				i = graph.nbSom;
				flag = 1;
			}
		}
	}
	for(i=0;i<node;i++){
		for(j=0;j<4;j++){
			if(graph.tab[i][j].somSucc != -1 && graph.tab[i][j].time == -1){
				nodeToGo = i;
				j = 4;
				i = node;
				flag = 1;
			}
		}
	}
	/*Si le flag n'a pas été mis a 1 alors tout le graphe est visité*/
	if(!flag)
		nodeToGo = -1;
	return nodeToGo;
}

int* roadToGo(Graph graph, int node, int currentNode, int *list){
	int i;
	int j;

	for(i=0;i<MAXNODE;i++){
		list[i] = -1;
	}

	for(i=0;i<4;i++){
		if(graph.tab[currentNode][i].somSucc == node){
			list[0] = node;
		}
	}
	if(list[0] == -1){
		while(currentNode != node){
			for(i=0;i<4;i++){
				if(abs(graph.tab[currentNode][i].somSucc - node) < abs(currentNode - node)){
					list[j++] = currentNode;
					currentNode = graph.tab[currentNode][i].somSucc;
					i=4;
				}
			}
		}
	}
	return list;
}
