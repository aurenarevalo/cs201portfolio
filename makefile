
CC = gcc
CFLAGS = -Wall
LDFLAGS= -std=c11 -lmenu -lncurses -lpanel
OBJFILES = main.o menumanager.o winman.o struct.o
TARGET = main

all: $(TARGET) clean

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	rm -f $(OBJFILES) *~