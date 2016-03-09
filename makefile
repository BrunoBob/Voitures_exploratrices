NAME = $(target)
OPT = -Wall -pthread -g
DIR = /home/utilisateur/Bureau/Voitures_exploratrices/Bin
WAY = $(path)
CC = gcc

MAKEFLAGS += --no-print-directory

all : server

binaryAll : binary.sh
	bash binary.sh ./Server

binaryAllD : binary.sh
	bash binary.sh ./Server debug

binary : $(WAY)/$(NAME).h $(WAY)/$(NAME).c
ifeq ($(debug),debug)
	$(CC) $(OPT) -DDEBUG -o $(DIR)/$(NAME).o -c $(WAY)/$(NAME).c
else
	$(CC) $(OPT) -o $(DIR)/$(NAME).o -c $(WAY)/$(NAME).c
endif

Dmain.o : main.c
	$(CC) $(OPT) -DDEBUG -c main.c

main.o : main.c
	$(CC) $(OPT) -c main.c

DEBUG : binaryAllD Dmain.o
	$(CC) $(OPT) -DDEBUG main.o Bin/*.o -o server

server : binaryAll main.o
	$(CC) $(OPT) main.o Bin/*.o -o server

clean :
	rm Bin/*.o server main.o *~
