#include "serial.h"




int open_s(serial_com* sc, char *name){
	struct termios toptions ;

	strcpy(sc->name, name) ;


	#ifdef DEBUG
		printf("Open file : %s\n", sc->name) ;
	#endif

	if((sc->fd = open(sc->name, O_RDWR))== -1){
		printf("Error opening %s\n", sc->name);
		exit(0);
	}

	#ifdef DEBUG
		printf("wait\n") ;
	#endif

	usleep(300000) ;


	tcgetattr(sc->fd, &toptions) ;

	cfsetispeed(&toptions, B9600) ;
	cfsetospeed(&toptions, B9600) ;
	toptions.c_cflag &= ~PARENB ;
	toptions.c_cflag &= ~CSTOPB ;
	toptions.c_cflag &= ~CSIZE;
	toptions.c_cflag |= CS8 ;

	toptions.c_cflag &= ~CRTSCTS;

	tcsetattr(sc->fd, TCSANOW, &toptions) ;

	usleep(30000);

	#ifdef DEBUG
		printf("Serial port open\n") ;
	#endif

	return 1;
}

int write_s(int fd, uint8_t *buffer, int nbyte){
	int val=0;
	val+= write(fd, "#", 1);
	val+= write(fd, buffer, nbyte);
	val+= write(fd, "!", 1);
	return val;
}

int read_s(int fd, uint8_t *buffer){
	uint8_t* car = (uint8_t*)malloc(1);
	if(read(fd,car,0) == -1){
		printf("Erreur reading file");
		return 0;
	}
	if(*car != '#'){
		read(fd,car,1);
		free(car);
		return 0;
	}
	else{
		int cpt=0;
		while(*car != '!' && cpt < MESSAGE_LENGHT){
			if(read(fd, car, 1) == 1){
				*(buffer + cpt) = *car;
				cpt++;
			}
		}
		//*(buffer + cpt -1) = ' ';
	}
	free(car);
	return 1;
}

int close_s(serial_com* sc){
	#ifdef DEBUG
		printf("Close file : %s\n", sc->name);
	#endif
	return close(sc->fd);
}
