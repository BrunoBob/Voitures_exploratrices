#ifndef PROTOCOL
#define PROTOCOL

#include "../library.h"

#define MESSAGE_LENGHT 4

uint8_t* getConfirmation(uint8_t* message);

void printMessage(uint8_t* message);

char* byteToStr(uint8_t* message);

#endif
