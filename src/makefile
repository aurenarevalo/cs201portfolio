
CC = gcc
CFLAGS = -Wall
USERLIB = "../userlib"
LDFLAGS= -std=c11 -pedantic-errors -lmenu -lncurses -lpanel -lm
OBJFILES = main.o scene.o infection_ds.o utility.o gamemanager.o
TARGET = main


all: $(TARGET) clean
	
$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

# grabdict:
# 	 grep -v "'\|^[A-Z]\|é"  /usr/share/dict/words > "prelim.txt"; 
# 	 grep -x "^..\+."  prelim.txt > "test.txt"; rm prelim.txt
	# find . -path "$(HOME)../" -print 


clean:
	rm -f $(OBJFILES) *~