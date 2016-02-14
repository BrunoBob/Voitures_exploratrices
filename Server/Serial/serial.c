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

int write_s(serial_com* sc, uint8_t *buffer, int nbyte){
	return write(sc->fd, buffer, nbyte);
}

int read_s(int fd, uint8_t *buffer){
	uint8_t* car = (uint8_t*)malloc(1);
	read(fd,car,1);
	if(*car != '#'){
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
