
#include "keycodes.h"
#include <panel.h>
#include <stdlib.h>
#include <stdbool.h>
 #include <ncurses.h>
#include <menu.h>
#include <signal.h>
#include <stdio.h>
#include <stdarg.h>
// #include "keycodes.h"
// #ifndef WINMANAGER
// #define WINMANAGER
// #include "winman.h"
// #include "menumanager.h"

#ifndef SCENE_H
#define SCENE_H

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
typedef struct choice{
	size_t size;
	int n;
	const char **choice_array;
}choiceList;
typedef struct m{
	size_t size;
	int n;
	MENU** menu_array;
}menuList;
typedef struct scene{
	bool has_menus;
	// char* name;
	panelList* pl;
	menuList* ml;
}SCENE;

WINDOW* newwin_offset(int offy, int offx);

//SIZE CHECK FUNS
void check_size_panelList(panelList *list);
void check_size_windowList(windowList *list);
void check_size_menuList(menuList *list);

char* get_identifier(panelList* pl, int panel_num);
WINDOW* panelList_window(panelList* pl, int indx);
panelList* init_panelList(size_t size);
void add_panel(SCENE* SCENE,char* id,int y, int x, int starty, int startx);

// panelList* add_panel_offset(panelList* pl, char* id, offy,int offx);

char* get_identifier(panelList* pl, int panel_num);

windowList* init_windowList(size_t size);

windowList* add_window(windowList* wl, int h, int w, int y, int x);

choiceList* init_choiceList(size_t n);

choiceList* add_choices( const char *str,...);
/*

menuList* init_menuList(size_t n);

*/
void check_size(choiceList *list);

ITEM** set_items(choiceList *list);

MENU* create_menu(choiceList* mm_list);

MENU* _new_menu(va_list args);

menuList* init_menuList(size_t size);

void add_menu(SCENE* scene,...);

void resize_handler(int sig);

void assign_menu(PANEL* p,MENU** menu);

void assign_menu_sub(MENU** menu,int x,int y, int offy, int offx);

void set_smenu_winsub(SCENE* scene, int mn,int pn,int xpad, int ypad, int offy, int offx);

void set_smenu_win(SCENE* scene, int mn, int pn);

/* SCENE functions*/

WINDOW* scene_window(SCENE* scene, int pn);

WINDOW* scene_menu_win(SCENE* scene, int mn);

SCENE* init_scene(size_t panels, size_t menus);

void new_scene_panel(SCENE* scene);

void new_scene_menu(SCENE* scene, const char* str,...);

void mainMenuLoop();
#endif

