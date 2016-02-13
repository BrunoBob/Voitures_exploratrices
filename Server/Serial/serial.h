#ifndef SERIAL
#define SERIAL

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <sys/types.h>


typedef struct {
	char name[20];
	int speed ;
	int fd ;
	char parity ;
	int stopBit ;
} serial_com;

int open_s(serial_com* sc, char *name);

int read_s(int fd, char *buffer);

int write_s(serial_com* sc, char *buffer, int nbyte);

int close_s(serial_com* sc);

#endif
