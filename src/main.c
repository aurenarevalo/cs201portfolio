
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
// #include "gamestates.h"


/*
* Each node traveled needs to take a specific amount of time.
* will help determining the cost of a path
*
*/

void initialize_ncurses();
void end_screen();
void splash(int fade_delay,int print_delay);

short WINCOL;
short PLAYER_COLOR,ENEMY_COLOR,NEUTRAL;


int main(int argc, char**args){
	
	initialize_ncurses();
	splash(8,50);

	//MAIN MENU, UNIMPLEMENTED, NOT NEEDED, BUT WAS DESIRED....
	
/*	SCENE* mm = init_scene(2,2);
	add_menu(mm,"Play against AI","Play locally","Tutorial","Options","Exit",NULL);
	add_panel_offset(mm,"menu", 5, 5);
	set_smenu_winsub(mm,0,0,mm->ml->max_len+1,5,5,mm->ml->max_len);
	box(scene_menu_win(mm,0),0,0);
	mvwhline(scene_menu_win(mm,0),2,0,ACS_LTEE,1);
	mvwhline(scene_menu_win(mm,0),2,1,ACS_HLINE,73);
	set_menu_back(mm->ml->menu_array[0],COLOR_PAIR(3));
	wbkgd(menu_win(mm->ml->menu_array[0]),COLOR_PAIR(1));
	post_menu(mm->ml->menu_array[0]);
	

	*/

	//GAMEPLAY



	/* need to rewrite so that generates new windows on different placements.
	The player can switch between windows and take their turn*/


	SCENE* game = init_scene(6,0,2);
	
	
	// add_panel_offset(game,"game grid",6,10);
	add_panel(game,"nav_gamewins",5, 60,19, 11);
	add_panel(game,"main_game",SUB_BOARD_H,SUB_BOARD_W,1,5);
	update_panels();
	refresh();
	doupdate();
	
	bkgd(COLOR_PAIR(4));
	refresh();
	update_panels();

	box(scene_window(game,0),0,0);
	keypad(scene_window(game,1),TRUE);
	keypad(scene_window(game,0),TRUE);

	keypad(stdscr,TRUE);

	update_panels();
	refresh();
	doupdate();
	// USE NEW_GAME state to regenerate a board,  and reset everything. For now
	//Starting on player turn.

	int loop_cond = 0;
	while(!loop_cond)
	{
		gameGrid* gg = generate_gameGrid(game,game->pl->panel_array[1]);
		loop_cond = GAME_LOOP_AI(gg,game);
		delete_gameGrid(gg);
		wclear(panel_window(gg->game_panel));
		wrefresh(panel_window(gg->game_panel));
		clear();
	}
	free(game);
	clear();
	refresh();
	update_panels();
	doupdate();
	end_screen();

	refresh();
	update_panels();
	doupdate();
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

	printw("\nPlease note that window sizes that are not default (24 rows, 80 cols)");
	printw(" Will result in a buggy display. If not this size, fix and rerun.");
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

void end_screen()
{
	nodelay(stdscr,FALSE);
	PANEL* end = new_panel(newwin_offset(5,10));
	mvwprintw(panel_window(end),10,30,"THANKS FOR PLAYING.");
	mvwprintw(panel_window(end),12,30,"PRESS ANY KEY TO EXIT");
	refresh();
	update_panels();
	doupdate();
	getch();
	del_panel(end);
}