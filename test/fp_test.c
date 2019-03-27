#include <stdio.h>
#include <stdlib.h>




typedef struct anode{
	int weight;
	int adj;
	struct anode *next;
}adjNode;

typedef struct alist{
	adjNode *head;
}adjList;

typedef struct grph{

	int vertices;
	char* letters;
	adjList *arr;
}Graph;

typedef struct imtrx{
	int r,c;
	int **m;
}intMatrix;

FILE* f;


adjNode* init_adjNode();
adjList* init_adjList();
Graph* init_Graph(int v);
void new_node(adjList* al, int to, int w);
void new_edge(Graph** gr,int from, int to,int w);
intMatrix* init_intMatrix(int r, int c);
intMatrix* construct_adj_matrix(Graph* gr);
void print_matrix(intMatrix *m);

void print_paths(Graph* gr,int src, int dest);
void print_all_pathsU(Graph* gr,int u, int d, int visited[], 
                            int path[], int path_index);
void set_letters(Graph* gr,char* str);

int main(void)
{
	f = fopen("test.txt","w");
	
	Graph* gr = init_Graph(9);
	new_edge(&gr,0,1,1);
	// new_edge(&gr,0,2,1);
	new_edge(&gr,0,3,1);
	new_edge(&gr,0,4,1);
	// new_edge(&gr,0,5,1);
	// new_edge(&gr,0,6,1);
	// new_edge(&gr,0,7,1);
	// new_edge(&gr,0,8,1);
	
	new_edge(&gr,1,2,1);
	new_edge(&gr,1,3,1);
	new_edge(&gr,1,4,1);
	new_edge(&gr,1,5,1);
	// new_edge(&gr,1,6,1);
	// new_edge(&gr,1,7,1);
	// new_edge(&gr,1,8,1);
	
	// new_edge(&gr,1,3,1);
	new_edge(&gr,2,4,1);
	new_edge(&gr,2,5,1);
	// new_edge(&gr,1,6,1);
	// new_edge(&gr,1,7,1);
	// new_edge(&gr,1,8,1);
	
	new_edge(&gr,3,4,1);
	// new_edge(&gr,3,5,1);
	new_edge(&gr,3,6,1);
	new_edge(&gr,3,7,1);
	// new_edge(&gr,1,8,1);
	
	new_edge(&gr,4,5,1);
	new_edge(&gr,4,6,1);
	new_edge(&gr,4,7,1);
	new_edge(&gr,4,8,1);
	
	
	new_edge(&gr,5,7,1);
	new_edge(&gr,5,8,1);
	
	new_edge(&gr,6,7,1);
	
	new_edge(&gr,7,8,1);

	
	// intMatrix *im = init_intMatrix(9,9);
	// im = construct_adj_matrix(gr);
	
	set_letters(gr,"TEUAPEQKS");
	// print_matrix(im);
	
	
/*	new_edge(&gr,1,2,1);
	new_edge(&gr,1,4,1);
	new_edge(&gr,1,3,1);
	new_edge(&gr,1,5,1);
	new_edge(&gr,0,3,1);
	new_edge(&gr,0,4,1);*/
	print_paths(gr,0,5);
	fclose(f);
/*	int p = 5;
	int *c = &p;
	
	printf("%d , %d ",p,*c);
	*c+=1;
		printf("%d , %d ",p,*c);*/
	
	
	
	
}

void set_letters(Graph* gr,char* str)
{
	for(int i = 0; (i < gr->vertices) && str[i] != '\0';i++)
	{
		gr->letters[i] = str[i];
	}
}

/*
* initialize variables and then call the function that actually will traverse
* Pass in your graph struct so you can access adjacency lists
* src is where you're coming from
* dest is where you want to go 
*/
void print_paths(Graph* gr,int src, int dest)
{
	//get size of graph
	int V = gr->vertices;
	// dynamically allocate arrays that keep track of the path --
	int path[V];
	// -- and visited nodes
	int visited[V];
	// initialization to 0;
	for(int i=0; i<V;i++)
	{
		visited[i]=0,path[V]=0;
	}
	//keeps track of index for path array
	int path_i=0;
	
	print_all_pathsU(gr,src, dest,visited,path,path_i);
	
}


/*
* The function that actually traverses the graph and finds all possible paths
*	from src to dest.  Takes in current node num (int u), destination (int d)
*	the array that keeps track of visited nodes (int visited[]), the array	
*	that keeps track of the current path (int path[]), and the  
*	index of the path array (int path_index) which is equivalent to path length
*
*/
void print_all_pathsU(Graph* gr,int u, int d, int visited[], 
                            int path[], int path_index) 
{
	//add node to path
	path[path_index]= u;
	//increement path_index/length
	path_index+=1;
	//mark node as visited
	visited[u] = 1;
	// printf("%d\n",u);

	//If current node == destination node
	if(u==d && path_index>2){
		//Print out the path
		
		for(int i=0; i<path_index; i++)
		{
			// printf("(%d)|%c| -> ",path[i],gr->letters[path[i]]);
			fprintf(f,"%c",gr->letters[path[i]]);
		}
		// printf("\n");
		fprintf(f, "\n");
		}
		//otherwise...
		else
		{
			//tmp node to traverse the adjacency list at current node (u)
			adjNode *tmp = gr->arr[u].head;
			//while the node exists
			while(tmp !=NULL)
			{
				//i is the node number of an adjacent node
				int i=tmp->adj;
				//If node hasn't been visited, call recursively using (int i) for the (int u)
				//		parameter.
				if(!visited[i])
					print_all_pathsU(gr,i,d,visited,path,path_index);
				//next node
				tmp=tmp->next;
			}
		}
		
	//occurs once all adjacent nodes have been searched
	// or current node == dest node
	
	// reset the path	
	path[path_index] = 0;
	//decrement path_index/length
	// at end of a path, it will decrement back to 0
	path_index-=1;
	//mark node as not visited
	visited[u]=0;
}


adjNode* init_adjNode()
{
	adjNode* an = malloc(sizeof(adjNode));
	an->adj = -1;
	an->weight = 0;
	an->next = NULL;
	return an;
}
adjList* init_adjList()
{
	adjList* al=malloc(sizeof(adjList));
	al->head = malloc(sizeof(adjNode));
	al->head->adj = -1;
	al->head->weight = 0;
	al->head->next = NULL;
	return al;
}

Graph* init_Graph(int v)
{
	Graph *g = malloc(sizeof(Graph));
	g->vertices = v;
	g->arr = calloc(v,sizeof(adjList));
	for(int i = 0; i<v;i++)
		g->arr[i] = *init_adjList();
	g->letters = calloc(v,sizeof(char));
	return g;
}
/*
* MAKE SURE TO CHECK FOR MEMORY LEAKS!!!!
*/
void new_node(adjList* al, int to, int w)
{
	adjNode* tmp_head = al->head;
	adjNode* tmp_traverse = al->head;
	adjNode* node= malloc(sizeof(adjNode));
	if(tmp_head->adj == -1)
	{
		node->adj = to;
		node->weight = w;
		node->next = NULL;
		*tmp_head = *node;
		return;
	}
	if(tmp_traverse->adj == to) return;
	while(tmp_traverse->next != NULL)
	{
		if(tmp_traverse->adj == to) return;
		tmp_traverse= tmp_traverse->next;
	}
	node->adj = to;
	node->weight = w;
	node->next = NULL;
	tmp_traverse->next = node;
	al->head = tmp_head;
}

void new_edge(Graph** gr,int from, int to, int w)
{
	Graph* g= *gr;
	if((g->vertices < from || g->vertices < to) || (from < 0 || to < 0))
	{
		printf("Trying to access unavailable node. Negative or greater than vertices.\n Aborting process --new_edge-- \n");
		return;
	}
	adjList *tmp_list = g->arr;
	new_node(&tmp_list[from],to,w);
	new_node(&tmp_list[to],from,w);
	g->arr[from] = tmp_list[from];
}

intMatrix* init_intMatrix(int r, int c)
{
	intMatrix* m = calloc(1,sizeof(intMatrix));
	m->r = r;
	m->c = c;
	m->m = (int**)calloc(r,sizeof(int*));
	for(int i=0; i<r; i++)
		m->m[i] = (int*)calloc(c,sizeof(int));
	return m;
}

intMatrix* construct_adj_matrix(Graph* gr)
{
	int v = gr->vertices;
	intMatrix *mat = init_intMatrix(v,v);
	
	for(int r=0; r<v; r++)
	{
		adjList tmp_list = gr->arr[r];
		adjNode* tmp = tmp_list.head;
		// if(tmp->adj == -1)break;
		while(tmp->next != NULL)
		{
			mat->m[r][tmp->adj] = 1;//tmp->weight;
			tmp=tmp->next;
		}
		mat->m[r][tmp->adj] =1;//	tmp->weight;

	}
	return mat;
}

void print_matrix(intMatrix *m)
{
	int rlim = m->r;
	int clim = m->c;
	printf(" ");
	for(int c=0; c<clim; c++)
		printf("__%d__",c);
	printf("\n");
	for(int r=0; r<rlim; r++)
	{
		printf("%d|",r);
		for(int c=0; c<clim; c++){
			if(m->m[r][c] != 9)
				printf(" %d |  ",m->m[r][c]);
			else
				printf(" * |  ");
		}
		printf("\n");
	}
}