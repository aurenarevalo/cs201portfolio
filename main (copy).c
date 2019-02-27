
// #include "struct.h"
#include "unistd.h"
#include <stdlib.h>
#include "winman.h"
#include "winman.h"
#include "menumanager.h"
#include "keycodes.h"
#include <ncurses.h>
#include <string.h>
#include <stdio.h>
// #include "struct.h"

void initialize_ncurses();


short WINCOL;

int main(int argc, char**args){

	// int max_x, max_y;
	// initialize main menu options list
	// choiceList* mm_list;
	// mm_list = init_choiceList(2);
	// mm_list = add_choices("Start Game Vs AI","Sart Game vs other player","Tutorial/How To Play","Options",NULL);
	// ITEM** mm_items = (ITEM**)calloc(mm_list->n+1,sizeof(ITEM*));
	// mm = create_menu(mm_list); 
 	// panelList *pans;
	// initialize_ncurses();
	/*panelList* pl = init_panelList(4);
	pl = add_panel(pl,"menu", 10, 10, 10, 10);
	box(panel_window(pl->panel_array[0]),0,0);
	wrefresh(panel_window(pl->panel_array[0]));*/

	// struct pt * p = Point.op_new();
	// if(!p){

	// } else {
	// Point.add_pt(p,0,0);
	// }




/*	update_panels();
	doupdate();
	getch();

	refresh();
	endwin();*/

}


void initialize_ncurses(){
	//initialize ncurses screen, allow color, disable line buffering, no echo of input, no cursor
	initscr();
	start_color();
	cbreak();
	noecho();
	curs_set(false);
	init_color(COLOR_BLACK,0,0,0);
	init_color(WINCOL,600,600,600);
	init_color(COLOR_WHITE,800,800,800);
	//Black text, white bg
	init_pair(1,COLOR_BLACK,COLOR_WHITE);
	init_pair(2,COLOR_WHITE,COLOR_BLACK);
	init_pair(3,COLOR_BLACK,WINCOL);
	//color pair 4 is for errors
	init_pair(4,COLOR_WHITE,COLOR_RED);
	// bkgd(COLOR_PAIR(1));
	// nodelay(mm_win,TRUE);
	// nodelay(stdscr,TRUE);
	// keypad(mm_win,TRUE);
	keypad(stdscr,TRUE);
}
