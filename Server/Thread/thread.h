#ifndef THREAD
#define THREAD

#include "../library.h"
#include "../Queue/queue.h"
#include "../Protocol/protocol.h"
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
