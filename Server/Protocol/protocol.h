#ifndef PROTOCOL
#define PROTOCOL

#include "../library.h"

#define MESSAGE_LENGHT 4

uint8_t* getConfirmation(uint8_t* message);

int isConfirmation(uint8_t* messageWrite, uint8_t* messageRead);

void printMessage(char* info, uint8_t* message);

#endif
