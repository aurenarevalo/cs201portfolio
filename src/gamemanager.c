#include "gamemanager.h"

static unsigned int SD1 = 234234, SD2 = 21456789, SD3 = 0x7f0ce821;

typedef struct point Point;

typedef struct inode infectionNode;

typedef struct ggrid gameGrid;

gameGrid* init_gameGrid(int r, int c, int nodes,PANEL* game_pan)
{
	gameGrid* gg = malloc(sizeof(gameGrid));
	gg->parent = init_intMatrix(r,c);
	gg->node = (infectionNode**)malloc(nodes*sizeof(infectionNode*));
	for(int i=0; i<nodes; i++)
		gg->node[i] = init_infectionNode();
	gg->node_size = nodes;
	gg->nodes = 0;
	gg->game_panel =game_pan;
	
	return gg;

}
void delete_gameGrid(gameGrid* gg)
{
	 gg->node_size=0,gg->nodes=0;
	gg->n_p1=0,gg->n_p2=0,gg->n_neutral=0;
	gg->pane_r=0, gg->pane_c=0;
	gg->parent=NULL;
	gg->node=NULL;
	gg->game_graph=NULL;
	
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
	node->node_but =NULL;
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
	if(ggrid->node_size ==0) ggrid->node_size = 1, ggrid->node = malloc(sizeof(infectionNode));
	if(ggrid->nodes >= ggrid->node_size)ggrid->node_size =2*ggrid->node_size, ggrid->node = realloc(ggrid->node,ggrid->node_size*sizeof(infectionNode));
	ggrid->node[ggrid->nodes-1] = init_infectionNode();
	set_node_params(ggrid->node[ggrid->nodes-1],contr,units,x,y);
	place_node(grid,ggrid->node[ggrid->nodes-1]);
		if(contr == 1) ggrid->n_neutral++;
		else if(contr==2) ggrid->n_p1++;
		else if(contr==3) ggrid->n_p2++;
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
	while(x<=0 || num < min){
		t ^= (t>>2); t ^= (t<<5); t^= (t<<5);
		SD1 ^= (SD1<<3); SD1^= (SD1>>3);
		SD2 ^= (SD2>>3); 
		SD3 ^= (SD3>>1);
		 x = (SD1*SD2 +SD3*SD1 + SD1*124567 + SD3)*t/3;
	 	num = x%(max+1);
	 }
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
	int strt = MIN_NODES*floor(y/SUB_BOARD_H);
	for(int i=strt; i<size; i++)
	{
		Point tmp = gg->node[i]->pos;
		int bounds_x[2] = {tmp.x,tmp.x+3};
		int bounds_y[2] = {tmp.y,tmp.y+3};
		int cond = (x >= bounds_x[0] &&x<=bounds_x[1]) &&( y>= bounds_y[0]&& y<=bounds_y[1]);
		if(cond) return 0;
		if(x==tmp.x && y==tmp.y)return 0;
	}
	return 1;
}

gameGrid* generate_gameGrid(SCENE* gscene,PANEL* game_pan)
{
	int nrows=5, ncols=5, nodes=0;
	PANEL* in_pan = new_panel(newwin_offset(4,5));
	top_panel(in_pan);
	update_panels();
	doupdate();

	update_panels();
	doupdate();
	keypad(panel_window(in_pan),TRUE);
	curs_set(TRUE);
	int  count=0;
	//input should not be larger than 20, (largest unsigned int)
	char buf[50];
	echo();
	int is_valid_input = 0;
do{	
		mvwprintw(panel_window(in_pan),2,20,"Max theoretical num is %d",INT_MAX);
		mvwprintw(panel_window(in_pan),3,14,"So input over 10 digits will read as invalid");
		mvwprintw(panel_window(in_pan),4,14,"anything over %d will cause error", INT_MAX);
		wattron(panel_window(in_pan),A_UNDERLINE);
		mvwprintw(panel_window(in_pan),5,10,"Enter rows:");
		wattroff(panel_window(in_pan),A_UNDERLINE);
		box(panel_window(in_pan),'*','*');
		count = 0;
		while((wgetnstr(panel_window(in_pan),buf,40)));
		is_valid_input=1;
		for( count=0; buf[count] !='\0'; count++)
		{
			if(isdigit(buf[count]) && buf[0] != '0');
			else is_valid_input=0;
		}
		if(count >= 11 || count ==0 ){  is_valid_input=0; }
		printw("%d-",is_valid_input);

	}while(!is_valid_input);
	nrows =0 ;
	char *in1 = malloc(count*sizeof(char));
	for(int i=0; i<count;i++)
	{
		in1[i]=buf[i];
	}
	int game_windows_y = atoi(in1);
	is_valid_input=0;
		do{	
		mvwprintw(panel_window(in_pan),2,20,"Max theoretical num is %d",INT_MAX);
		mvwprintw(panel_window(in_pan),3,14,"So input over 10 digits will read as invalid");
		mvwprintw(panel_window(in_pan),4,14,"anything over %d will guarantee error", INT_MAX);
		wattron(panel_window(in_pan),A_UNDERLINE);
		mvwprintw(panel_window(in_pan),6,10,"Enter cols:");
		wattroff(panel_window(in_pan),A_UNDERLINE);
		box(panel_window(in_pan),'*','*');
		count = 0;

		while((wgetnstr(panel_window(in_pan),buf,40)));
		is_valid_input=1;
		for( count=0; buf[count] !='\0'; count++)
		{
			if(isdigit(buf[count]) && buf[0] != '0');
			else is_valid_input=0;
		}
		if(count >= 11  || count ==0){  is_valid_input=0; }
		printw("%d-",is_valid_input);

	}while(!is_valid_input);
	char *in2 = malloc(count*sizeof(char));
	for(int i=0; i<count;i++)
	{
		in2[i]=buf[i];
	}
	mvwprintw(panel_window(in_pan),10,20,"Generating board");
	update_panels();
	doupdate();
	noecho();
	int game_windows_x = atoi(in2);
	ncols = atoi(in2);
	curs_set(FALSE);
	nrows= game_windows_y*SUB_BOARD_H/NODE_H;
	ncols= game_windows_x*SUB_BOARD_W/NODE_W;
	gameGrid* gg = init_gameGrid(nrows,ncols,nodes,game_pan);
	gg->pane_r = game_windows_y, gg->pane_c = game_windows_x;
	gg->game_graph= init_Graph(gg->pane_r*gg->pane_c*MAX_NODES);
	refresh();
	int pos_x,pos_y,p1_nodes,p2_nodes,win_nodes,ntrl_nodes,ctrl;
	int tot_nodes=1;
	for(int i = 0; i < gg->pane_r; i++)
	{
		
		for (int j = 0; j < gg->pane_c; j++)
		{
			int ctrl_count[3] = {0};
			win_nodes = rng(MAX_NODES,MIN_NODES);
			tot_nodes+=win_nodes;
			p1_nodes = p2_nodes = win_nodes/4;
			ntrl_nodes = win_nodes - 2*p1_nodes;
			for(int c = 0; c < win_nodes;c++){
				pos_x = rng(SUB_BOARD_W*(j+1)/3-2,SUB_BOARD_W*j/NODE_W+1);
				pos_y = rng(SUB_BOARD_H*(i+1)/2-2,SUB_BOARD_H*i/NODE_H+1);
				if (ctrl_count[1] < p1_nodes) ctrl = 2;
				else if (ctrl_count[2] < p2_nodes ) ctrl =3;
				else if (ctrl_count[0] < ntrl_nodes)ctrl = 1;
				
				ctrl_count[ctrl-1]++;
				
				while(!is_pos_available(gg,pos_x,pos_y))
				{
					pos_x = rng(SUB_BOARD_W*(j+1)/3-2,SUB_BOARD_W*j/NODE_W+1);
					pos_y = rng(SUB_BOARD_H*(i+1)/2-2,SUB_BOARD_H*i/NODE_H+1);
				}
				add_new_node(&gg,(float)ctrl,50,pos_x,pos_y);
		}
			for(int l = tot_nodes-win_nodes; l <tot_nodes; l++ ){
				for( int b = l; b < tot_nodes; b++)
					add_adjacency(&gg,l-1,b-1);
				}
				mvwprintw(panel_window(in_pan),12,20,"Completed: %d rows AND %d columns",i,j);
				wrefresh(panel_window(in_pan));
				doupdate();
		
		}
	}

	refresh();
	update_panels();
	doupdate();
	del_panel(in_pan);

	return gg;
}
/*
* Search method that picks a minimum possible node to improve
*upon a simple linear search. Only has to traverse 1 row.
*/
int find_start_node(gameGrid *gg, int win_y, int win_x)
{
	int mstart=((win_y)*(gg->pane_c))*(MIN_NODES-1)+win_x*(MIN_NODES - 1);
	int bounds_x[2], bounds_y[2];
	bounds_x[0] = win_x*SUB_BOARD_W/NODE_W,bounds_x[1] = (win_x+1)*SUB_BOARD_W/NODE_W-1;
	bounds_y[0] = win_y*SUB_BOARD_H/NODE_H,bounds_y[1] = (win_y+1)*SUB_BOARD_H/NODE_H-1;
	infectionNode *tmp = gg->node[mstart];
	int node_x=tmp->pos.x, node_y=tmp->pos.y;
	if(node_x >= bounds_x[0] && node_x <bounds_x[1] && node_y >= bounds_y[0]&& node_y < bounds_y[1]){
		 return mstart;
	}
	else
	{
		while(node_y < bounds_y[0])
		{
			mstart++;
			tmp = gg->node[mstart];
			node_y = tmp->pos.y;
			node_x = tmp->pos.x;
		} 
		while(node_x <bounds_x[0] )
		{
			mstart++;
			tmp = gg->node[mstart];
			node_x = tmp->pos.x;
		}
		node_y=tmp->pos.y;
		return mstart;
	}
	return 0;
}

int find_end_node(gameGrid* gg,int snode, int win_y, int win_x)
{	
	int bounds_x[2];//, bounds_y[2];
	bounds_x[0] = win_x*SUB_BOARD_W/NODE_W,bounds_x[1] = (win_x+1)*SUB_BOARD_W/NODE_W-1;
//	bounds_y[0] = win_y*SUB_BOARD_H/NODE_H,bounds_y[1] = (win_y+1)*SUB_BOARD_H/NODE_H-1;
	infectionNode *tmp = gg->node[snode];
	int node_x = tmp->pos.x;//, node_y = tmp->pos.y;
	int i =0 ;
	for( i =snode; i < gg->nodes && node_x <bounds_x[1]+1;i++)
	{
		tmp = gg->node[i];
		
		node_x = tmp->pos.x;
		// node_y = tmp->pos.y;
	}
	return i-1;
}

void refresh_nodes(gameGrid** gg,int win_y, int win_x)
{
	gameGrid* ggt= *gg;
	int start_indx = find_start_node(ggt,win_y,win_x);
	int end_indx = find_end_node(ggt,start_indx,win_y,win_x);
	for(int i = start_indx; i<= end_indx; i++)
	{	
		if(ggt->node[i]->control == 1)wbkgd(button_win(ggt->node[i]->node_but),COLOR_PAIR(7));
		else if ( ggt->node[i]->control == 2)wbkgd(button_win(ggt->node[i]->node_but),COLOR_PAIR(5));
		else if (ggt->node[i]->control == 3)wbkgd(button_win(ggt->node[i]->node_but),COLOR_PAIR(6));
		mvwprintw(button_win(ggt->node[i]->node_but),0,0,"%d ",ggt->node[i]->units);
		wrefresh(button_win(ggt->node[i]->node_but));
	}
}
void change_sub_board(gameGrid** gg,int win_y_new, int win_x_new,int old_x, int old_y)
{
	gameGrid* ggt= *gg;
	int start_indx = find_start_node(ggt,win_y_new,win_x_new);
	int end_indx = find_end_node(ggt,start_indx,win_y_new,win_x_new);
	int oldstart = 0;//find_start_node(ggt,old_y,old_x);
	int oldend = 2;//find_end_node(ggt,oldstart,old_y,old_x);
	for(int i = oldstart; i <= oldend;i++)
		free(ggt->node[i]->node_but);
	for(int i = start_indx; i <= end_indx;i++)
	{
		infectionNode* tmp = ggt->node[i];
		int rel_x = tmp->pos.x*NODE_W-SUB_BOARD_W*win_x_new,rel_y=tmp->pos.y*NODE_H-SUB_BOARD_H*win_y_new;
		ggt->node[i]->node_but = new_button(panel_window(ggt->game_panel),rel_y,rel_x,2,3);
		if(tmp->control == 1){
		wbkgd(button_win(ggt->node[i]->node_but),COLOR_PAIR(7));
		}
		else if(tmp->control == 2)
			wbkgd(button_win(ggt->node[i]->node_but),COLOR_PAIR(5));
		else if(tmp->control == 3)
			wbkgd(button_win(ggt->node[i]->node_but),COLOR_PAIR(6));
		mvwprintw(button_win(ggt->node[i]->node_but),0,0,"%d",ggt->node[i]->units);
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

int is_node_press(gameGrid* gg,int n, MEVENT me, int win_y, int win_x)
{
	Point tmpp = gg->node[n]->pos;
	BUTTON* tmp = new_button(panel_window(gg->game_panel),tmpp.y,tmpp.x,NODE_H,NODE_W);
	int ret_val = is_button_press(me,tmp);
	printw(" IT IS NOW : %d",ret_val);
	return ret_val;
}

//Check mouse button 1 click (down) is within bounds of a button. Returns the button node.
int mc1_check_nodes(gameGrid* gg,MEVENT me,int win_y,int win_x )
{
	int strt = find_start_node(gg,win_y,win_x);
	int end = find_end_node(gg,strt,win_y,win_x);
		for(int i = strt; i<= end; i++)
		{
			infectionNode* node = gg->node[i];
			if(is_button_press(me,node->node_but))
			{
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
	if(gg->node[node]->units <10); 
	if (gg->node[node]->units <= 0)
		return 2;
	return 0;
}
//changes control of node and sets color accordingly
void change_control(gameGrid* gg, int node, int new_control, int overflow,int win_y,int win_x)
{
	int ctrl = gg->node[node]->control;
	if(ctrl == 1) gg->n_neutral--;
	else if (ctrl == 2) gg->n_p1--;
	else if (ctrl == 3)gg->n_p2--;
	gg->node[node]->control = new_control;
	if(new_control == 1) gg->n_neutral++;
	else if (new_control == 2) gg->n_p1++;
	else if (new_control == 3)gg->n_p2++;
	if(gg->node[node]->units < 0) gg->node[node]->units = overflow;
	refresh_nodes(&gg,win_y,win_x);
}

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
	if(gg->n_p2 <=0) return 1;
	if(gg->n_p1 <=0) return 2;
	if(gg->n_neutral <= 0 && gg->n_p1 > gg->n_p2) return 1;
	if(gg->n_neutral <= 0 && gg->n_p2 > gg->n_p1) return 2;
	
	return 0;
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


int calc_attack_AI(gameGrid* gg, int selected, int win_y,int win_x)
{
	if(gg->game_graph->arr[selected].head == NULL) return 0;
	int V = gg->game_graph->vertices; //max nodes per board
	float weight[V];
	float self_unit_weight;
	for(int i=0; i<V; i++)
		weight[i] = 10000;
	adjNode *curr = gg->game_graph->arr[selected].head;
	int n=0;
	while(curr != NULL)
	{
		n=curr->adj;
		int contr = gg->node[n]->control;
		int units = gg->node[n]->units;
		if(contr==1) weight[n] += CTRL_W_N;
		else if(contr==2) weight[n] += CTRL_W_P;
		else if(contr==3 && units < 50) weight[n] += CTRL_W_SLT;
		else if(contr==3 && units >= 50) weight[n] += CTRL_W_SGT;
		weight[n] = gg->node[n]->units/3;
		curr = curr->next;
	}
	int units = gg->node[selected]->units;
	if(units > 75) self_unit_weight = 0.5;
	else if (units > 50) self_unit_weight = 0.75;
	else if (units <= 25) self_unit_weight = 1.5;
	else self_unit_weight = 1;
	int min=weight[0];
	int minindx = 0;
	for(int i=0; i<V; i++){ 
		if(min == 0 || weight[i] < min) min =weight[i],minindx = i;
		weight[i] =weight[i]*self_unit_weight;
	}
	return minindx;
	
}

int GAME_LOOP_AI(gameGrid* gg, SCENE* game_scene)
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
	
	
	int curr_win_x=0, curr_win_y=0;
	int new_win_x = 0, new_win_y = 0;
	change_sub_board(&gg,new_win_y,new_win_x,curr_win_y,curr_win_x);
	refresh_nodes(&gg,curr_win_y,curr_win_x);
	int units = 10;
	int unit_gen = 5;
	
	nodelay(panel_window(gg->game_panel),TRUE);
	int endgame=0;
	int end_game_return = 0;
	int selected=-1, dest=-1;
	while((input=wgetch(panel_window(gg->game_panel)))!='q' && !endgame)
	{
		switch(input)
		{
			case KEY_MOUSE:
			default:
				switch(gsai)
				{
					case NEW_GAME_AI: //implement regeneration of board
					
					break;
					
					case TURN_PLAYER:
						wclear(stdscr);
						//something useless
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
							selected = mc1_check_nodes(gg,event,curr_win_y,curr_win_x);
							if(selected>=0)
							{	
								if(event.bstate & BUTTON1_CLICKED)
								{
									if(is_player_node(gg,selected))
									{
										box(button_win(gg->node[selected]->node_but),0,0);
										units = gg->node[selected]->units/2;
										gsai = SEND_UNITS_PLAYER;
									}
									else
									{
										selected=-1;
									} 
							}
						}
						if(event.bstate  & BUTTON2_CLICKED)
							gsai=END_TURN_PLAYER;
						else if ( event.bstate & BUTTON1_DOUBLE_CLICKED){
								if(curr_win_x+1 < gg->pane_c) new_win_x++;
								else break;
								wclear(panel_window(gg->game_panel));
								change_sub_board(&gg,new_win_y,new_win_x,curr_win_y,curr_win_x);
								
								curr_win_x++;
							}
						else if( event.bstate & BUTTON1_TRIPLE_CLICKED)
							{
								if(curr_win_y+1 < gg->pane_r) new_win_y++;
								else break;
								wclear(panel_window(gg->game_panel));
								change_sub_board(&gg,new_win_y,new_win_x,curr_win_y,curr_win_x);
								
								curr_win_y++;
							}
						else if(event.bstate & BUTTON3_TRIPLE_CLICKED)
							{
								if(curr_win_y-1 >=0) new_win_y--;
								else break;
								wclear(panel_window(gg->game_panel));
								change_sub_board(&gg,new_win_y,new_win_x,curr_win_y,curr_win_x);
								
								curr_win_y--;
							}
							else if(event.bstate & BUTTON3_DOUBLE_CLICKED)
							{
								if(curr_win_x-1 >=0) new_win_x--;
								else break;
								wclear(panel_window(gg->game_panel));
								change_sub_board(&gg,new_win_y,new_win_x,curr_win_y,curr_win_x);
								
								curr_win_x--;
							}
						}
						
					break;
					
					case SELECT_NODE_AI: ;
						while(!is_enemy_node(gg,(selected = rng(gg->nodes-1,0))) && !check_win_condition(gg));
						units = gg->node[selected]->units/2;
						gsai=WEIGH_OPTIONS_AI;
					break;
					
					case WEIGH_OPTIONS_AI:
						
						dest = calc_attack_AI(gg,selected, curr_win_y, curr_win_x);
						gsai=SEND_UNITS_AI;
					break;
					
					case SEND_UNITS_PLAYER:
						if(getmouse(&event) == OK)
						{
						if(event.bstate & BUTTON1_CLICKED)
						{	
							if((dest= mc1_check_nodes(gg,event,curr_win_y,curr_win_x))+1){
								int cond = check_units(gg,dest);
								if(cond!=1 || (is_enemy_node(gg,dest)))
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
										change_control(gg,dest,2,extra_sub,curr_win_y,curr_win_x);
									if(check_units(gg,dest) ==1  && is_player_node(gg,dest)){
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
					break;
					
					case SEND_UNITS_AI:
					printw("ai selected %d, control: %d**",dest,gg->node[dest]->control);
					printw("  %d units start", gg->node[dest]->units);
						sub_units(gg,selected,units);
						printw("  %d units end", gg->node[dest]->units);
						int extra_add, extra_sub;
						if(is_enemy_node(gg,dest)) extra_add = add_units(gg,dest,units);
						else extra_sub = sub_units(gg,dest,units);
						printw("  %d units end", gg->node[dest]->units);
						if(check_units(gg,dest)==2)
							change_control(gg,dest,3,extra_sub,curr_win_y,curr_win_x);
						if(check_units(gg,dest) ==1 && is_enemy_node(gg,dest)){
							add_units(gg,selected,extra_add);
							}
						gsai=END_TURN_AI;
					break;
					
					case END_TURN_PLAYER:
					selected=-1,dest=-1;
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
					selected=-1,dest=-1;
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
					
					case PLAYER_WIN: ;
						mousemask(!ALL_MOUSE_EVENTS,NULL);
						PANEL* pwin = new_panel(newwin_offset(8,15));
						box(panel_window(pwin),0,0);
						
						mvwprintw(panel_window(pwin),3,20,"PLAYER WIN!");
						mvwprintw(panel_window(pwin),5,20,"Press a key to continue, press q to exit");
						nodelay(panel_window(pwin),FALSE);
						update_panels();
						doupdate();
						int endch;
						del_panel(pwin);
						while((endch= wgetch(panel_window(pwin)))==KEY_MOUSE);
						if(endch=='q') end_game_return=1;
						endgame=1;
					break;
					
					case AI_WIN: ;
						mousemask(!ALL_MOUSE_EVENTS,NULL);
						PANEL* ewin = new_panel(newwin_offset(8,15));
						box(panel_window(ewin),0,0);
						
						mvwprintw(panel_window(ewin),3,20,"ENEMY WIN!");
						mvwprintw(panel_window(ewin),5,20,"Press a key to play again, press q to exit");
						nodelay(panel_window(ewin),FALSE);
						update_panels();
						doupdate();
						while((endch= wgetch(panel_window(ewin)))==KEY_MOUSE);
						if(endch=='q') end_game_return=1;
						del_panel(ewin);
						endgame=1;
					break;
			
				}
			break;
		case 113: // if q
			endgame=1;
			end_game_return=1;
			//exit
		break;
		
		
		}

		mvwprintw(scene_window(game_scene,0),2,10,"Curr x win: %d  of %d| Curr y win: %d of %d", curr_win_x,gg->pane_c-1,curr_win_y,gg->pane_r-1);
		refresh_nodes(&gg,curr_win_y,curr_win_x);
		update_panels();
		doupdate();
	}
			if(input=='q') end_game_return=1;
	clear();
	return end_game_return;
}

void GAME_LOOP_LOCAL(gameGrid* gg)
{
	int input;
	// GAME_STATES_LOCAL gsl;
int curr_win_x=1, curr_win_y=1;
	while((input=wgetch(panel_window(gg->game_panel)))!=122)
	{
		switch(input){	
		case KEY_MOUSE:
		/*	switch(gsl)
			{
				
				
				
			}*/
		
		
		
		break;
		
		}
		refresh_nodes(&gg,curr_win_y,curr_win_x);
		update_panels();
		doupdate();

	}


}