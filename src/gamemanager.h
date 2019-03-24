#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <panel.h>


#include "infection_ds.h"
#include "utility.h"
#include "button.h"
#include "gamestates.h"

#define MAX_UNITS 100
#define MAX_ROWS 15
#define MAX_COLS 15
#define MAX_NODES 20

#define MIN_UNITS 50
#define MIN_ROWS 6
#define MIN_COLS 6
#define MIN_NODES 5

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
	int nodes;
	int n_p1,n_p2,n_neutral; //actually implement this
	intMatrix *parent;
	PANEL* game_panel;
	infectionNode **node;
	Graph* game_graph;
}gameGrid;

gameGrid* init_gameGrid(int r, int c, int nodes,PANEL* game_pan);

infectionNode* init_infectionNode();

void place_node(gameGrid** grid,infectionNode *node);

void set_node_params(infectionNode* node,int contr, int units, int x, int y);

void add_new_node(gameGrid** gg, int contr, int units, int x, int y);

int rng(unsigned int max,unsigned int min);

float find_distance(infectionNode n1, infectionNode n2);

/* function that will need to be implemented, named for ideas */

/*	
*	randomize_adjacency(gameGrid*,...?);
*	players_not_adjacent
*	draw_connections(gameGrid*,) -- calls add_new_node
*/

void add_adjacency(gameGrid** gg, int from, int to);

gameGrid* generate_gameGrid(PANEL* game_pan);

void refresh_nodes(gameGrid** gg);

int is_player_node(gameGrid* gg, int node);

//returns if node is a neutral node or not
int is_neutral_node(gameGrid* gg, int node);


// ALSO used for player2 identification!
int is_enemy_node(gameGrid* gg, int node);

int mk1_check(gameGrid* gg,MEVENT me);

int check_units(gameGrid* gg, int node);

void change_control(gameGrid* gg, int node, int new_control, int overflow);

// void set_units(gameGrid* gg, int node, int units);

int add_units(gameGrid* gg,int node, int units);


int sub_units(gameGrid* gg,int node, int units);
float calc_base_weight(gameGrid *gg,int node);
int calc_attack_AI(gameGrid* gg, int selected);

void GAME_LOOP_AI(gameGrid* gg);

void GAME_LOOP_LOCAL(gameGrid* gg);

