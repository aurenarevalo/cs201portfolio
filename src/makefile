
CC = gcc
CFLAGS = -Wall
USERLIB = "../userlib"
LDFLAGS= -std=c11 -pedantic-errors -lmenu -lncurses -lpanel -lm
OBJFILES = main.o scene.o infection_ds.o utility.o gamemanager.o button.o
TARGET = main


all: $(TARGET) clean
	
$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	rm -f $(OBJFILES) *~