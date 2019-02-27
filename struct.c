#include "struct.h"

typedef struct anode adjNode;
typedef struct alist adjList;
typedef struct grph Graph;

/*
*	initialize adjacency list, set initial (head) node
*/
adjList* init_adjList(int n1)
{
	adjList *alist;
	alist = malloc(sizeof(adjList));
	alist->head = malloc(sizeof(adjNode));
	// alist->head->n = n1;
	alist->head->next = NULL;
	return alist;
}


void new_node(adjList** alist, int node)
{
	adjNode* n = malloc(sizeof(adjNode));
	adjList* tmplist = *alist;
	adjNode* tmpnode = tmplist->head;
	adjNode* tmp2 = tmplist->head;
	while(tmpnode->next != NULL){
		printf("%d\n",tmpnode->n);
		tmpnode = tmpnode->next;
	}
	n->n = node;
	n->next = NULL;
	tmpnode->next = n;
	tmplist->head = tmp2;
}
/*
*	Initialize graph. vertices determines size of graph (adjList)
*/
Graph* init_Graph(int v)
{
	Graph* g = malloc(sizeof(Graph));
	g->arr = calloc(v,sizeof(adjList));
	for(int i=0;i<v;i++)
	{
		// g->arr
	}
}