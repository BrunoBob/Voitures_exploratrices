#include "protocol.h"

uint8_t* getConfirmation(uint8_t* message){
	uint8_t* confirmation = (uint8_t*)malloc(MESSAGE_LENGHT);
	*confirmation = *(message+1);
	*(confirmation+1) = *(message);
	*(confirmation+2) = (*(message+2)) + 128;
	*(confirmation+3) = *(message+3);
	return confirmation;
}

void printMessage(uint8_t* message){
	int i;
	for(i = 0 ; i < MESSAGE_LENGHT ; i++){
		printf("%d", *(message+i));
	}
}

char* byteToStr(uint8_t* message){
	char* str = (char*)malloc(3*MESSAGE_LENGHT);
	sprintf(str,"%d%d%d%d", (int)*(message), (int)*(message+1), (int)*(message+2), (int)*(message+3));
	return str;
}
