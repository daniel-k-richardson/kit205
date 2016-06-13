/*  graph.c
*  Purpose: Graph representation for neruons and connections.
*  Author: Daniel Richardson
*  Date: 15th May 2016
*/

#ifndef graph_h
#define graph_h

#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct edge {
    int toVertex;
    int weight;
}Edge;

typedef struct edgeNode {
    Edge edge;
    struct edgeNode *next;
} *EdgeList;

typedef struct graph {
    int V;
    int *state;
    EdgeList *edges;
} Graph;

Graph graph_init(int graph_size);
EdgeList create_edgelist(int to_vertex, int weight);
void add_edge(Graph self, int from_vertex, int to_vertex, int weight);
void print_graph(Graph self);
void destroy_graph(Graph *self);
void print_indegree(int indegrees[], int size);



#endif /* graph_h */
