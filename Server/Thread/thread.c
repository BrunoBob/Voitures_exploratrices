#include "thread.h"

void *thread_Serial_Read(void *arg){

	#ifdef DEBUG
		printf("Start the thread read\n" );
	#endif

	dataThread *dataRead = arg;

	Queue *queueRead = dataRead->queueRead;

	tcflush(dataRead->fd, TCIFLUSH);

	char* message = (char*)malloc(20 * sizeof(char));

	while(!*(dataRead->close)){
		if(read_s(dataRead->fd, message) == 1){
			#ifdef DEBUG
				printf("Serial read : %s\n", message);
			#endif

			pthread_mutex_lock(&(dataRead->mutexRead));
			*queueRead = enqueue(*queueRead, message);
			pthread_mutex_unlock(&(dataRead->mutexRead));
			message = (char*)malloc(20 * sizeof(char));
		}
	}

	#ifdef DEBUG
		printf("Close the thread read\n" );
	#endif

	(void) arg;
	pthread_exit(NULL);
}

void *thread_Exec(void *arg){
	#ifdef DEBUG
		printf("Start the thread exec\n" );
	#endif

	dataThread *dataExec = arg;
	Queue *queueRead = dataExec->queueRead;

	while(!*(dataExec->close)){
		//execution of the request on the top of the queue
		if(*queueRead != NULL){
			#ifdef DEBUG
				printf("Execution of -> %s\n", getTop(*queueRead)); //Change to make to execute action
				#endif
				pthread_mutex_lock(&(dataExec->mutexRead));
				*queueRead = dequeue(*queueRead);
				pthread_mutex_unlock(&(dataExec->mutexRead));
				//sleep(3);
		}
	}

	#ifdef DEBUG
		printf("Close the thread exec\n" );
	#endif

	(void) arg;
	pthread_exit(NULL);
}

/*void *thread_Serial_Write(void *arg){

	#ifdef DEBUG
		printf("Start the thread write\n" );
	#endif

	dataSerial *dataWrite = arg;

	Queue *queue = dataWrite->queue;

	tcflush(dataRead->fd, TCIFLUSH);

	while(!*(dataWrite->close)){
		if(queue != NULL){
			write_s
		}

			pthread_mutex_lock(&(dataRead->mutex));
			*queue = enqueue(*queue, message);
			pthread_mutex_unlock(&(dataRead->mutex));
		}
	}

	#ifdef DEBUG
		printf("Close the thread read\n" );
	#endif

	(void) arg;
	pthread_exit(NULL);
}*/
