
#include <stdlib.h>
#include "winman.h"
#include "menumanager.h"
#include "keycodes.h"
#include <ncurses.h>
#include <string.h>
#include <stdio.h>
#include "infection_ds.h"
#include "gamemanager.h"
#include "utility.h"
#include "unistd.h"
#include <signal.h>



/*
* Each node traveled needs to take a specific amount of time.
* will help determining the cost of a path
*
*/

void initialize_ncurses();


short WINCOL;


int main(int argc, char**args){

	// initialize_ncurses();
	// int* seed = malloc(sizeof(int));
	// seed =0;

	// int f = 0x0001; 
	// f ^= 0x0004;
	// printf("%d,\n",f);
	gameGrid* gg = generate_gameGrid();
	print_matrix(gg->parent);
	// printf("%d\n",binary_search(arr,10,4));

/*	int idd = getpid();
	int nums[100]= {0};
	int ncount[30] = {0};
	float sum=0;
	for (int i=0; i<100; i++){
		nums[i] = rng(20,5);
		ncount[nums[i]-1] ++;
	}
	for (int j=0; j<30; j++){
		printf("%d occured %d times : %f percent chance\n", j+1, ncount[j], (float)ncount[j]);
		sum+=(float)ncount[j];
	}
	printf("\n %f total",sum);*/
}


void initialize_ncurses(){
	//initialize ncurses screen, allow color, disable line buffering, no echo of input, no cursor
	initscr();
	start_color();
	cbreak();
	noecho();
	curs_set(false);
	nodelay(stdscr,TRUE);
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
