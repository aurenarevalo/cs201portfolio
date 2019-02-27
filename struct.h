// #include "struct_p.h"
#include <stdlib.h>
#include <stdio.h>


typedef struct anode{
	int n;
	struct anode *next;
}adjNode;

typedef struct alist{
	adjNode *head;
}adjList;

typedef struct grph{
	int vertices;
	adjList *arr;
}Graph;

adjList* init_adjList(int n1);

void new_node(adjList** alist, int node);

Graph* init_Graph(int v);