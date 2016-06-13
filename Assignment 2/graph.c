/*  graph.c
 *  Purpose: Graph representation for neruons and connections.
 *  Author: Daniel Richardson
 *  Date: 15th May 2016
 */

#include "graph.h"
#include "queue.h"

int *clone_array(int target_to_clone[], int size);

/* create_graph: This creates a graph by assigning a size to
 * the array of EdgeList. Each index inside the array represents a single
 * vertex in the graph. Each EdgeList array index is set to NULL and will
 * later store a linked list for each edge that it is connected to.
 *
 * Params: V is the number of vertexes in our graph.
 *
 * Return: returns
 */
Graph graph_init(int graph_size)
{
    /* create a new graph, set size of vertexes in the graph */
    Graph new_graph;

    new_graph.V = graph_size;
    /* create an array of EdgeLists the side of v */
    new_graph.edges = malloc(graph_size * sizeof(struct edge));
    new_graph.state = malloc(graph_size * sizeof(int));

    /* set each array the starting value of NULL */
    for (int i = 0; i < graph_size; i++) {
        new_graph.edges[i] = NULL;
        new_graph.state[i] = 0;
    }
    return (new_graph);
}

/* create_edgelist: this function creates a node that stores
 * the Edge (toVertex and it's weight).
 *
 * Params: to_vertex is the vertex that the current vertex is connected.
 * the weight is the weight value of the edge connecting the current vertex
 * and the to_vertex.
 *
 * Returns: the EdgeList node that will be linked to the appropriate linked_list.
 */
EdgeList create_edgelist(int to_vertex, int weight)
{
    EdgeList new_edge = NULL;

    /* create a new EdgeList node and assign it's values */
    new_edge = malloc(sizeof(struct edgeNode));
    new_edge->edge.toVertex = to_vertex;
    new_edge->edge.weight = weight;
    new_edge->next = NULL;

    return (new_edge);
}

/* add_edge: adds an Edge(node) to the EdgeList(link-list) under the correct
 * vertex(array index) to show the link between vertexes.
 *
 *  Params: self is the graph to add to. from_vertex is the index representing
 *  the vertex. to_vertex is the vertex that is connected to the current vertex
 *  and weight is the weight of that edge.
 *
 *  Returns: void.
 */
void add_edge(Graph self, int from_vertex, int to_vertex, int weight)
{
    if (to_vertex != -1) {
        EdgeList new_edge = create_edgelist(to_vertex, weight);
        new_edge->next = self.edges[from_vertex];
        self.edges[from_vertex] = new_edge;
    }
}


/* print_graph: goings through each index in the array and displays all the
 * edges between each vertex. (just using this for a visual understanding).
 *
 * Params: the graph to print.
 *
 * Returns: void.
 */
void print_graph(Graph self)
{
    for (int vertex = 0; vertex < self.V; vertex++) {
        EdgeList current = self.edges[vertex];
        
        if (current != NULL)
            printf("\n Adjacency list of vertex %d\n ", vertex);
        
        while (current != NULL) {
            printf("-(W:%d)-> %d", current->edge.weight, current->edge.toVertex);
            current = current->next;
        }
    }
    printf("\n");
}

/*  print_indegree: displays the indegree of the each vertex.
 *
 *  Params: an in degree array to traverse.
 *
 *  Returns: void.
 */
void print_indegree(int indegrees[], int size) {
    for (int i = 0; i < size;  i++) {
        printf(" %d has %d in degrees\n", i,  indegrees[i]);
    }
}

/* destroy: frees all the graphs edges.
 *
 * Params: a point to the graph to destroy.
 *
 * Returns: void.
 */
void destroy_graph(Graph *self)
{
    for (int v = 0; v < (*self).V; v++) {
        EdgeList current = (*self).edges[v] ;

        while (current != NULL) {
            (*self).edges[v] = current->next;
            free(current);
            current = (*self).edges[v];
        }
    }
    self = NULL;
}
