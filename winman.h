
// #include "keycodes.h"
#include <panel.h>
#include <stdlib.h>

#ifndef PANEL_STRUCTS
#define PANEL_STRUCTS


 typedef struct win {
	size_t size;
	int n;
	int *row,*col;
	int *x,*y;
	WINDOW** window_array;
}windowList;
typedef struct pan {
	size_t size;
	int n;
	PANEL **panel_array;
	windowList* wl_ref;
	char **id;  // Identifier for panel 
}panelList;
#endif

#ifndef PANEL_FUNS
#define PANEL_FUNS
#include "menumanager.h"

WINDOW* newwin_offset(int offy, int offx);

//SIZE CHECK FUNS
void check_size_panelList(panelList *list);
void check_size_windowList(windowList *list);
void check_size_menuList(menuList *list);


#endif
panelList* init_panelList(size_t size);
panelList* add_panel(panelList* pl,char* id,int y, int x, int starty, int startx);
// panelList* add_panel_offset(panelList* pl, char* id, offy,int offx);

char* get_identifier(panelList* pl, int panel_num);


 windowList* init_windowList(size_t size);

 windowList* add_window(windowList* wl, int h, int w, int y, int x);
