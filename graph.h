/**
 * File: graph.h
 * Aneesh Kulkarni, November 2011
 * ------------------------------
 * Defines the interface for the Graph type.
 *
 * The Graph manages a collection of vertices and edges 
 */

#ifndef _GRAPH_H
#define _GRAPH_H

#include <stdio.h>
#include <stdbool.h>

struct n;
struct e;

typedef struct e {
    struct n *node;
    int weight;
    struct e *next;
} Edge;


typedef struct n {
    int value, leader;
    bool visited;
    Edge *outgoing;
    Edge *incoming;
} Node;
// a node is represented by its integer value {1,2,...n}

typedef struct {
    int num_nodes;
    int num_edges;
    
    //points to heap array of Node *'s, nodes[i] points to ith node
    Node **nodes;
    Node **nodes_by_fvalue; 
} Graph;



/*
 * Function: GraphAlloc
 * Usage: Graph *mygraph = GraphAlloc(10);
 * ---------------------------------------
 * Creates a new empty graph, and returns a pointer to it.  The pointer points
 * to storage allocated in the heap.  Client should call GraphFree(mygraph) to
 * deallocate it.  An assert is raised if allocation fails.
 *
 * The nodes parameter indicates the initial number of nodes in the graph.
 * All nodes are initialized, and numbered consecutively {1,2,...,n}, n=nodes.
 */
Graph *GraphAlloc(int nodes);


/*
 * Function: GraphFree
 * Usage: GraphFree(mygraph);
 * ---------------------------------------
 * Deallocates heap memory used by the Graph.  Behavior undefined if mygraph is
 * not a valid pointer to a Graph.
 */
void GraphFree(Graph *g);


/*
 * Function: GraphAddNode
 * Usage: GraphAddNode(mygraph, 1);
 * ---------------------------------------
 * Adds a node to mygraph
 */

int GraphAddNode(Graph *g, int value);


/*
 * Function: GraphAddEdge
 * Usage: GraphAddEdge(mygraph, 2, 5);
 * ---------------------------------------
 * Adds an edge to mygraph connecting the specified nodes
 */
int GraphAddEdge(Graph *g, int n1, int n2);

void ReverseGraph(Graph *g);

void dfs(Graph *g, Node *i);

void dfsloop(Graph *g, bool is_first_call);

void PrintGraph(Graph *g, FILE *ofp);

void GraphReorderNodesByFValue(Graph *g);



#endif
