#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "graph.h"

#define DEFAULT_NODES = 100;

int t;
int s;
bool is_first_call;

/* Function prototypes
 */
void GraphFreeNode(Node *n);
int helpAddEdge(Graph *g, Edge *list, int node);


/* Implementations
 */

Graph *GraphAlloc()
{
    Graph *g = (Graph *)malloc(sizeof(Graph));
    assert(g != NULL);
    
    int nodes = 100;
    g->nodes = (Node **)malloc( (nodes+1) * sizeof(Node *));
    assert(g->nodes != NULL);

    g->nodes_by_fvalue = (Node **)malloc( (nodes+1) * sizeof(Node *));
    assert(g->nodes_by_fvalue != NULL);
    
    g->num_nodes_alloced = nodes;
    g->num_nodes = 0;
    g->num_edges = 0;
    
    return g;
}

void *GraphRealloc(Graph *g, int new_size) 
{
    Node ** temp = g->nodes;
    int old_size = g->num_nodes;
    g->nodes = (Node **)realloc(g->nodes,  (new_size+1) * sizeof(Node *));
    memcpy(g->nodes, temp, old_size * sizeof(Node *));
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
    int i = g->num_nodes + 1;
    g->nodes[i] = (Node *)malloc(sizeof(Node));
    g->nodes[i]->value = value;
    g->nodes[i]->visited = false;
    g->nodes[i]->leader = 0;
    g->nodes[i]->outgoing = NULL;
    g->nodes[i]->incoming = NULL;

    g->num_nodes++;
    return 0;
}


/* returns 1 on success, 0 on failure (example: repeating an edge)
 */
int GraphAddEdge(Graph *g, int n1, int n2)
{
    //self-edges are ok
    int success = 1;
    
    if ( (g->nodes[n1])->outgoing == NULL) { //initialize adj list
        Edge *new = (Edge *)malloc(sizeof(Edge));
        new->node = g->nodes[n2];
        new->next = NULL;
	new->weight = 1;
        (g->nodes[n1])->outgoing = new;
    }
    else { //iterate to end of adjacency list and add node
        success = helpAddEdge(g, (g->nodes[n1])->outgoing, n2);
    }

    if ( (g->nodes[n2])->incoming == NULL) {
        Edge *new = (Edge *)malloc(sizeof(Edge));
        new->node = g->nodes[n1];
        new->next = NULL;
	new->weight = 1;
        (g->nodes[n2])->incoming = new;
    }
    else {
        success = helpAddEdge(g, (g->nodes[n2])->incoming, n1);
    }

    if(success)
        g->num_edges += 1;
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


int helpAddEdge(Graph *g, Edge *list, int node)
{
    Edge *prev = list;
    while(list != NULL) {
        if(list->node->value == node) { // prevents duplicate edges
	    list->weight++;
	    return 0;
	}
        prev = list;
        list = list->next;
    }
        
    Edge *new = (Edge *)malloc(sizeof(Edge));
    new->node = g->nodes[node];
    new->next = NULL;
    new->weight = 1;
    prev->next = new;
    return 1;
}


void PrintGraph(Graph *g, FILE *ofp)
{
    fprintf(ofp, "%d\n%d\n", g->num_nodes, g->num_edges);
    for(int i=1; i <= g->num_nodes; i++) {
        Edge *temp = g->nodes[i]->outgoing;
        while(temp != NULL) {
            fprintf(ofp, "%d %d\n", g->nodes[i]->value, temp->node->value);
            temp = temp->next;
        }
    }
}
