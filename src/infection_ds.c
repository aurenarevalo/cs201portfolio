#include "infection_ds.h"

typedef struct anode adjNode;
typedef struct alist adjList;
typedef struct grph Graph;

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
	return g;
}
/*
* MAKE SURE TO CHECK FOR MEMORY LEAKS!!!!
*/
void new_node(adjList* al, int to, float w)
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

void new_edge(Graph** gr,int from, int to,float w)
{
	Graph* g= *gr;
	if((g->vertices < from || g->vertices < to) || (from < 0 || to < 0))
	{
		printf("Trying to access unavailable node. Negative or greater than vertices.\n Aborting process --new_edge-- \n");
		return;
	}
	adjList *tmp_list = g->arr;
	new_node(&tmp_list[from],to,w);
	g->arr[from] = tmp_list[from];
}

intMatrix* init_intMatrix(int r, int c)
{
	intMatrix* m = calloc(1,sizeof(intMatrix));
	m->r = r;
	m->c = c;
	m->m = (float**)calloc(r,sizeof(float*));
	for(int i=0; i<r; i++)
		m->m[i] = (float*)calloc(c,sizeof(float));
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
			mat->m[r][tmp->adj] = tmp->weight;
			tmp=tmp->next;
		}
		mat->m[r][tmp->adj] =tmp->weight;

	}
	return mat;
}