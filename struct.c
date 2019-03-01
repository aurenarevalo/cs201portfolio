#include "struct.h"

typedef struct anode adjNode;
typedef struct alist adjList;
typedef struct grph Graph;


adjList* init_adjList()
{
	adjList* al=malloc(sizeof(adjList));
	al->head = malloc(sizeof(adjNode));
	al->head->adj = -1;
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

	return g;
}

void new_node(adjList* al, int to)
{
	// adjList* tmp = al;
	adjNode* tmp_head = al->head;
	adjNode* tmp_traverse = al->head;
	adjNode* node= malloc(sizeof(adjNode));
	if(tmp_head->adj == -1)
	{
		node->adj = to;
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
	node->next = NULL;
	tmp_traverse->next = node;
	al->head = tmp_head;
}

void new_edge(Graph** gr,int from, int to)
{
	Graph* g= *gr;
	if((g->vertices < from || g->vertices < to) || (from < 0 || to < 0))
	{
		printf("Trying to access unavailable node. Negative or greater than vertices.\n Aborting process --new_edge-- \n");
		return;
	}
	adjList *tmp_list = g->arr;
	new_node(&tmp_list[from],to);
	g->arr[from] = tmp_list[from];
/*	new_node(&tmp_list[to],from);
	g->arr[to] = tmp_list[to];*/
}

int** init_matrix(int r, int c)
{
	int** m = calloc(r,sizeof(int*));
	for(int i=0;i<c;i++)
		m[i] = calloc(1,sizeof(int));
	return m;
}

int** construct_adj_matrix(Graph* gr)
{
	int v = gr->vertices;
	int **mat = init_matrix(v,v);
	return mat;
}