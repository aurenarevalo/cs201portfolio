
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
#include "struct.h"

void initialize_ncurses();


short WINCOL;

int main(int argc, char**args){
	Graph* gr = init_Graph(5);
	new_edge(&gr,1,2);
	new_edge(&gr,1,3);
	int **m = construct_adj_matrix(gr);
	// int **m = calloc(4,sizeof(int*));
	// for(int i=0;i<4;i++)
	// 	m[i] = malloc(sizeof(int));

	for(int r=0; r<4;r++)
	{
		for(int c=0; c<4; c++)
			m[r][c] = 3*c;
	}
	for(int r=0; r<4;r++)
	{
		for(int c=0; c<4; c++)
			printf("%d | ",m[r][c]);
		printf("\n");
	}
}


void initialize_ncurses(){
	//initialize ncurses screen, allow color, disable line buffering, no echo of input, no cursor
	initscr();
	start_color();
	cbreak();
	noecho();
	curs_set(false);
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
