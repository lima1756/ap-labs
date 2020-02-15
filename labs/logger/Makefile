SHELL    = /bin/bash
CC       = gcc
CFLAGS   = -Wall
LDFLAGS  =
OBJFILES = logger.o testLogger.o
TARGET   = testLogger

all: $(TARGET)

$(TARGET) : $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	rm -f $(OBJFILES) $(TARGET) *~

build:
	gcc -c logger.c
	gcc -c testLogger.c 
	gcc -o testLogger logger.o testLogger.o

run: build
	./testLogger

include ../../common.mk
