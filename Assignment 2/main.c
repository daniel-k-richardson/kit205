/* main.c
* Purpose: Assignment 2 (Part A) for kit205, calculate the output of neurons
* using a directed graph (DAG) where a vertex represents individual neurons and
* Edges represent the connection between each neuron.
*
* Author: Daniel Richardson
* Date: 15th May 2016
*/

#include "graph.h"

int *get_indegree(Graph self);
void populate_graph(Graph self);
void update_state(Graph self, int sorted_graph[], int in_degrees[]);
int *clone_array(int target_to_clone[], int size);
void print_graph_state(Graph self);
int *topological_sort(Graph self, int indegrees[]);


int main()
{
	int graph_size;
	int *indegrees;
	int *sorted_graph;

	/* Number of vertexs for graph taken from text input. */
	scanf("%d", &graph_size);

	/* Create a graph, populate it and display the graph. */
	Graph graph = graph_init(graph_size);
	populate_graph(graph);
	print_graph(graph);

	/* Get the indegrees of the vertexs in the graph and display it. */
	indegrees = get_indegree(graph);
	//print_indegree(indegrees, graph.V);

	/* Topologically sort the graph in order of depenence. */
	sorted_graph = topological_sort(graph, indegrees);

	/* updates the neuron state in the graph and displays the final result. */
	update_state(graph, sorted_graph, indegrees);
	print_graph_state(graph);

	/* Program is finished and so do some house cleaning. */
	destroy_graph(&graph);
	free(indegrees);
	free(sorted_graph);

	return EXIT_SUCCESS;
}


/* populate_graph: Populates the graph via text file input
 *
 * Params: the graph to populate and the size of the graph
 *
 * Returns: void.
 */
void populate_graph(Graph self)
{
	const int NO_OUT_EDGES = -1;
	const int NO_WEIGHT = 0;

	int state;
	int outdegrees;
	int to_vertex;
	int weight;

	/* Currently have an empty graph of vertex, loop through each
	 * vertex to assign each vertex its values from the input file,
	 * such as its state and what edges it has.
	 */
	for (int i = 0; i < self.V; i++) {
		scanf("%d ", &state);
		self.state[i] = state;
		scanf("%d", &outdegrees);

		/* loops through the number of out degree edges the vertex has
		 * so that the edge, the weight and what the edge connects to
		 * can be added to the vertex's edge-list for later use. 
		 */
		for (int j = 0; j < outdegrees; j++) {
			scanf("%d, %d", &to_vertex, &weight);
			add_edge(self, i, to_vertex, weight);
		}
	}
}


/* indegree: gets the in degrees for each vertex (number of edges pointing to it).
 *
 * Params: the graph to get the in-degrees from.
 *
 * Returns: an array of in degrees.
 */
int *get_indegree(Graph self)
{
	EdgeList current;	// used to traverse through the vertex's edge lists.
	int *indegree = malloc(self.V * sizeof(int));

	/* when malloc'd the array it is filled 
	 * with junk values and I want to increment by
	 * 1 each time an edge points to a vertex.  
	 */
	for (int i = 0; i < self.V; i++) {
		indegree[i] = 0;
	}

	/* Each vertex is an index position in an array (Graph). To
	 * get the indegree, go through all the vertex's edge-list
	 * to find the toVertex variable and use that to increment
	 * the indgree array at the toVertex position.
	 * i.e if toVertex = 3 then indegree[3]++ 
	 */
	for (int i = 0; i < self.V; i++) {
		current = self.edges[i];

		while (current != NULL) {
			indegree[current->edge.toVertex]++;
			current = current->next;
		}
	}
	return indegree;
}

/* topological_sort: Creates an array of vertexes in order of dependence,
 * such that the first vertex (A) comes before vertex (B) where (B) depends on
 * (A).
 *
 * Params: the graph to sort and an array of in_degrees for each vertex.
 *
 * Returns: a topologically sorted array of vertexes.
 */
int *topological_sort(Graph self, int indegrees[])
{
	/* As we go through the indegrees and start decrementing 
	 * the indegrees as we discover their edges we need to distinguish
	 * between vertexs that have just been discovered (to add to our queue)
	 * and the ones that have already been in our queue or are currently in
	 * the queue before we check for indgrees again. I'm using the DISCOVERED
	 * so that we can ignore it already discovered vertex edges. */
	const int DISCOVERED = -1;
	int array_size = self.V;

	/* Cloning indegrees because when this function is finished all indexes will
	 * have the value -1 and  the original values in indegrees are needed later 
	 * for updating the graph state. */
	int *indegree_clone = clone_array(indegrees, array_size);
	int *sorted_graph = malloc(array_size * sizeof(int));

	EdgeList current;
	Queue queue = queue_init(array_size);

	for (int vertex = 0; vertex < array_size; vertex++) {
		/* At this point we don't know how many vertexes have an indegree of 0
		 * so check all the vertex indegrees. This is inside the outer for loop
		 * because we need to check if any new vertex indegrees are 0 once they
		 * have been decremented */
		for (int edge = 0; edge < array_size; edge++) {
			if (indegree_clone[edge] == 0) {
				insert(&queue, edge);
				indegree_clone[edge] = DISCOVERED;
			}
		}
		/* After the first pass of getting the vertexs that have no indegrees.
		 * The queue should have at least one vertex to start (if a correct DAG).
		 * If the queue is empty we have a cycle and topological sort fails.
		 *  
		 * The logic that follows is: remove the vertex with 0 and traverse its
		 * edge list to find what vertexes it has an edge to. I've done it this 
		 * way because we can use the toVertex value as the index position for 
		 * the indegree array and decrement it by 1. Once it's finished we can 
		 * had the vertex to the sorted graph which can later be used as an index.
		 */
		if (!is_empty(queue)) {
			sorted_graph[vertex] = remove_from_queue(&queue);
			current = self.edges[sorted_graph[vertex]];

			while (current != NULL) {
				indegree_clone[current->edge.toVertex]--;
				current = current->next;
			}
		}
	}

	free(indegree_clone);
	destory_queue(queue);
	return (sorted_graph);
}

/* update_state: update the vertex states: for each vertex in the graph find the
* edges that are connected to it and keep a running tally of the weight
* multiplied by the state. When no more edges to the vertex exist, check the
* tally. if the tally is positive assign 1 to the correct vertex position in
* the graphs state array. Otherwise assign 0. If there are no indegrees leave
* the state as it is.
*
* Params: the graph to update the state of. The topologically sorted graph array
* An array of indegrees.
*
* Returns: void.
*/
void update_state(Graph self, int sorted_graph[], int indegrees[])
{
	int input;
	int vertex;
	int num_indegree;
	int place_to_search;
	EdgeList current = NULL;

	/* Traverse through the topologically sorted graph array because it should be in order
	   of dependency which means that at any given point, vertex n should only have edges
	   pointing to it from the previous vertexs. For example, the vertex found at the index
	   position 2 (in sorted graph) should only have in edges from vertexs stored in 0 and 1.*/
	for (int i = 0; i < self.V; i++) {
		vertex = sorted_graph[i];
		num_indegree = indegrees[vertex];
		input = 0;

		/* Searches all the vertexes edges by selecting all the vertexs between the ranges of
		 * 0 up to the current vertex position in the topoglically sorted graph array. Using
		 * the value of i keeps track of where we are in the topologically sorted graph array */
		for (int j = 0; j < i; j++) {
			place_to_search = sorted_graph[j];
			current = self.edges[place_to_search];

			/* Traverse the edge-list of the vertexes to find edges to target vertex.
			 * If there is an edge to the target vertex update input to edge weight * state
			 * for every edge to the target vertex. */
			while (current != NULL) {
				if (current->edge.toVertex == vertex) {
					input += (current->edge.weight * self.state[place_to_search]);
				}
				current = current->next;
			}
		}
		/* Added the extra check before updating vertex state because there are occasions when
		 * the vertex state should not be changed (if the vertex has no indegrees state should
		 * keep its original state value). However, if this is removed input is assigned to 0 
		 * before each inner loop (to find edges to vertexs) if the indegrees is 0 the inner
		 * loop is skipped and so input is always 0. A vertex state will only be 1 if all edges
		 * connected to it are also one. If our starting vertex state is set to 0 all vertex
		 * state values will become 0. */ 
		if (num_indegree != 0) {
			if (input > 0) {
				self.state[vertex] = 1;
			}
			else {
				self.state[vertex] = 0;
			}
		}
	}
}

/* clone_array: this is a helper function used by topological sort, where an
* array of vertex in degrees is needed modified and discarded when the sort is
* finished.
*
* Params: the array to clone and the size of the array.
*
* Returns: the cloned array.
*/
int *clone_array(int target_to_clone[], int size)
{
	int *clone = malloc(size * sizeof(int));

	for (int i = 0; i < size; i++) {
		clone[i] = target_to_clone[i];
	}

	return (clone);
}

/* print_graph_state: prints the state (1/0) for each vertex in the graph.
*
* Params: graph to traverse through the states of.
*
* Returns: void.
*/
void print_graph_state(Graph self)
{
	printf("\n");
	for (int i = 0; i < self.V; i++) {
		printf("%d", self.state[i]);
	}
	printf("\n");
}
