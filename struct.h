// #include "struct_p.h"
#include <stdlib.h>
#include <stdio.h>

/*
* Structures for a node, adjacency list, and a directed, weighted graph
*/
typedef struct anode{
	int adj;
	struct anode *next;
}adjNode;

typedef struct alist{
	adjNode *head;
}adjList;


typedef struct grph{
	int weight;
	int vertices;
	adjList *arr;
}Graph;

adjList* init_adjList();

Graph* init_Graph();

void new_node(adjList* al, int to);

void new_edge(Graph** gr,int from, int to);

int** init_matrix(int r, int c);

int** construct_adj_matrix(Graph* gr);