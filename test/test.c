#include <curses.h>
#include <menu.h>
#include <string.h>
#include <stdlib.h>
#include <panel.h>


int main()
{	
	
	initscr();
	start_color();
	cbreak();
	// noecho();
	// curs_set(false);
/*	WINDOW* pad;
	refresh();
	pad = newpad(20,20);
	prefresh(pad,0,0,5,5,20,30);
	for(int i=0; i<500; i++){
	pechochar(pad,'g');
	prefresh(pad,0,0,5,5,20,30);
	refresh();*/
// }
    
    
WINDOW* win1, *win2;
PANEL* pan1, *pan2;
    win1 = newwin(10,20,0,0);
    
    refresh();
    // wrefresh(win1);
    pan1 = new_panel(win1);
    win2 = newwin(10,20,0,0);
    pan2 = new_panel(win2);
    
    box(panel_window(pan1),0,0);
    box(panel_window(pan2),0,0);
    
    mvwprintw(panel_window(pan1),4,1,"panel1");
    mvwprintw(panel_window(pan2),4,1,"panel2");
    refresh();
    update_panels();
    getch();
    hide_panel(pan2);
    update_panels();
    getch();
    show_panel(pan2);
    update_panels();
    getch();
// box(pad,0,0);
	// mvwprintw(pad,5,5,"asd");

    
	doupdate();
	
	getch();
	
	endwin();
	
	
}

