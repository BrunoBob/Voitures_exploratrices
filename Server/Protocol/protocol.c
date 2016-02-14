#include "protocol.h"

uint8_t* getConfirmation(uint8_t* message){
	uint8_t* confirmation = (uint8_t*)malloc(MESSAGE_LENGHT);
	*confirmation = *(message+1);
	*(confirmation+1) = *(message);
	*(confirmation+2) = (*(message+2)) + 128;
	*(confirmation+3) = *(message+3);
	return confirmation;
}

int isConfirmation(uint8_t* messageWrite, uint8_t* messageRead){
	int value = 0;
	if(messageRead != NULL){
		value= ((*(messageWrite) == *(messageRead+1)) && (*(messageWrite+1) == *(messageRead)) &&  (*(messageWrite+2) + 128 == *(messageRead+2)) && (*(messageWrite+3) == *(messageRead+3)));
	}
	return value;
}

void printMessage(char* info, uint8_t* message){
	char str[15];
	sprintf(str,"%d.%d.%d.%d", (int)*(message), (int)*(message+1), (int)*(message+2), (int)*(message+3));
	printf("%s%s\n", info, str);
}
