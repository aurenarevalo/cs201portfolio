#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <panel.h>
#include <limits.h>
#include <ctype.h>

#include "infection_ds.h"
#include "utility.h"
#include "button.h"
#include "gamestates.h"

#define MAX_UNITS 100
// #define MAX_ROWS 7
// #define MAX_COLS 7
#define MAX_NODES 10

#define NODE_H 2
#define NODE_W 3

#define SUB_BOARD_H 16
#define SUB_BOARD_W 69

#define MIN_UNITS 50
// #define MIN_ROWS 5
// #define MIN_COLS 6
#define MIN_NODES 4

/*

		INFECTION GAME GUIDELINES: 
		1. No two player nodes should be directly adjacent! At least 1 neutral node in b/t
		2. Maximum connections is 8! one for each adjacent position on the grid! ( for simplicity)
		3. Each player starts at 50 units! Max is 100 units!
		4. Weight of sending costs should be calculated (ignoring loss of origin units -- add later?)
			based on the DISTANCE to each node, and the UNITS within each node!








*/

typedef struct point{
	int x,y;
}Point;
// 9 for neutral, 1 for connector vert/horiz, 2 for player 1, 3 for player 2/AI, 4 connector horiz.-- int control
typedef struct inode{
	int control;
	int units;
	Point pos;
	BUTTON* node_but;
	// adjNode* anode;
	adjList al;
}infectionNode;

typedef struct ggrid{
	int node_size,nodes;
	int n_p1,n_p2,n_neutral; //actually implement thiss
	// int *snode_winy,*snode_winx;
	int pane_r, pane_c;
	intMatrix *parent;
	PANEL* game_panel;
	// WINDOW* game_pad;
	infectionNode **node;
	Graph* game_graph;
}gameGrid;

typedef struct _gameboard
{
	int grids, curr;
	gameGrid **shown_grid;
}gameBoard;

gameGrid* init_gameGrid(int r, int c, int nodes,PANEL* game_pan);
void delete_gameGrid(gameGrid* gg);

infectionNode* init_infectionNode();

void place_node(gameGrid** grid,infectionNode *node);

void set_node_params(infectionNode* node,int contr, int units, int x, int y);

void add_new_node(gameGrid** gg, int contr, int units, int x, int y);

int rng(unsigned int max,unsigned int min);

float find_distance(infectionNode n1, infectionNode n2);

int gen_board_panes(SCENE* scene, int y_panes, int x_panes);
gameGrid* switch_game_pane(SCENE* scene, gameGrid* gg, int dir);

WINDOW* grid_window(gameGrid* gg);

void add_adjacency(gameGrid** gg, int from, int to);

gameGrid* generate_gameGrid(SCENE* gscene,PANEL* game_pan);

int find_start_node(gameGrid *gg, int win_y, int win_x);

int find_end_node(gameGrid* gg,int snode, int win_y, int win_x);

void refresh_nodes(gameGrid** gg,int win_y, int win_x);

void change_sub_board(gameGrid** gg,int win_y_new, int win_x_new,int old_x, int old_y);

int is_player_node(gameGrid* gg, int node);

//returns if node is a neutral node or not
int is_neutral_node(gameGrid* gg, int node);

// ALSO used for player2 identification!
int is_enemy_node(gameGrid* gg, int node);

int is_node_press(gameGrid* gg,int n, MEVENT me, int win_y, int win_x);

int mc1_check_nodes(gameGrid* gg,MEVENT me,int win_y, int win_x);

int check_units(gameGrid* gg, int node);

void change_control(gameGrid* gg, int node, int new_control, int overflow,int win_y, int win_x);

int add_units(gameGrid* gg,int node, int units);

int sub_units(gameGrid* gg,int node, int units);

int check_win_condition(gameGrid* gg);
float calc_base_weight(gameGrid *gg,int node);
int calc_attack_AI(gameGrid* gg, int selected,int win_y, int win_x);

int GAME_LOOP_AI(gameGrid* gg, SCENE* game_scene);

void GAME_LOOP_LOCAL(gameGrid* gg);

