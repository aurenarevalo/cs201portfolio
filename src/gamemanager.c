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
	unsigned long long x;
	unsigned int t = time(NULL);
	// unsigned int m = 37;
		t ^= (t>>2); t ^= (t<<5); t^= (t<<10);
		SD1 ^= (SD1<<3); SD1^= (SD1>>8);
		SD2 ^= (SD2>>5); 
		SD3 ^= (SD3>>8);
		 x = (SD1*SD2 +SD3*SD1 + SD1*124567 + SD3)*t/3;
	int num = x%(max+1);
	if(num < min) return rng(max,min);
	printf("%u\n",num);	
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
	printf("%f\n\n", dist);
	return dist;
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

// void connect_nodes

gameGrid* generate_gameGrid(PANEL* game_pan)
{
	int nrows, ncols, nodes;
	nrows = 7;//rng(MAX_ROWS,MIN_ROWS);
	ncols = 7;//rng(MAX_COLS,MIN_COLS);
	nodes =4;// rng(MAX_NODES,MIN_NODES);
	gameGrid* gg = init_gameGrid(nrows,ncols,nodes,game_pan);
	ncols--;nrows--;
	for(int r=0; r<=nrows; r++){
		for(int c=0; c<=ncols; c++){
			gg->parent->m[r][c] = 9;
		}
	}
	/*set_node_params(&gg->node[0],2,50,rng(ncols,0),rng(nrows,0));
	place_node(&gg,gg->node[0]);
	set_node_params(&gg->node[1],3,50,rng(ncols,1),rng(nrows,1));
	place_node(&gg,gg->node[1]);*/
	// add_new_node(&gg,3,50,1,1);
	// add_new_node(&gg,1,50,3,3);
	// add_new_node(&gg,2,50,5,5);
	// add_new_node(&gg,1,50,5,5);
	set_node_params(gg->node[0],2,50,5,5);
	place_node(&gg,gg->node[0]);
	set_node_params(gg->node[1],3,50,1,1);
	place_node(&gg,gg->node[1]);
/*	for(int i=0; i<nodes; i++)
	{
		set_node_params(&gg->node[i],1,50,rng(ncols,0),rng(nrows,0));
		place_node(&gg,gg->node[i]);
		printf("%d\n",gg->node[i].pos.x);
	}*/
	set_node_params(gg->node[2],1,50,3,3);
	set_node_params(gg->node[3],2,50,5,1);
	place_node(&gg,gg->node[2]);
	place_node(&gg,gg->node[3]);
	add_adjacency(&gg,0,3);
	add_adjacency(&gg,0,2);
	add_adjacency(&gg,2,1);
	add_adjacency(&gg,1,3);
	add_adjacency(&gg,2,3);
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
			printw(" - %d  %d ", i,gg->nodes);
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
	if (gg->node[node]->units < 0)
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
	
	// if()
}

int sub_units(gameGrid* gg,int node, int units)
{
	gg->node[node]->units -= units;
	if(gg->node[node]->units< 0) return (-1*(gg->node[node]->units));
	
	return 0;
}

void check_win_condition();

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
	int V = gg->game_graph->vertices;
	intMatrix* adj_mat = construct_adj_matrix(gg);
	int visited[V],dist[V][V];
	for(int r=0; r<V; r++)
	{
		for(int c=0; c<V; c++)
		{
			if(adj_mat->m[r][c]==0)
				dist[r][c] = INFINITY;
			else
				dist[r][c] = adj_mat->m[r][c];
				
		}
	}
	
	
	
}


float calc_wieghts_from_node(gameGrid *gg,int node)
{
	int V = gg->game_graph
	int weights[V];
	//initialize weights to the unit value of each node
	for (int i=0; i<V; i++)
		weights[i] = gg->node[i]->weight;
	weights[node]=-1;
	
	for(int i=0; i<V; i++)
	{
		if(i!=selected)
		{
			int contr = gg->node[i]->control;
			if(contr==1) weights[i] += CTRL_W_N;
			else if(contr==2) weights[i] += CTRL_W_P;
			else if(contr==3 && units < 50) weights[i] += CTRL_W_SLT;
			else if(contr==3 && units >= 50) weights[i] += CTRL_W_SGT;
		}
	}
	return 0;
// 	/*/*int contr = gg->node[node]->control;
// 	int units = gg->node[node]->units;
// 	// float dist_weight = gg->game_graph->
// 	adjNode* tmp = gg->game_graph->arr[node].head;
// /*	while(tmp->adj != node){
// 		tmp = tmp->next;
// 		if(tmp->next == NULL) return -1;
// 	}*/
	
// 	float dist_weight =(tmp->weight/(float)sqrt(8));
// 	dist_weight*=DIST_W_MULT;
// 	float contr_weight;
// 	if(contr==1)contr_weight = CTRL_W_N;
// 	else if(contr==2) contr_weight = CTRL_W_P;
// 	else if(contr==3 && units < 50) contr_weight = CTRL_W_SLT;
// 	else if(contr==3 && units >= 50) contr_weight = CTRL_W_SGT;
// 	int V = gg->game_graph->vertices;
// /*	intMatrix* adj_mat = construct_adj_matrix(gg->game_graph); 
// 	int visited[V];
// 	for(int i=0; i<V; i++) visited[i]=0;
// 	calc_total_dist(mat,)*/
	
// 	// int* dist_arr = DFS(adj_mat,node,visited,path,0);
// 	return dist_weight+contr_weight+units;*/*/
	
}

int calc_attack_AI(gameGrid* gg, int selected)
{
	
	int V = gg->game_graph->vertices;
	float weight[V];
	for(int i=0; i<V; i++)
		weight[i] = 0;
	adjNode *curr = gg->game_graph->arr[selected]->head;
	int n=-1;
	while(curr->next != NULL)
	{
		n=curr->adj;
		curr = curr->next;
	}
	// float type_weight;
	// float units_weight;
	// float self_unit_weight;
	// int units = gg->node[selected]->units;
	
	// if(units > 75) self_unit_weight = 0.5;
	// else if (units > 50) self_unit_weight = 0.75;
	// else if (units <= 25) self_unit_weight = 1.5;
	// else self_unit_weight = 1;	
	// float weights[V];
	
	// intMatrix* adj_mat = construct_adj_matrix(gg->game_graph); 
	// int visited[V];
	// int path[V];
	// for(int i=0; i<V; i++) visited[i]=0,path[i]=0;
	// int* dist_arr = DFS(adj_mat,selected,visited,path,0);
	// calc_total_dist(mat,)
	
/*	for(int i = 0; i<V; i++)
	{
		if(i==selected) weights[i] = 0;
		else weights[i] = self_unit_weight*calc_base_weight(gg,i);
	}
	*/
	//Implement Djikstras.
	
	// int visited[V];
/*	float sp[V];
	for(int i=0;i<V;i++)
		sp[i] = INFINITY,visited[i]=-1;
	sp[selected] = 0;
	for(int i=0; i<V; i++)
	{
		
		// else{
			// if(!visited[i])
			if(gg->parent->m[i][selected])
			{
				// if(!visited[i])
				// {
					
				// }
				adjNode* trav = gg->game_graph->arr[i].head;
				while(trav->next!=NULL)
				{
					if(!visited[trav->adj])
					{
						
					}
				}
				
			
			// }
		}
	}*/
	return NULL;
	
}
void GAME_LOOP_AI(gameGrid* gg)
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
						mvprintw(0,0,"Player turn");
						gsai = SELECT_NODE_PLAYER;
						
					break;
					
					case TURN_AI:
						wclear(stdscr);
						mvprintw(0,0,"Computer turn");
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
									mvprintw(1,1,"node %d selected",selected);
									units = gg->node[selected]->units/2;
									mvprintw(1,1,"node %d selected %d",selected, units);
									gsai = SEND_UNITS_PLAYER;
								}
								else{
									mvprintw(0,0,"Not a player node");
									selected=-1;
								} 
							}
						}
						}
						// gsai = TURN_PLAYER;
					break;
					
					case SELECT_NODE_AI:
						
						while(!is_enemy_node(gg,(selected = rng(gg->nodes-1,0))));
						gsai=WEIGH_OPTIONS_AI;
					break;
					
					case WEIGH_OPTIONS_AI:
						printw("AI thinking");
						napms(2000);
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
								selected = -1; dest = -1;
							}
						}
						}
						
					break;
					
					case SEND_UNITS_AI:
						sub_units(gg,0,10);
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
						
					gsai = TURN_AI;
					break;
					case END_TURN_AI:
						gsai=TURN_PLAYER;
					break;
					
					case PLAYER_WIN:
					
					break;
					
					case AI_WIN:
						
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