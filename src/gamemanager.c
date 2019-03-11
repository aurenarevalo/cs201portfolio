#include "gamemanager.h"

static unsigned int SD1 = 234234, SD2 = 21456789, SD3 = 0x7f0ce821;

typedef struct point Point;

typedef struct inode infectionNode;

typedef struct ggrid gameGrid;

gameGrid* init_gameGrid(int r, int c, int nodes)
{
	printf("\n\n %d ROWS %d COLS %d NODES : %d possible NODES\n\n\n",r,c,nodes,r*c);
	gameGrid* gg = malloc(sizeof(gameGrid));
	gg->parent = init_intMatrix(r,c);
	gg->node = malloc(nodes*sizeof(infectionNode));
	for(int i=0; i<nodes; i++)
		gg->node[i] = *init_infectionNode(0,-1);
	gg->nodes = nodes;
	return gg;

}

infectionNode* init_infectionNode()
{
	infectionNode *node = malloc(sizeof(infectionNode));
	node->control = -1;
	node->units = -1;
	node->pos.x = 0;
	node->pos.y = 0;
	node->al.head = NULL;
	return node;
}

void place_node(gameGrid** grid,infectionNode node)
{
	gameGrid* ggrid = *grid;
	int x,y;
	x = node.pos.x;
	y = node.pos.y;
	ggrid->parent->m[y][x] = node.control;
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
	set_node_params(&ggrid->node[ggrid->nodes-1],contr,units,x,y);
	place_node(grid,ggrid->node[ggrid->nodes-1]);
}
 /*
 * pseudo rng that seems to maintain a decent distribution. 
 * probably can figure out, mathematically, one that will do better... but whatev
 * inspired by the KISS algorithm, but uses time() so the same board wont be generated at startup
 */

int rng(unsigned int max,unsigned int min)
 {	
	unsigned long long x;
	unsigned int t = time(NULL);
	unsigned int m = 37;
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

gameGrid* generate_gameGrid()
{
	int nrows, ncols, nodes;
	nrows = 7;//rng(MAX_ROWS,MIN_ROWS);
	ncols = 7;//rng(MAX_COLS,MIN_COLS);
	nodes =4;// rng(MAX_NODES,MIN_NODES);
	gameGrid* gg = init_gameGrid(nrows,ncols,nodes);
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
	set_node_params(&gg->node[0],2,50,5,5);
	place_node(&gg,gg->node[0]);
	set_node_params(&gg->node[1],3,50,1,1);
	place_node(&gg,gg->node[1]);
/*	for(int i=0; i<nodes; i++)
	{
		set_node_params(&gg->node[i],1,50,rng(ncols,0),rng(nrows,0));
		place_node(&gg,gg->node[i]);
		printf("%d\n",gg->node[i].pos.x);
	}*/
	set_node_params(&gg->node[2],1,50,3,3);
	set_node_params(&gg->node[3],1,50,5,1);
	place_node(&gg,gg->node[2]);
	place_node(&gg,gg->node[3]);

	add_new_node(&gg,4,50,4,4);
	add_new_node(&gg,4,50,2,2);
	find_distance(gg->node[0],gg->node[1]);


	return gg;
}

