#include "Server/library.h"
#include "Server/Graph/graph.h"
#include "Server/Thread/thread.h"
#include "Server/Queue/queue.h"

int main(int argc, char *argv[]){

	#ifdef DEBUG
		printf("Start Server !!!\n");
	#endif

	serial_com serial;

	open_s(&serial, "/dev/ttyACM0") ;

	Queue queueRead = NULL;
	Queue queueWrite = NULL;

	int stop = 0;
	int show = 0;
	int empty = 0;

	dataThread* data = (dataThread*)malloc(sizeof(*data));
	data->queueRead = &queueRead;
	data->queueWrite = &queueWrite;
	data->fd = serial.fd;
	data->close = &stop;

	pthread_t threadRead;
	pthread_t threadExec;
	pthread_t threadWrite;
	pthread_t threadMain;

	#ifdef DEBUG
		printf("Création of reading thread\n" );
	#endif

	pthread_create(&threadRead, NULL, thread_Serial_Read, data);

	#ifdef DEBUG
		printf("Création of execution thread\n" );
	#endif

	pthread_create(&threadExec, NULL, thread_Exec, data);

	#ifdef DEBUG
		printf("Création of writing thread\n" );
	#endif

	pthread_create(&threadWrite, NULL, thread_Serial_Write, data);


	#ifdef DEBUG
		printf("Création of main thread\n" );
	#endif

	pthread_create(&threadMain, NULL, thread_main, data);

	char* standardInput = (char*) malloc(4*sizeof(char));

	fd_set readSelect;
	struct timeval selectTime;

	FD_ZERO(&readSelect);
	FD_SET(0, &readSelect);

	selectTime.tv_sec = 0;
	selectTime.tv_usec = 500;

	while(!stop){
		FD_ZERO(&readSelect);
		FD_SET(0, &readSelect);
		switch(select(1, &readSelect, NULL, NULL, &selectTime)){
			case -1 :
				printf("ERREUR SELECT\n");
				break;
			case 1 :
				read(0, standardInput, 4);
				stop = ((strcmp(standardInput, "quit") == 0));
				show = ((strcmp(standardInput, "show") == 0));
				empty = ((strcmp(standardInput, "empt") == 0));
		}
		if(show){
			printf("Read = \n");
			printQueue(queueRead);
			printf("Write = \n");
			printQueue(queueWrite);
			show = 0;
		}
		if(empty){
			queueRead = emptyQueue(queueRead);
			empty = 0;
		}
		sleep(1);
	}

	pthread_join(threadRead, NULL);
	pthread_join(threadExec, NULL);
	pthread_join(threadWrite, NULL);
	pthread_join(threadMain, NULL);

	close_s(&serial) ;

	queueRead = emptyQueue(queueRead);
	queueWrite = emptyQueue(queueWrite);

	#ifdef DEBUG
		printf("End Server !!!\n");
	#endif

	return 0;
}
