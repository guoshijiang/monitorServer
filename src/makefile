.PHONY:clean all

WORKDIR=.
VPATH = ./src

CC=gcc
CFLGS= -Wall -g -I$(WORKDIR)/inc/
LIBFLAG = -L$(HOME)/lib

BIN = minitorServer

all:$(BIN)

minitorServer:minitorServer.o   minitorUlaneServerOp.o  ulaneLog.o config.o
	$(CC) $(LIBFLAG) $^ -o $@ -lpthread -libulanemysql    
 	
%.o:%.c
	$(CC) $(CFLGS) -c $< -o $@	

clean:
	rm -f *.o $(BIN)
	
	
	




