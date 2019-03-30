#include <curses.h>
#include <stdlib.h>

// #include <>
#ifndef BUTTON_H
#define BUTTON_H

typedef struct but{
	int x,y,h,w;
	int xbound[2],ybound[2];
	char* text;
	WINDOW* button_win;
	WINDOW* super;
}BUTTON;


BUTTON* new_button(WINDOW* super, int yrel, int xrel, int h, int w);

void set_text(BUTTON* button, char* text);

WINDOW* button_win(BUTTON* b);

int is_button_press(MEVENT event,BUTTON* but );
#endif