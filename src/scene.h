
#include "keycodes.h"
#include <panel.h>
#include <stdlib.h>
 #include <ncurses.h>
#include <menu.h>
#include <signal.h>
#include <stdio.h>
#include <stdarg.h>
#include "button.h"
// #include "button.h"
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
	int max_len;
	MENU** menu_array;
}menuList;
typedef struct scene{
	int has_menus;
	int bnum;
	size_t bsize;
	BUTTON** buttons;
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
void add_panel_offset(SCENE* scene, char*id, int offy, int offx);
// panelList* add_panel_offset(panelList* pl, char* id, offy,int offx);
// void add_pad_panel(SCENE* scene, char*id, int h, int w);

void create_border(SCENE* scene,int lm, int tm, int rm, int bm);

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



void assign_menu(PANEL* p,MENU** menu);

void assign_menu_sub(MENU** menu,int x,int y, int offy, int offx);

void set_smenu_winsub(SCENE* scene, int mn,int pn, int ypad,int xpad, int offy, int offx);

void set_smenu_win(SCENE* scene, int mn, int pn);

void add_button(SCENE* scene, int pn,int yrel, int xrel, int h, int w);

/* SCENE functions*/

WINDOW* scene_window(SCENE* scene, int pn);

WINDOW* scene_menu_win(SCENE* scene, int mn);

SCENE* init_scene(size_t panels, size_t menus, size_t buttons);

// void new_scene_panel(SCENE* scene);

// void new_scene_menu(SCENE* scene, const char* str,...);

#endif

