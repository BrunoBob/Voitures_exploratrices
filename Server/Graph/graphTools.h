#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXNODE 20

typedef struct cell{
	int somSucc;
	int time;
}List;

typedef struct{
	int nbSom;
	List **tab;
	int *visited;
}Graph;

typedef struct info{
	int time;
	int previousAngleTaken;
	int angles[4];
	int currentNode;
	int nextNode;
}Information;

/*
 * convert relative angle in absolute
 */
Information convertAngle(Information);

/*
 * Create a new graph
 */
void createGraph(Graph*);

/*
 * Add a new node, or update 
 */
Graph addNode(Graph *graph, Information info);

/*
 * Print textually the graph
 *
 */
void printGraph(Graph graph);

/*Function to call after serverInformationUpdate for know chat will do the robot*/
void printInformation(Information info);

/*
 * Function that return the angle that robot will take 
 * and update info.currentNode and info.nextNode
 * info.previousAngleTaken by newer datas
 */
int serverInformationUpdate(Graph graph, Information *info, int nextNode);

/*Search the next node that robot will to go*/
int nodeToGo(Graph graph, int currentNode);

/*Search in all the graph for unexplorate ndoe*/
int searchUnexplore(Graph graph, int node);

int* roadToGo(Graph graph, int node, int currentNode, int* list);
