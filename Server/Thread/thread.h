#ifndef THREAD
#define THREAD

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include "../Queue/queue.h"
#include "../Serial/serial.h"

typedef struct {
	Queue* queueRead;
	Queue* queueWrite;
	int fd;
	int *close;
	pthread_mutex_t mutexRead;
	pthread_mutex_t mutexWrite;
} dataThread;

void *thread_Serial_Read(void *arg);

void *thread_Serial_Write(void *arg);

void *thread_Exec(void *arg);

#endif
