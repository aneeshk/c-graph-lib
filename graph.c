#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "graph.h"
#include <limits.h>

#define DEFAULT_NODES = 100;

int t;
int s;
bool is_first_call;

/* Function prototypes
 */
void GraphFreeNode(Node *n);
int helpAddEdge(Graph *g, Edge *list, int node, int val);


/* Implementations
 */

Graph *GraphAlloc()
{
    Graph *g = (Graph *)malloc(sizeof(Graph));
    assert(g != NULL);
    
    int nodes = 20;
    g->nodes = (Node **)malloc( (nodes+1) * sizeof(Node *));
    assert(g->nodes != NULL);

    g->nodes_by_fvalue = (Node **)malloc( (nodes+1) * sizeof(Node *));
    assert(g->nodes_by_fvalue != NULL);
    
    g->num_nodes_alloced = nodes;
    g->num_nodes = 0;
    g->num_edges = 0;
    
    return g;
}

void GraphRealloc(Graph *g, int new_size) 
{
  
    int old_size = g->num_nodes_alloced;
    Node ** temp = (Node **) malloc( (old_size+1) * sizeof(Node *));
    memcpy(temp, g->nodes, (old_size+1) * sizeof(Node *));
    g->nodes = (Node **)realloc(g->nodes, (new_size+1) * sizeof(Node *));
    memcpy(g->nodes, temp, (old_size+1) * sizeof(Node *));
    g->num_nodes_alloced = new_size;
}

void GraphFree(Graph *g)
{
    // by convention, we number nodes starting at 1
    for(int i=1; i <= g->num_nodes; i++) {
        GraphFreeNode(g->nodes[i]);
        free( g->nodes[i] );
    }
    free(g->nodes);

    free(g);
}

int GraphAddNode(Graph *g, int value)
{
    if(g->num_nodes >= g->num_nodes_alloced) {
	GraphRealloc(g, 2*g->num_nodes);
    }
    
    // don't add a node whose value already exists
    for(int i = 1; i <= g->num_nodes; i++) {
	if(g->nodes[i]->value == value)
	    return 1;
    }

    int i = g->num_nodes + 1;
    g->nodes[i] = (Node *)malloc(sizeof(Node));
    g->nodes[i]->value = value;
    g->nodes[i]->visited = false;
    g->nodes[i]->leader = 0;
    g->nodes[i]->distance = 0;
    g->nodes[i]->outgoing = NULL;
    g->nodes[i]->incoming = NULL;
    g->nodes[i]->shortestPath = NULL;

    g->num_nodes++;
    return 0;
}


int getNodeIndexByValue(Graph *g, int value) {
    for(int i = 1; i < g->num_nodes; i++) {
	if(g->nodes[i]->value == value) {
	    return i;
	}
    }

    return -1;
}


/* returns 1 on success, 0 on failure (example: repeating an edge)
 */
int GraphAddEdge(Graph *g, int val1, int val2)
{
    //disallow self-edges
    if(val1 == val2) {
	return 0;
    }

    int success = 1;
    
    int n1 = getNodeIndexByValue(g, val1);
    int n2 = getNodeIndexByValue(g, val2);
    
    
    if(n1 == -1 || n2 == -1) {
	return 0;
    }
    
    if ( (g->nodes[n1])->outgoing == NULL) { //initialize adj list
        Edge *new = (Edge *)malloc(sizeof(Edge));
        new->node = g->nodes[n2];
        new->next = NULL;
	new->weight = 1;
        (g->nodes[n1])->outgoing = new;
    }
    else { //iterate to end of adjacency list and add node
	success = helpAddEdge(g, (g->nodes[n1])->outgoing, n2, val2);
    }
    
    if ( (g->nodes[n2])->incoming == NULL) {
        Edge *new = (Edge *)malloc(sizeof(Edge));
        new->node = g->nodes[n1];
        new->next = NULL;
	new->weight = 1;
        (g->nodes[n2])->incoming = new;
    }
    else {
        success = helpAddEdge(g, (g->nodes[n2])->incoming, n1, val1);
    }

    if(success)
        g->num_edges += 1;
    return success;
}


/* returns 1 on success, 0 on failure (example: repeating an edge)
 */
int GraphAddShortestPath(Graph *g, int pathTo, int via, int dijkstra_starting_node)
{
    if(pathTo == dijkstra_starting_node)
      return 0;

    int success = 1;
    
    int n1 = getNodeIndexByValue(g, pathTo);
    int n2 = getNodeIndexByValue(g, via);
    
    if(n1 == -1 || n2 == -1) {
	return 0;
    }
    
    if ( (g->nodes[n1])->shortestPath == NULL) { //initialize adj list
        Edge *new = (Edge *)malloc(sizeof(Edge));
        new->node = g->nodes[n2];
        new->next = NULL;
	new->weight = 1;
        (g->nodes[n1])->shortestPath = new;
    }
    else { //iterate to end of adjacency list and add node
	success = helpAddEdge(g, (g->nodes[n1])->shortestPath, n2, via);
    }
    
    //now add shortest path to get to via ... until via == pathTo
    if(g->nodes[n2]->shortestPath != NULL && g->nodes[n2]->shortestPath->node->value != pathTo)
      GraphAddShortestPath(g, via, g->nodes[n2]->shortestPath->node->value, dijkstra_starting_node);
    
    return success;
}


void ReverseGraph(Graph *g) {
    Edge *temp;
    for(int i=1; i <= g->num_nodes; i++) {
        temp = g->nodes[i]->incoming;
        g->nodes[i]->incoming = g->nodes[i]->outgoing;
        g->nodes[i]->outgoing = temp;
    }
}


void dfs(Graph *g, Node *i) {
    i->visited = true;
    if(!is_first_call)
        i->leader = s;
    
    Edge *temp = i->outgoing;
    while(temp != NULL) {
        if(temp->node->visited == false) {
            //dfs(g, temp->node, is_first_call);
            temp->node->visited = true;
            if(!is_first_call)
                temp->node->leader = s;
            Edge *temp2 = temp->node->outgoing;
            while(temp2 != NULL) {
                if(temp2->node->visited == false)
                    dfs(g, temp2->node);
                temp2 = temp2->next;
            }
            if(is_first_call) {
                t++;
                g->nodes_by_fvalue[t] = temp->node;
            }
        }
        temp = temp->next;
    }

    if(is_first_call) {
        t++;
        g->nodes_by_fvalue[t] = i;
    }
}

bool allConnectedNodesAreVisited(Graph *g) {
  for(int i=1; i < g->num_nodes; i++) {
    if(g->nodes[i]->visited == false) {
      return false;
    }
  }
  return true;
}

int getUnvisitedNodeWithSmallestDistance(Graph *g) {
  int minDistanceSoFar = INT_MAX;
  int nodeWithMinDistance = 0;

  for(int i=1; i < g->num_nodes; i++) {
    if(g->nodes[i]->visited == false && g->nodes[i]->distance < minDistanceSoFar && g->nodes[i]->distance > 0) {
      minDistanceSoFar = g->nodes[i]->distance;
      nodeWithMinDistance = i;
    }
  }

  return nodeWithMinDistance;

}

void dijkstraloop(Graph *g, Node *i) {
  for(int j=1; j < g->num_nodes; j++) {
      g->nodes[j]->visited = false;
      g->nodes[j]->distance = 0;
  }
  
  dijkstra(g, i, 0, i->value);

}

void dijkstra(Graph *g, Node *i, int cur_distance, int starting_node) {
    if(allConnectedNodesAreVisited(g))
      return;

    Edge *temp = i->outgoing;
    while(temp != NULL) {
	if(temp->node->visited == false) {
	  if(temp->node->distance == 0 || temp->node->distance > cur_distance + 1) {
	    
	    temp->node->distance = cur_distance + 1;
	    GraphAddShortestPath(g, temp->node->value, i->value, starting_node);
	  }
	}
	temp = temp->next;
    }
    i->visited = true;

    int index = getUnvisitedNodeWithSmallestDistance(g);
    if(index == 0) return; // all nodes are visited
    dijkstra(g, g->nodes[index], g->nodes[index]->distance, i->value);
}

void dfsloop(Graph *g, bool first) {
    t = 0;
    s = 0;
    is_first_call = first;
    for(int i = g->num_nodes; i >= 1; i--) {
        if (g->nodes[i]->visited == false) { //not yet visited
            s = g->nodes[i]->value;
            dfs(g, g->nodes[i]);
        }
    }
    for(int i = g->num_nodes; i >= 1; i--) {
        g->nodes[i]->visited = false;
    }
    s = 0;

}

void GraphReorderNodesByFValue(Graph *g) {
    free(g->nodes);    
    g->nodes = g->nodes_by_fvalue;
}


/*
 * Helper Functions
 */
void GraphFreeNode(Node *n)
{
    assert(n != NULL);

    Edge *cur = n->outgoing;
    Edge *temp;
    while(cur != NULL) {
        temp = cur->next;
        free(cur);
        cur = temp;
    }

    cur = n->incoming;
    while(cur) {
        temp = cur->next;
        free(cur);
        cur = temp;
    }
}


int helpAddEdge(Graph *g, Edge *list, int node_index, int val)
{
    Edge *prev = list;
    
    while(list != NULL) {
        if(list->node->value == val) { // prevents duplicate edges
	    list->weight++;
	    return 0;
	}
        prev = list;
        list = list->next;
    }
        
    Edge *new = (Edge *)malloc(sizeof(Edge));
    new->node = g->nodes[node_index];
    new->next = NULL;
    new->weight = 1;
    prev->next = new;
    return 1;
}


void PrintGraph(Graph *g, FILE *ofp)
{
    fprintf(ofp, "%d\n%d\n", g->num_nodes, g->num_edges);
    printf("g has %d nodes and %d edges\n", g->num_nodes, g->num_edges);
    for(int i=1; i < g->num_nodes; i++) {

        Edge *temp = g->nodes[i]->outgoing;

        while(temp != NULL) {
            fprintf(ofp, "%d %d\n", g->nodes[i]->value, temp->node->value);
            temp = temp->next;
        }
    }
}
