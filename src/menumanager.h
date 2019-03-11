#include <ncurses.h>
#include <menu.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "keycodes.h"
// #ifndef WINMANAGER
// #define WINMANAGER
#include "winman.h"
// #endif

#ifndef MENU_STRUCTS
#define MENU_STRUCTS
typedef struct choice{
	size_t size;
	int n;
	const char **choice_array;
}choiceList;

typedef struct m{
	size_t size;
	int n;
	MENU** menu_array;
} menuList;
#endif

#ifndef MENU_FUNS
#define MENU_FUNS

choiceList* init_choiceList(size_t n);

choiceList* add_choices( const char *str,...);
/*

menuList* init_menuList(size_t n);

*/

void check_size(choiceList *list);

ITEM** set_items(choiceList *list);

MENU* create_menu(choiceList* mm_list);

MENU* _new_menu(const char *str,...);

void resize_handler(int sig);

void assign_menu(panelList* pl, int pn,MENU** menu);

void assign_menu_subwin(MENU** menu,int x,int y, int offy, int offx);

void mainMenuLoop();
#endif
