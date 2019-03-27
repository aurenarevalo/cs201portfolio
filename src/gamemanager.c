#include "gamemanager.h"

static unsigned int SD1 = 234234, SD2 = 21456789, SD3 = 0x7f0ce821;

typedef struct point Point;

typedef struct inode infectionNode;

typedef struct ggrid gameGrid;

gameGrid* init_gameGrid(int r, int c, int nodes,PANEL* game_pan)
{
	// printf("\n\n %d ROWS %d COLS %d NODES : %d possible NODES\n\n\n",r,c,nodes,r*c);
	gameGrid* gg = malloc(sizeof(gameGrid));
	gg->parent = init_intMatrix(r,c);
	gg->node = (infectionNode**)malloc(nodes*sizeof(infectionNode*));
	for(int i=0; i<nodes; i++)
		gg->node[i] = init_infectionNode(0,-1);
	gg->nodes = nodes;
	gg->game_panel =game_pan;
	// int padh = (2*r>16) ? 2*r+16 : 16, padw = (3*c>69) ? 3*c+69 : 69;
	// gg->game_pad = newpad(padh,padw);
	gg->game_graph = init_Graph(nodes);
	return gg;

}

infectionNode* init_infectionNode()
{
	infectionNode *node = malloc(sizeof(infectionNode));
	node->control = -1;
	node->units = -1;
	node->pos.x = 0;
	node->pos.y = 0;
	node->node_but = malloc(sizeof(BUTTON));
	node->al.head = NULL;
	return node;
}

void place_node(gameGrid** grid,infectionNode* node)
{
	gameGrid* ggrid = *grid;
	int x,y;
	x = node->pos.x;
	y = node->pos.y;
	ggrid->parent->m[y][x] = node->control;
	// node->node_but = new_button(panel_window(ggrid->game_panel),y*2+1,x*3+1,2,3);
	node->node_but = new_button(panel_window(ggrid->game_panel),y*2+1,x*3+1,2,3);
	if(node->control == 1){
		wbkgd(button_win(node->node_but),COLOR_PAIR(7));
	}
	else if(node->control == 2)
		wbkgd(button_win(node->node_but),COLOR_PAIR(5));
	else if(node->control == 3)
		wbkgd(button_win(node->node_but),COLOR_PAIR(6));
	mvwprintw(button_win(node->node_but),0,0,"%d",node->units);
}

void set_node_params(infectionNode* node,int contr, int units, int x, int y)
{
	node->control = contr;
	node->units = units;
	node->pos.x = x;
	node->pos.y = y;

}
void add_new_node(gameGrid** grid, int contr, int units, int x, int y)
{
	gameGrid *ggrid = *grid;
	ggrid->nodes++;
	ggrid->node = realloc(ggrid->node,ggrid->nodes*sizeof(infectionNode));
	set_node_params(ggrid->node[ggrid->nodes-1],contr,units,x,y);
	place_node(grid,ggrid->node[ggrid->nodes-1]);
}
 /*
 * pseudo rng that seems to maintain a decent distribution. 
 * probably can figure out, mathematically, one that will do better... but whatev
 * inspired by the KISS rng algorithm, but uses time() so the same board wont be generated at startup
 */

int rng(unsigned int max,unsigned int min)
 {	
	unsigned long long x=0;
	unsigned int t = time(NULL);
	int num =-1;
	// unsigned int m = 37;
	while(x==0 || num < min){
		t ^= (t>>2); t ^= (t<<5); t^= (t<<10);
		SD1 ^= (SD1<<3); SD1^= (SD1>>8);
		SD2 ^= (SD2>>5); 
		SD3 ^= (SD3>>8);
		 x = (SD1*SD2 +SD3*SD1 + SD1*124567 + SD3)*t/3;
	 	num = x%(max+1);
	 }
	// if(num < min) return rng(max,min);
	// printf("%u\n",num);	
	return num;
}
/*
* Finds the distance between two nodes. This is to help determine cost of movement.
*/
float find_distance(infectionNode n1, infectionNode n2)
{
	float x1 = n1.pos.x, y1 = n1.pos.y;
	float x2 = n2.pos.x, y2 = n2.pos.y;
	float dx = x2-x1, dy = y2-y1;
	float dist = sqrt(pow(dx,2)+pow(dy,2));
	// printf("%f\n\n", dist);
	return dist;
}
WINDOW* grid_window(gameGrid* gg)
{
	if(gg->game_panel==NULL) return NULL;
	return panel_window(gg->game_panel);
}
/*
* wow, what a function this will be
* thinking about using a tree to store positions. easier to check 
*/

/*void find_adjacencies(gameGrid *gg)
{
	int r = gg->parent->r, c = gg->parent->c;
	int **m = gg->parent->m;
	for(int i=0;i<gg->nodes;i++)
	{
		int y = gg->node[i]->pos.y;
		int x = gg->node[i]->pos.x;
		for(int row=-1; row<2; row++)
		{
			for(int col=-1; col<2; col++){
				int in_bounds = (((col+x) >= 0 && (col+x) <c)
					&& ((row+y) >=0 && (row+y) < r));
				if(in_bounds)
				{
					if(m[row+y][col+x] == 4)
					{
						y=row+y
					}
				}
			}
		}
		
		
	}
}*/

void add_adjacency(gameGrid** gg, int from, int to)
{
	gameGrid* ggt = *gg;
	float dist = find_distance(*ggt->node[from],*ggt->node[to]);
	new_edge(&ggt->game_graph,to,from,dist);
	new_edge(&ggt->game_graph,from,to,dist);
}

int is_pos_available(gameGrid *gg, int x, int y)
{	
	int size = gg->nodes;
	for(int i=0; i<size; i++)
	{
		if(gg->node[i]->pos.x == x && gg->node[i]->pos.y == y) return 0;
	}
	return 1;
}
// void connect_nodes

gameGrid* generate_gameGrid(PANEL* game_pan)
{
	int nrows, ncols, nodes;
	nrows = 6;//rng(MAX_ROWS,MIN_ROWS); //7
	ncols = 6;//rng(MAX_COLS,MIN_COLS); //7
	nodes = 4;//rng(MAX_NODES,MIN_NODES); //4
	
	gameGrid* gg = init_gameGrid(nrows,ncols,nodes,game_pan);
	// scrollok(panel_window(gg->game_panel),TRUE);
	// gg->game_pad = newpad(50,100);
	// PANEL* p = new_panel(gg->game_pad);
	// top_panel();
		// bottom_panel(gg->game_panel);
	// gg->game_pad = newpad(50,100);
	ncols--;nrows--;
	// top_panel(gg->game_panel);
// getch();
	for(int r=0; r<nrows; r++){
		for(int c=0; c<ncols; c++){
			gg->parent->m[r][c] = 9;
		}
	}
	refresh();
	int nrow = rng(ncols,1), ncol =rng(nrows,1);
	// printf("%d %d \n",nrow,ncol);
	set_node_params(gg->node[0],2,50,3,3);
	place_node(&gg,gg->node[0]);
	refresh();
	// top_panel(p);
	update_panels();
	doupdate();
	// doupdate();
	// pnoutrefresh(gg->game_pad,8,15,5,5,6*4,6*5);
	// doupdate();
	// wrefresh(panel_window(gg->game_panel));
	// while(is_pos_available(gg,ncol,nrow)) nrow = rng(ncols,1), ncol = rng(nrow,1);
	// set_node_params(gg->node[1],3,50,nrow,ncol);
	// place_node(&gg,gg->node[1]);
	// add_new_node(&gg,3,50,1,1);
	// add_new_node(&gg,1,50,3,3);
	// add_new_node(&gg,2,50,5,5);
	// add_new_node(&gg,1,50,5,5);
/*	set_node_params(gg->node[0],2,50,5,5);
	place_node(&gg,gg->node[0]);
	set_node_params(gg->node[1],3,50,1,1);
	place_node(&gg,gg->node[1]);*/
/*	for(int i=2; i<nodes; i++)
	{
		int newc = rng(ncols,0),newr = rng(nrows,0);
		while (!is_pos_available(gg,newc,newr)) newc = rng(ncols,0),newr = rng(nrows,0);
		set_node_params(gg->node[i],1,50,newc,newr);
		place_node(&gg,gg->node[i]);
		// printf("%d\n",gg->node[i].pos.x);
	}*/
/*	set_node_params(gg->node[2],1,50,3,3);
	set_node_params(gg->node[3],1,50,5,1);
	place_node(&gg,gg->node[2]);
	place_node(&gg,gg->node[3]);
	add_adjacency(&gg,0,3);
	add_adjacency(&gg,0,2);
	add_adjacency(&gg,2,1);
	add_adjacency(&gg,1,3);
	add_adjacency(&gg,1,2);
	add_adjacency(&gg,2,3);*/
	// add_new_node(&gg,4,50,4,4);
	// add_new_node(&gg,4,50,2,2);
	// find_distance(gg->node[0],gg->node[1]);


	return gg;
}


void refresh_nodes(gameGrid** gg)
{
	gameGrid* ggt= *gg;
	for(int i = 0; i< ggt->nodes; i++)
	{	
		mvwprintw(button_win(ggt->node[i]->node_but),0,0,"%d",ggt->node[i]->units);
		wrefresh(button_win(ggt->node[i]->node_but));
	}
}
//returns if node is a player node or not
int is_player_node(gameGrid* gg, int node)
{
	return (gg->node[node]->control == 2);
}
//returns if node is a neutral node or not
int is_neutral_node(gameGrid* gg, int node)
{
	return (gg->node[node]->control == 1);
}

// ALSO used for player2 identification!
int is_enemy_node(gameGrid* gg, int node)
{
	return (gg->node[node]->control == 3);
}

//Check mouse button 1 click (down) is within bounds of a button. Returns the button node.
int mk1_check(gameGrid* gg,MEVENT me)
{
		for(int i = 0; i< gg->nodes; i++)
		{
			// printw(" - %d  %d ", i,gg->nodes);
			infectionNode* node = gg->node[i];
			if(is_button_press(me,node->node_but))
			{
				// printw(" - %d  %d ", i,gg->nodes);
				// mvwprintw(button_win(gg->node[i]->node_but),0,0,"%d",gg->node[i]->units);
				return i;
			}
		}
	return -1;
}

/*
* Returns value depending on situtation
* If units > 100, returns 1
* if units < 0, returns 2
* else, returns 0
*/
int check_units(gameGrid* gg, int node)
{
	if(gg->node[node]->units >=MAX_UNITS)
	{
		gg->node[node]->units = MAX_UNITS;
		return 1;
	}
	if(gg->node[node]->units <10) wclear(button_win(gg->node[node]->node_but));
	if (gg->node[node]->units <= 0)
		return 2;
	return 0;
}
//changes control of node and sets color accordingly
void change_control(gameGrid* gg, int node, int new_control, int overflow)
{
	gg->node[node]->control = new_control;
	if(gg->node[node]->units < 0) gg->node[node]->units = overflow;
	switch(new_control)
	{
		case 2:
			wbkgd(button_win(gg->node[node]->node_but),COLOR_PAIR(5));
		break;
		case 3:
			wbkgd(button_win(gg->node[node]->node_but),COLOR_PAIR(6));
		break;
	}
	refresh_nodes(&gg);
}

/*void set_units(gameGrid* gg, int node, int units)
{
	gg->
}*/

int add_units(gameGrid* gg, int node, int units)
{
	gg->node[node]->units += units;
	if(gg->node[node]->units > MAX_UNITS) return (gg->node[node]->units - MAX_UNITS);
	return 0;
}

int sub_units(gameGrid* gg,int node, int units)
{
	gg->node[node]->units -= units;
	if(gg->node[node]->units< 0) return (-1*(gg->node[node]->units));
	
	return 0;
}

// if return 1, player win, if 2 enemy win, return 0 else.
int check_win_condition(gameGrid* gg)
{	
	int size = gg->game_graph->vertices;
	int has_enemy=0, has_player=0;
	for(int i=0; i<size; i++)
	{	
		// wprintw(panel_window(gg->game_panel),"---%d--- ",gg->node[i]->control);
		if(gg->node[i]->control == 2) has_player=1;
		if(gg->node[i]->control==3) has_enemy=1;
	}
	// wprintw(panel_window(gg->game_panel),"-.- %d, %d -.-",has_player,has_enemy);
	if(has_player && !has_enemy) return 1;
	if(has_enemy && !has_player) return 2;
	
	return 0;
}

/*enum get_game_state()
{

}*/

int select_node(gameGrid *gg, MEVENT event,int control_select)
{
	
}
#define CTRL_W_P 2
#define CTRL_W_N 1
#define CTRL_W_SLT 4
#define CTRL_W_SGT 10

#define SU_W_GT_MAX 0.5;
#define SU_W_GT_MID 0.75;
#define SU_W_BASE 1;
#define SU_W_LOW 1.5;

#define DIST_W_MULT 3;
/*  WEIGHT TABLE, subject to change
*	is_neutral -> 1 (priority)
*	is_player -> 2
*	is_self(AI) and < 50 units -> 4
*	is_self(AI) and >= 50 units -> 10
*	
*		Self units ...
*	units > 75 -> 0.5;
*	units > 50 -> 0.75;
*	units < 25 -> 1.5;
*
*		Other units ...
*	 units -> units
*
*		Distance  ...
*	turns = Distance/sqrt(8);
*		-> turns*3
*/

// used https://www.thecrazyprogrammer.com/2014/03/dijkstra-algorithm-for-finding-shortest-path-of-a-graph.html
// as reference
int Djikstra(gameGrid *gg,int n,int start)
{
	
	
}


float calc_wieghts_from_node(gameGrid *gg,int node)
{
	
}

int calc_attack_AI(gameGrid* gg, int selected)
{
	if(gg->game_graph->arr[selected].head == NULL) return;
	int V = gg->game_graph->vertices;
	float weight[V];
	float self_unit_weight;
	for(int i=0; i<V; i++)
		weight[i] = 0;
	adjNode *curr = gg->game_graph->arr[selected].head;
	int n=-1;
	while(curr != NULL)
	{
		n=curr->adj;
		int contr = gg->node[n]->control;
		int units = gg->node[n]->units;
		if(contr==1) weight[n] += CTRL_W_N;
		else if(contr==2) weight[n] += CTRL_W_P;
		else if(contr==3 && units < 50) weight[n] += CTRL_W_SLT;
		else if(contr==3 && units >= 50) weight[n] += CTRL_W_SGT;
		weight[n] += gg->node[n]->units/3;
		curr = curr->next;
	}
	int units = gg->node[selected]->units;
	if(units > 75) self_unit_weight = 0.5;
	else if (units > 50) self_unit_weight = 0.75;
	else if (units <= 25) self_unit_weight = 1.5;
	else self_unit_weight = 1;
	int min=weight[0];
	for(int i=1; i<V; i++){ 
		if(min == 0 || weight[i] < min) min =weight[i];
		weight[i] *= self_unit_weight;
		printf("%f\n",weight[i]);
	}
	int minindx = binary_search(weight,V,min);
	// printf("\n%d",minindx);

	return minindx;
	
}
void GAME_LOOP_AI(gameGrid* gg, SCENE* game_scene)
{
	int input;

	GAME_STATES_AI gsai = TURN_PLAYER;
	mousemask(ALL_MOUSE_EVENTS,NULL);
	MEVENT event;

/*	NEW_GAME,
	SELECT_NODE_PLAYER,
	SELECT_NODE_AI,
	WEIGH_OPTIONS_AI,
	TURN_PLAYER,
	TURN_AI,
	SEND_UNITS_PLAYER,
	SEND_UNITS_AI,
	END_TURN,
	PLAYER_WIN,
	AI_WIN*/
	
	/*
	*	AI flow: The AI will select one of its nodes at random,
	*	It will then weigh it's options and proceed to attack
	*
	*/
	
	
	/*
		ADD A CONFIRMATION FOR SENDING UNITS. TELL PLAYER HOW MANY TURNS IT WILL TAKE TO GET THERE.
		IF POSSIBLE ADD VISUAL SHOWING WHERE UNITS ARE.
	*/
	int units = 10;
	int unit_gen = 5;
	
	nodelay(panel_window(gg->game_panel),TRUE);
	
	int selected=-1, dest=-1;
	while((input=wgetch(panel_window(gg->game_panel)))!=122)
	{
		switch(input){
			default:
			case KEY_MOUSE:
			
			if(1){
				switch(gsai)
				{
					case NEW_GAME_AI: //implement regeneration of board
					
					break;
					
					case TURN_PLAYER:
						wclear(stdscr);
						//something useless
						// mvprintw(0,0,"Player turn");
						gsai = SELECT_NODE_PLAYER;
						
					break;
					
					case TURN_AI:
						wclear(stdscr);
						// mvprintw(0,0,"Computer turn");
						gsai=SELECT_NODE_AI;
						
					break;
					
					case SELECT_NODE_PLAYER:
						if(getmouse(&event) == OK)
						{
							selected = mk1_check(gg,event);
							if(!(selected<0))
							{
									
							if(event.bstate & BUTTON1_PRESSED)
							{
								// printw(" oooooo");
								
								
								if(is_player_node(gg,selected))
								{
									box(button_win(gg->node[selected]->node_but),0,0);
									// mvprintw(1,1,"node %d selected",selected);
									units = gg->node[selected]->units/2;
									// mvprintw(1,1,"node %d selected %d",selected, units);
									gsai = SEND_UNITS_PLAYER;
								}
								else{
									// mvprintw(0,0,"Not a player node");
									selected=-1;
								} 
							}
						}
						}
						// gsai = TURN_PLAYER;
					break;
					
					case SELECT_NODE_AI:
						
						while(!is_enemy_node(gg,(selected = rng(gg->nodes-1,0))) && !check_win_condition(gg));
						units = gg->node[selected]->units/2;
						gsai=WEIGH_OPTIONS_AI;
					break;
					
					case WEIGH_OPTIONS_AI:
						printw("AI thinking");
						// napms(2000);
						dest = calc_attack_AI(gg,selected);
						gsai=SEND_UNITS_AI;
					break;
					
					case SEND_UNITS_PLAYER:
						if(getmouse(&event) == OK)
						{
						if(event.bstate & BUTTON1_PRESSED)
						{	
							if((dest=mk1_check(gg,event))+1){
								if(check_units(gg,dest)!=1)
								{
								wclear(button_win(gg->node[selected]->node_but));
								if(dest != selected){
									int extra_sub=0;
									int extra_add=0;
									sub_units(gg,selected,units);
									if(is_neutral_node(gg,dest) || is_enemy_node(gg,dest))extra_sub = sub_units(gg,dest,units);
									if(is_player_node(gg,dest)) extra_add = add_units(gg,dest,units);
									
									gsai = END_TURN_PLAYER;
									if(check_units(gg,dest)==2)
										change_control(gg,dest,2,extra_sub);
									if(check_units(gg,dest) ==1){
										add_units(gg,selected,extra_add);
										gg->node[dest]->units =100;
									}
								}
								else gsai = TURN_PLAYER;
								
								}
								else {
									gsai=TURN_PLAYER;
									wclear(button_win(gg->node[selected]->node_but));
								}
								selected = -1; dest = -1;
							}
						}
						}
						// gsai=TURN_PLAYER;
					break;
					
					case SEND_UNITS_AI:
					
						// int extra_sub=0;
						// int extra_add=0;
						sub_units(gg,selected,units);
						int extra_add, extra_sub;
						if(is_neutral_node(gg,dest) || is_player_node(gg,dest))extra_sub = sub_units(gg,dest,units);
						if(is_enemy_node(gg,dest)) extra_add = add_units(gg,dest,units);
								
						// gsai = END_TURN_PLAYER;
						if(check_units(gg,dest)==2)
							change_control(gg,dest,3,extra_sub);
						if(check_units(gg,dest) ==1){
							add_units(gg,selected,extra_add);
							gg->node[dest]->units =100;
							}
						gsai=END_TURN_AI;
					break;
					
					case END_TURN_PLAYER:
					
					for(int i=0; i<gg->nodes;i++)
						{
							if(is_player_node(gg,i) ) {
								add_units(gg,i,unit_gen);
								check_units(gg,i);
							}
						}
						int win1= check_win_condition(gg);
						if(win1 == 2)
							gsai=AI_WIN;
						else if(win1 == 1)
							gsai=PLAYER_WIN;
						else
							gsai=TURN_AI;
					break;
					case END_TURN_AI:
						for(int i=0; i<gg->nodes;i++)
						{
							if(is_enemy_node(gg,i) ) {
								add_units(gg,i,unit_gen);
								check_units(gg,i);
							}
						}
						int win2= check_win_condition(gg);
						if(win2 == 2)
							gsai=AI_WIN;
						else if(win2 == 1)
							gsai=PLAYER_WIN;
						else
							gsai=TURN_PLAYER;
					break;
					
					case PLAYER_WIN:
						mvprintw(0,0,"PLAYER WIN!");
					break;
					
					case AI_WIN:
						mvprintw(0,0,"AI WIN!");
					break;
				}
			
			}
		
		
			break;
			
			// default:
		
			// break;
		
		}
		refresh_nodes(&gg);
		update_panels();
		doupdate();
		// mvprintw(0,0,"           STATE: %d", gsai);
	}
}


void GAME_LOOP_LOCAL(gameGrid* gg)
{
	int input;


	// GAME_STATES_LOCAL gsl;

	while((input=wgetch(panel_window(gg->game_panel)))!=122)
	{
		switch(input){	
		case KEY_MOUSE:
		/*	switch(gsl)
			{
				
				
				
			}*/
		
		
		
		break;
		
		}
		refresh_nodes(&gg);
		update_panels();
		doupdate();

	}


}