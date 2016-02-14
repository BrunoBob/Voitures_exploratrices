#include "thread.h"

void *thread_Serial_Read(void *arg){

	#ifdef DEBUG
		printf("Start the thread read\n" );
	#endif

	dataThread *dataRead = arg;

	Queue *queueRead = dataRead->queueRead;
	Queue *queueWrite = dataRead->queueWrite;

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

			if(*(message+2) < 128){
				uint8_t* confirmation = getConfirmation(message);
				#ifdef DEBUG
					printMessage("Create confirmation == ", confirmation);
				#endif
				pthread_mutex_lock(&(dataRead->mutexWrite));
				*queueWrite = enqueue(*queueWrite, confirmation); //Add the message of confirmation in the writing queue
				pthread_mutex_unlock(&(dataRead->mutexWrite));
			}

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
	#ifdef DEBUG
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
	#endif

	(void) arg;
	pthread_exit(NULL);
}

void *thread_Serial_Write(void *arg){

	sleep(2);
	#ifdef DEBUG
		printf("Start the thread write\n" );
	#endif


	dataThread *dataWrite = arg;

	Queue *queueRead = dataWrite->queueRead;
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

			if(*(message+2) < 128){
				while(!isConfirmation(message,getTop(*queueRead)) && !*(dataWrite->close) && *queueRead != NULL);
				pthread_mutex_lock(&(dataWrite->mutexRead));
				*queueRead = dequeue(*queueRead);
				pthread_mutex_unlock(&(dataWrite->mutexRead));
			}
		}
	}

	#ifdef DEBUG
		printf("Close the thread write\n" );
	#endif

	(void) arg;
	pthread_exit(NULL);
}
