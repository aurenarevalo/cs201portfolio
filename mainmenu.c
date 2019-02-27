#include "menumanager.h"
#include "mainmenu.h"



/*
void formatMainMenu(){
	// bgwin = newwinOffset(1,1);
	// wprintw(bgwin,"");
	// wbkgd(bgwin,COLOR_PAIR(3));
	int max_x, max_y;
	mm_win=newwinOffset(10,5);
	getmaxyx(mm_win,max_y,max_x);
	int width = max_x;
	WINDOW* sw = derwin(mm_win, 10, 40, 3, (max_x-2)/3);
	wbkgd(sw,COLOR_PAIR(3));
	mvprintw(0,0, "%d", max_x);
	
	set_menu_win(mm,mm_win);
    set_menu_sub(mm, sw);
	set_menu_mark(mm, " * ");

	set_menu_back(mm,COLOR_PAIR(3));
	// bkgd(COLOR_PAIR(3));
	printw("");
	box(mm_win,0,0);
	// box(sw,0,0);
	mvwprintw(mm_win,1,(max_x-4)/2	,"HELLO");
	mvwaddch(mm_win, 2, 0, ACS_LTEE);
	mvwhline(mm_win, 2, 1, ACS_HLINE, width-2);
	mvwaddch(mm_win, 2, width-1, ACS_RTEE);
	wbkgd(mm_win,COLOR_PAIR(1));	
	wbkgd(sw,COLOR_PAIR(3))
	;

}
*/

