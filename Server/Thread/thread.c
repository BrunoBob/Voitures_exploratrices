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
				char* mess = byteToStr(message);
				printf("Serial read : %s\n", mess);
				free(mess);
			#endif

			pthread_mutex_lock(&(dataRead->mutexRead));
			*queueRead = enqueue(*queueRead, message); //Add the message read int the reading queue
			pthread_mutex_unlock(&(dataRead->mutexRead));

			if(*(message+2) < 128){
				uint8_t* confirmation = getConfirmation(message);
				#ifdef DEBUG
					char* mess = byteToStr(message);
					char* conf = byteToStr(confirmation);
					printf("Confirmation of %s = %s\n", mess, conf);
					free(mess);
					free(conf);
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

	while(!*(dataExec->close)){
		//execution of the request on the top of the queue
		if(*queueRead != NULL){
			#ifdef DEBUG
				char* mess = byteToStr(getTop(*queueRead));
				printf("Execution of -> %s\n", mess); //Change to make to execute action
				free(mess);
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
