// #include "struct_p.h"
#include <stdlib.h>
#include <stdio.h>

/*
* Structures for a node, adjacency list, and a directed, weighted graph
*/
#ifndef STRUCT_GUARD
#define STRUCT_GUARD
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

typedef struct imtrx{
	int r,c;
	int **m;
}intMatrix;

adjList* init_adjList();

Graph* init_Graph();

void new_node(adjList* al, int to);

void new_edge(Graph** gr,int from, int to);

intMatrix* init_intMatrix(int r, int c);

intMatrix* construct_adj_matrix(Graph* gr);
#endif