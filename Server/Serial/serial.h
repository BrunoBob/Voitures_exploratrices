#ifndef SERIAL
#define SERIAL

#include "../library.h"

typedef struct {
	char name[20];
	int speed ;
	int fd ;
	char parity ;
	int stopBit ;
} serial_com;

int open_s(serial_com* sc, char *name);

int read_s(int fd, uint8_t *buffer);

int write_s(int fd, uint8_t *buffer, int nbyte);

int close_s(serial_com* sc);

#endif
