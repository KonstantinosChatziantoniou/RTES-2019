CC = /opt/cross-pi-gcc-9.1.0-0/bin/arm-linux-gnueabihf-gcc
CFLAGS =  -Ofast -std=c99 
LIBS =  -pthread -lrt #-lm -Wall -Wextra  
OBJDIR = ./bin
HDRDIR = ./headers
SRCDIR = ./src


##CHANGE HERE 
_OBJ =  main.o AwareNodes.o Message.o MessageBuffer.o Worker.o Server.o Client.o Concurrency.o Logger.o
OBJ = $(patsubst %, $(OBJDIR)/%, $(_OBJ))




##CHANGE HERE
_DEPS = AwareNodes.h Message.h MessageBuffer.h Worker.h Server.h Client.h Concurrency.h Logger.h
DEPS = $(patsubst %, $(HDRDIR)/%, $(_DEPS))


mainProgramArm: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $<  $(CFLAGS)

clean:
	rm -rf ./*.csv


client:
	gcc src/client.c -o client
