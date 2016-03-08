#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXNODE 20
typedef struct cell{
	int somSucc;
	int time[4];
}*List;

typedef struct{
	int nbSom;
	List *tab[4];
	int *visited;
}Graph;

typedef struct info{
	int time;
	int previousAngleTaken;
	int angles[4];
	int currentNode;
	int nextNode;
}Information;
