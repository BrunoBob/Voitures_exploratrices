#include "Server/Serial/serial.h"
#include "Server/Queue/queue.h"
#include "Server/Graph/graph.h"
#include "Server/Thread/thread.h"

int main(int argc, char *argv[]){

	#ifdef DEBUG
		printf("Start Server !!!\n");
	#endif

	serial_com serial;

	open_s(&serial, "/dev/ttyACM0") ;

	Queue queueRead = NULL;

	int stop = 0;
	int show = 0;
	int empty = 0;

	dataThread* data = (dataThread*)malloc(sizeof(*data));
	data->queueRead = &queueRead;
	data->fd = serial.fd;
	data->close = &stop;

	pthread_t threadRead;
	pthread_t threadExec;

	#ifdef DEBUG
		printf("Création of lecture thread\n" );
	#endif

	pthread_create(&threadRead, NULL, thread_Serial_Read, data);

	#ifdef DEBUG
		printf("Création of execution thread\n" );
	#endif

	pthread_create(&threadExec, NULL, thread_Exec, data);

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
			printQueue(queueRead);
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

	close_s(&serial) ;

	queueRead = emptyQueue(queueRead);

	#ifdef DEBUG
		printf("End Server !!!\n");
	#endif

	return 0;
}
