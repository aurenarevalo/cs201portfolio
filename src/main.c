
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

void splash(int fade_delay,int print_delay);

short WINCOL;
short PLAYER_COLOR,ENEMY_COLOR,NEUTRAL;


int main(int argc, char**args){

	/*Graph* g = init_Graph(4);
	new_edge(&g,3,0);
	new_edge(&g,1,2);
	new_edge(&g,2,3);
	new_edge(&g,0,1);
	intMatrix* m = construct_adj_matrix(g);
	*/
	
	
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



	/* need to rewrite so that generates new windows on different placements.
	The player can switch between windows and take their turn*/


	SCENE* game = init_scene(6,0,2);

	
	// add_panel_offset(game,"game grid",6,10);
	add_panel(game,"nav_gamewins",5, 60,19, 11);
	add_panel(game,"main_game",SUB_BOARD_H,SUB_BOARD_W,1,5);
	update_panels();
	refresh();
	doupdate();

	// add_panel(game,"main_game",16,70,2,5);
	// add_panel_offset(game,"g",12,20);
	
	add_button(game,0,2,3,2,8);
	set_text(game->buttons[0],"left");
	add_button(game,0,2,50,2,8);
	set_text(game->buttons[1],"right");
	add_button(game,0,2,42,2,4);
	set_text(game->buttons[2],"up");
	add_button(game,0,2,15,2,4	);
	set_text(game->buttons[3],"down");
	wbkgd(button_win(game->buttons[0]),COLOR_PAIR(1));
	wbkgd(button_win(game->buttons[1]),COLOR_PAIR(1));
	wbkgd(button_win(game->buttons[2]),COLOR_PAIR(1));
	wbkgd(button_win(game->buttons[3]),COLOR_PAIR(1));
	for(int r = 0; r < 8;r++)
	{
		for(int c = 0; c<23;c++)
		{
			mvwprintw(scene_window(game,1),r*2-1,c*3-1,"|");
			mvwprintw(scene_window(game,1),r*2-1,c*3-1,"|");
		}
	}
	gameGrid* gg = generate_gameGrid(game,game->pl->panel_array[1]);
	bkgd(COLOR_PAIR(4));
	refresh();
	update_panels();

	box(scene_window(game,0),0,0);
	box(scene_window(game,1),0,0);
	// nodelay(scene_window(game,0),TRUE);
	keypad(scene_window(game,1),TRUE);
	keypad(scene_window(game,0),TRUE);
	// wbkgd(grid_window(gg),COLOR_PAIR(4));
	keypad(stdscr,TRUE);
	// prefresh(gg->game_pad,0,0,6,11,18,69);
	// top_panel(game->pl->panel_array[2]);
	
	update_panels();
	refresh();
	doupdate();
	
	// change_sub_board(&gg,1,1,0,0);
	// refresh_nodes(&gg,1,1);
// getch();
	// switch_game_pane(game,gg,1);
	// getch()s;
	// switch_game_pane(game,gg,1);
	// getch();
	// wclear(panel_window(gg->game_panel));
	// update_panels();
	// doupdate();
	// getch();
	// change_sub_board(&gg,1,0,1,1);
	// refresh_nodes(&gg,1,0);
	// getch();
	// switch_game_pane(game,gg,1);
	// mousemask(ALL_MOUSE_EVENTS,NULL);
	
	// int selected_node = -1,dest_node = -1; 

	// MEVENT event;
	// int ch;	
	// USE NEW_GAME state to regenerate a board,  and reset everything. For now
	//Starting on player turn.
	GAME_LOOP_AI(gg,game);
	
	
	
	
	// intMatrix
	// int V=gg->game_graph->vertices;
	// 	intMatrix* adj_mat = construct_adj_matrix(gg->game_graph); 
	// int visited[V];
	// int path[V];
	// for(int i=0; i<V; i++) visited[i]=0,path[i]=0;
	// int* dist_arr= malloc(V*sizeof(int));
	//  DFS(adj_mat,2,visited,path,0);
	

	// free(game);
	// free(gg);

	// SCENE* game = init_scene(s2,2);


	refresh();
	update_panels();
	doupdate();
	getch();
	// menu_loop(mm,0);
	// getch();
	refresh();
	endwin();
	// intMatrix* gm = construct_adj_matrix(gg->game_graph);
	// print_matrix(gm);
	// calc_attack_AI(gg,1);
	// for(int i=0; i<V; i++)
	// 	printf("%d\n",dist_arr[i]);
	// DFS(adj_mat,2,visited,path,0);
	
	// print_matrix(m);
	// printf("\n%d",KEY_ENTER);
	// print_matrix(gg->parent);
	/*for(int i=0; i< game->pl->n;i++)
	{
		printf("%s\n",game->pl->id[i]);
	}
	printf("%d, %d",gg->parent->r, gg->parent->c);*/
	
	// float t = 1.1;

	// printf("\n%lf\n",t);
	// t =  t/10;

	// printf("\n%lf\n",t);
	// int nodeee = find_start_node(gg,1,1 );
	// int edon =  find_end_node(gg,nodeee,1,1);
	// printf("%d p1, %d p2, %d n\n",gg->n_p1,gg->n_p2,gg->n_neutral);
	// printf("%d node %d\n",nodeee,edon);
	
	
	
	Point p;
	p.x = 3,p.y=2;
	if(p.x >= 3 && p.x < 6) printf("yeah\n");
	
	for(int i = 0; i < gg->nodes; i++)
	{
		printf("***%d, %d***\n",gg->node[i]->pos.x,gg->node[i]->pos.y);
		printf("!**%d, %d**!\n",gg->node[i]->node_but->x,gg->node[i]->node_but->y);
		
	}
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