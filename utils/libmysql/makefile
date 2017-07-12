.PHONY:clean
WORKDIR=.
VPATH = ./src
LIBOBJS=  ulaneMysql.o ulaneDBLog.o  ulaneStmtMysql.o
LIB1 = libulanemysql.so

CC = gcc
CFLGS= -Wall -g -I$(WORKDIR)/inc/ -I/usr/include/mysql/ -L/usr/lib64/mysql/ -lmysqlclient -lstdc++ -lpthread -ldl -lrt -lm
LIBFLAG = -L$(HOME)/lib

all: $(LIB1) 

$(LIB1):$(LIBOBJS)
	$(CC) -shared -fPIC $^ -o $@ 
	cp  $(LIB1) $(HOME)/lib
	mv  $(LIB1) ./lib

.c.o:
	$(CC) $(CFLGS) -c $< -o $@  -shared -fPIC
	
clean:
	rm -rf $(BIN) *.o $(HOME)/lib/$(LIB1) ./$(LIB1) 
		




