#include "serial.h"




int open_s(serial_com* sc, char *name){
	struct termios toptions ;

	strcpy(sc->name, name) ;


	#ifdef DEBUG
		printf("Open file : %s\n", sc->name) ;
	#endif

	sc->fd = open(sc->name, O_RDWR) ;

	#ifdef DEBUG
		printf("wait\n") ;
	#endif

	usleep(3000) ;


	tcgetattr(sc->fd, &toptions) ;

	cfsetispeed(&toptions, B9600) ;
	cfsetospeed(&toptions, B9600) ;
	toptions.c_cflag &= ~PARENB ;
	toptions.c_cflag &= ~CSTOPB ;
	toptions.c_cflag &= ~CSIZE;
	toptions.c_cflag |= CS8 ;

	toptions.c_cflag &= ~CRTSCTS;

	tcsetattr(sc->fd, TCSANOW, &toptions) ;

	usleep(300000);

	#ifdef DEBUG
		printf("Serial port open\n") ;
	#endif

	return 1;
}

int write_s(serial_com* sc, char *buffer, int nbyte){
	return write(sc->fd, buffer, nbyte);
}

int read_s(int fd, char *buffer){
	char* car = (char*)malloc(sizeof(char));
	read(fd,car,1);
	if(*car != '#') return 0;
	else{
		int cpt=0;
		while(*car != '!'){
			if(read(fd, car, 1) == 1){
				*(buffer + cpt) = *car;
				cpt++;
			}
		}
		*(buffer + cpt -1) = ' ';
	}
	return 1;
}

int close_s(serial_com* sc){
	#ifdef DEBUG
		printf("Close file : %s\n", sc->name);
	#endif
	return close(sc->fd);
}

void *thread_Serial_Read(void *arg){

	#ifdef DEBUG
		printf("Start the thread read\n" );
	#endif

	dataSerial *dataRead = arg;

	Queue *queue = dataRead->queue;

	tcflush(dataRead->fd, TCIFLUSH);

	while(!*(dataRead->close)){
		char* message = (char*)malloc(20 * sizeof(char));
		if(read_s(dataRead->fd, message) == 1){
			#ifdef DEBUG
				printf("Serial read : %s\n", message);
			#endif

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
