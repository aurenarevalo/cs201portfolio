#include "button.h"

typedef struct but BUTTON;


BUTTON* new_button(WINDOW* super, int yrel, int xrel, int h, int w)
{
	int begy,begx;
	getbegyx(super,begy,begx);
	BUTTON* but = malloc(sizeof(BUTTON));
	but->super = super;
	but->y = yrel;
	but->x = xrel;
	but->h = h;
	but->w = w;
	but->ybound[0] = begy+yrel; but->ybound[1] = begy+h+yrel;
	but->xbound[0] = begx+xrel; but->xbound[1] = begx+w+xrel;
	but->button_win = derwin(but->super,h,w,yrel,xrel);
	return but;
}

void set_text(BUTTON* button, char* text)
{
	
}


WINDOW* button_win(BUTTON* b)
{
	return b->button_win;
}

int is_button_press(MEVENT event,BUTTON* but )
{
	int con1,con2;
	con1 = ((event.x >= but->xbound[0]) && (event.x <= but->xbound[1]));
	con2 = ((event.y >= but->ybound[0]) && (event.y <= but->ybound[1]));
	// printw("%d, %d : %d --",event.x,event.y,con1&&con2);
	return con1&&con2;
}

