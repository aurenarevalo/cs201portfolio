// #include "struct_p.h"
#include <stdlib.h>
#include <stdio.h>

/*
* Structures for a node, adjacency list, and a directed, weighted graph
*/
#ifndef STRUCT_GUARD
#define STRUCT_GUARD
typedef struct anode{
	float weight;
	int adj;
	struct anode *next;
}adjNode;

typedef struct alist{
	adjNode *head;
}adjList;

typedef struct grph{

	int vertices;
	adjList *arr;
}Graph;

typedef struct imtrx{
	int r,c;
	float **m;
}intMatrix;

adjNode* init_adjNode();

adjList* init_adjList();

Graph* init_Graph();

void new_node(adjList* al, int to, float w);

void new_edge(Graph** gr,int from, int to, float w);

intMatrix* init_intMatrix(int r, int c);

intMatrix* construct_adj_matrix(Graph* gr);
#endif