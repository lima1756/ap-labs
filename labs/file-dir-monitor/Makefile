CC       = gcc
CFLAGS   = -Wall 
LDFLAGS  = -lpthread
OBJFILES = logger.o monitor.o dictionary.o
TARGET   = monitor

all: $(TARGET)

$(TARGET) : $(OBJFILES)
	$(CC) -g $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	rm -f $(OBJFILES) $(TARGET) *~

include ../../common.mk
