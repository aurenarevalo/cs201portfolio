
#include <stdlib.h>
// #include "winman.h"
// #include "menumanager.h"
#include "keycodes.h"
#include <ncurses.h>
#include <string.h>
#include <stdio.h>
#include "infection_ds.h"
#include "gamemanager.h"
#include "utility.h"
#include "unistd.h"
#include <signal.h>
#include "scene.h"



/*
* Each node traveled needs to take a specific amount of time.
* will help determining the cost of a path
*
*/

void initialize_ncurses();

void splash(int fade_delay,int print_delay);

short WINCOL;


int main(int argc, char**args){

/*
	initialize_ncurses();
	
	MENU* mm = _new_menu("Play vs. AI","Play locally","Tutorial", "Options","Exit" ,NULL);
	panelList* pl = init_panelList(4);
	pl = add_panel(pl,"menu",15,40,5,5);
	assign_menu(pl,0,&mm);
	assign_menu_subwin(&mm,8,8,2,3);
	box(menu_win(mm),0,0);
	box(menu_sub(mm),0,0);
	// wbkd(menu_win(mm),COLOR_PAIR(2));
	update_panels();
	doupdate();
	post_menu(mm);
	wrefresh(menu_win(mm));
	refresh();


	getch();

	Gr

	refresh();
	endwin();*/
/*
	Graph* g = init_Graph(4);
	new_edge(&g,3,2);
	new_edge(&g,1,2);
	intMatrix* m = construct_adj_matrix(g);
	print_matrix(m);*/
	initialize_ncurses();
	splash(15,75);
	SCENE* mm = init_scene(2,2);
	add_menu(mm,"Play against AI","Play locally","Tutorial","Options","Exit",NULL);
	add_panel(mm,"menu",20, 40, 5, 5);
	set_smenu_win(mm,0,0);
	box(scene_menu_win(mm,0),0,0);
	wbkgd(menu_win(mm->ml->menu_array[0]),COLOR_PAIR(3));
	post_menu(mm->ml->menu_array[0]);
	refresh();
	update_panels();
	doupdate();

	// update_panels();
	// doupdate();
	// post_menu(sc->ml->menu_array[0]);
	// wrefresh(menu_win(sc->ml->menu_array[0]));
	// refresh();

	// getch();

	// refresh();
	// endwin();


	getch();
	refresh();
	endwin();

}


void initialize_ncurses(){
	//initialize ncurses screen, allow color, disable line buffering, no echo of input, no cursor
	initscr();
	start_color();
	cbreak();
	noecho();
	curs_set(false);
	// nodelay(stdscr,TRUE);
	WINCOL = 9;
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


void splash(int fade_delay,int print_delay)
{

	printw("\nPlease note that the windows resize with the terminal,");
	printw(" but some terminal sizes may not be optimal for the game");
	refresh();
	napms(2000);
	clear();
	FILE* f = fopen("splash.txt","r");
	char c;
	char buf[100];
	int i=0;
	while((c=fgetc(f))!=EOF)
	{
		if(c!='\n')
			buf[i++]=c;
		else
		{	
			buf[i]='\n';
			i=0;
			printw("%s",buf);
			napms(print_delay);
			refresh();
			char tst[100] = {'\0'};
			memcpy(buf,tst,sizeof(tst));
		}

	}
	int interval = 800/100;
	int currcolor=800;
	bkgd(COLOR_PAIR(2));
	for(int i = 0; i<100; i++)
	{
		init_color(COLOR_WHITE,currcolor,currcolor,currcolor);
		currcolor-=interval;
		refresh();
		napms(fade_delay);
	}
	init_color(COLOR_WHITE,800,800,800);
	clear();
	napms(1000);
}