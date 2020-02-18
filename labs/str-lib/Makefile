SHELL    = /bin/bash
CC       = gcc
CFLAGS   = -Wall
LDFLAGS  =
OBJFILES = strlib.o main.o
TARGET   = main

all: $(TARGET)

$(TARGET) : $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	rm -f $(OBJFILES) $(TARGET) *~

build:
	gcc -c strlib.c
	gcc -c main.c
	gcc -o main main.o strlib.o

run: build
	./main originalVeryLongString Addition Add

include ../../common.mk
