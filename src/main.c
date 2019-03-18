
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
#include "button.h"


/*
* Each node traveled needs to take a specific amount of time.
* will help determining the cost of a path
*
*/

void initialize_ncurses();

void splash(int fade_delay,int print_delay);

short WINCOL;
short PLAYER_COLOR,ENEMY_COLOR,NEUTRAL;


int main(int argc, char**args){

/*	Graph* g = init_Graph(4);
	new_edge(&g,3,2);
	new_edge(&g,1,2);
	intMatrix* m = construct_adj_matrix(g);
	print_matrix(m);*/
	initialize_ncurses();
	//splash(15,75);

	//MAIN MENU
/*	SCENE* mm = init_scene(2,2);
	add_menu(mm,"Play against AI","Play locally","Tutorial","Options","Exit",NULL);
	add_panel_offset(mm,"menu", 5, 5);
	set_smenu_winsub(mm,0,0,mm->ml->max_len+1,5,5,mm->ml->max_len);
	box(scene_menu_win(mm,0),0,0);
	mvwhline(scene_menu_win(mm,0),2,0,ACS_LTEE,1);
	mvwhline(scene_menu_win(mm,0),2,1,ACS_HLINE,73);
	set_menu_back(mm->ml->menu_array[0],COLOR_PAIR(3));
	wbkgd(menu_win(mm->ml->menu_array[0]),COLOR_PAIR(1));
	post_menu(mm->ml->menu_array[0]);*/
	
	/*TO DO

	1. Write a menu returning fun
	2. new resize handler
	3. ensure error handling

	*/

	//GAMEPLAY

	SCENE* game = init_scene(2,0);
	add_panel_offset(game,"game grid",5,10);
	gameGrid* gg = generate_gameGrid(game->pl->panel_array[0]);
	box(scene_window(game,0),0,0);
	nodelay(scene_window(game,0),TRUE);
	keypad(scene_window(game,0),TRUE);
	mousemask(ALL_MOUSE_EVENTS,NULL);
	
	int selected_node = -1,dest_node = -1; 

	MEVENT event;
	int ch;	
	while((ch=wgetch(scene_window(game,0)))!=122){
		int mouse_x, mouse_y;
		switch(ch)
		{
			case KEY_MOUSE:
			if(getmouse(&event) == OK)
			{	
				if(selected_node<0){
					if(event.bstate & BUTTON1_PRESSED)
					{
						selected_node = mk1_check(gg,event);
						printw("node %d selected",selected_node);
						// wattron()
						box(button_win(gg->node[selected_node]->node_but),0,0);
					}
				}
				else
				{
					if(event.bstate & BUTTON1_PRESSED)
					{
						if(mk1_check(gg,event) == selected_node){
							printw("**cancelled selection");
							wclear(button_win(gg->node[selected_node]->node_but));
							selected_node=-1;
						}
						else
						{
							/*
							* Logic for sending units
							*/
						}
					}
				}
			}
			break;
		}
		// refresh();

		refresh_nodes(&gg);
		/*for(int i=0; i<gg->nodes-1; i++)
		{
			// printf("%d\n",i);
			// wrefresh(button_win(gg->node[i]->node_but));
		}*/

		update_panels();
		doupdate();
	}

	// SCENE* game = init_scene(s2,2);


	refresh();
	update_panels();
	doupdate();
	// menu_loop(mm,0);
	// getch();
	refresh();
	endwin();
	print_matrix(gg->parent);
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
	PLAYER_COLOR=10;
	ENEMY_COLOR=11;
	NEUTRAL=12;
	init_color(COLOR_BLACK,0,0,0);
	init_color(WINCOL,600,600,600);
	init_color(COLOR_WHITE,800,800,800);
	init_color(PLAYER_COLOR,0,600,600);
	init_color(ENEMY_COLOR,600,600,0);
	init_color(NEUTRAL,600,0,600);
	//Black text, white bg
	init_pair(1,COLOR_BLACK,COLOR_WHITE);
	init_pair(2,COLOR_WHITE,COLOR_BLACK);
	init_pair(3,COLOR_BLACK,WINCOL);
	//color pair 4 is for errors
	init_pair(4,COLOR_WHITE,COLOR_RED);
	init_pair(5,COLOR_WHITE,PLAYER_COLOR);
	init_pair(6,COLOR_WHITE,ENEMY_COLOR);
	init_pair(7,COLOR_WHITE,NEUTRAL);
	// bkgd(COLOR_PAIR(1));
	// nodelay(mm_win,TRUE);
	// nodelay(stdscr,TRUE);
	// keypad(mm_win,TRUE);
	// keypad(stdscr,TRUE);
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
	fclose(f);
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