#include "thread.h"

void *thread_Serial_Read(void *arg){

	#ifdef DEBUG
		printf("Start the thread read\n" );
	#endif

	dataThread *dataRead = arg;

	Queue *queueRead = dataRead->queueRead;
	//Queue *queueWrite = dataRead->queueWrite;

	tcflush(dataRead->fd, TCIFLUSH);

	uint8_t* message = (uint8_t*)malloc(MESSAGE_LENGHT);

	while(!*(dataRead->close)){
		if(read_s(dataRead->fd, message) == 1){
			#ifdef DEBUG
				printMessage("Serial read : ", message);
			#endif

			pthread_mutex_lock(&(dataRead->mutexRead));
			*queueRead = enqueue(*queueRead, message); //Add the message read int the reading queue
			pthread_mutex_unlock(&(dataRead->mutexRead));

			message = (uint8_t*)malloc(MESSAGE_LENGHT);
		}
	}

	#ifdef DEBUG
		printf("Close the thread read\n" );
	#endif

	(void) arg;
	pthread_exit(NULL);
}

void *thread_Exec(void *arg){
/*	#ifdef DEBUG
		printf("Start the thread exec\n" );
	#endif

	dataThread *dataExec = arg;

	Queue *queueRead = dataExec->queueRead;
	//Queue *queueWrite = dataExec->queueWrite;

	while(!*(dataExec->close)){
		//execution of the request on the top of the queue
		if(*queueRead != NULL && (*(getTop(*queueRead) + 2) < 128)){
			#ifdef DEBUG
				printMessage("Execution of -> ", getTop(*queueRead));
			#endif
			pthread_mutex_lock(&(dataExec->mutexRead));
			*queueRead = dequeue(*queueRead);
			pthread_mutex_unlock(&(dataExec->mutexRead));
		}
	}

	#ifdef DEBUG
		printf("Close the thread exec\n" );
	#endif*/

	(void) arg;
	pthread_exit(NULL);
}

void *thread_Serial_Write(void *arg){

	//sleep(2);
	#ifdef DEBUG
		printf("Start the thread write\n" );
	#endif


	dataThread *dataWrite = arg;

	//Queue *queueRead = dataWrite->queueRead;
	Queue *queueWrite = dataWrite->queueWrite;

	uint8_t* message = NULL;

	while(!*(dataWrite->close)){
		if(*queueWrite != NULL){
			message = getTop(*queueWrite);
			#ifdef DEBUG
				printMessage("writing message >> ", message);
			#endif
			write_s(dataWrite->fd, message, MESSAGE_LENGHT);

			pthread_mutex_lock(&(dataWrite->mutexWrite));
			*queueWrite = dequeue(*queueWrite);
			pthread_mutex_unlock(&(dataWrite->mutexWrite));

		}
	}

	#ifdef DEBUG
		printf("Close the thread write\n" );
	#endif

	(void) arg;
	pthread_exit(NULL);
}

void *thread_main(void* arg){
	#ifdef DEBUG
		printf("Start the thread main\n" );
	#endif


	dataThread *dataMain = arg;

	Queue *queueRead = dataMain->queueRead;
	//Queue *queueWrite = dataMain->queueWrite;


	Information info;
	Graph graph;
	int i, goalNode = -2;
	int list[MAXNODE];
	int configuration;

	createGraph(&graph);

	for(i=0;i<4;i++){
		graph.tab[0][i].somSucc = -1;
	}
	graph.tab[0][2].somSucc = 1;
	graph.tab[0][2].time = -1;
	graph.visited[0] = 1;
	graph.nbSom = 2;

	info.currentNode = 0;
	info.nextNode = 1;
	info.previousAngleTaken = 2;

	while(*queueRead == NULL){}
	uint8_t *request = getTop(*queueRead);
	configuration = request[3];
	recognizeConfiguration(&info, configuration);
	for(i=0;i<4;i++){
		printf("Angle n°%d=%d ; ", i, info.angles[i]);
	}
	pthread_mutex_lock(&(dataMain->mutexRead));
	*queueRead = dequeue(*queueRead);
	pthread_mutex_unlock(&(dataMain->mutexRead));

	graph = addNode(&graph, info);

	i=0;
	while(goalNode != -1){
		if(list[i] == -1){
			/*First we search the next goal*/
			goalNode = nodeToGo(graph, info.nextNode);

			/*Then we search road to go*/
			roadToGo(graph, goalNode, info.nextNode, list);
		}
		serverInformationUpdate(graph, &info, list[i++]);

		/*Next we send datas ie:info.previousAngleTaken to the robot*/

		printInformation(info);

		/*Then we wait new datas send by the robot and addNode to the graph*/

		while(*queueRead == NULL){}
		request = getTop(*queueRead);
		configuration = request[3];
		recognizeConfiguration(&info, configuration);

		pthread_mutex_lock(&(dataMain->mutexRead));
		*queueRead = dequeue(*queueRead);
		pthread_mutex_unlock(&(dataMain->mutexRead));

		graph = addNode(&graph, info);

		printGraph(graph);
	}



	#ifdef DEBUG
		printf("Close the thread main\n" );
	#endif

	(void) arg;
	pthread_exit(NULL);
}
