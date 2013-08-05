#include <stdio.h>
#include "graph.h"
#include <stdlib.h>

void findShortestPaths(FILE *ifp, FILE *ofp, FILE *dij_fp) {

    int num_matches = 0;
    Graph *g = GraphAlloc();
    
    //construct graphs
    for(int i=0; i < 2076; i++) {
	int match;
	int p[22];

	if(fscanf(ifp, "%d,%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
		  &match, p, p+1, p+2, p+3, p+4, p+5, p+6, p+7, p+8, p+9, p+10, p+11, p+12, p+13, p+14, p+15, p+16, p+17, p+18, p+19, p+20, p+21)) {
	    num_matches++;
	    for(int j = 0; j < 22; j++) {
		GraphAddNode(g, p[j]);
		for(int k = 0; k < j; k++) {
		    GraphAddEdge(g, p[j], p[k]);
		    GraphAddEdge(g, p[k], p[j]);
		}
	    }
	}
	else { break; }
    }

    
    PrintGraph(g, ofp);

    for(int k = 1; k < g->num_nodes; k++) {
      printf("running dijkstra from node %d of %d\n", k, g->num_nodes);
      dijkstraloop(g, g->nodes[k]);
      for(int m = 1; m < g->num_nodes; m++) {
	fprintf(dij_fp, "Shortest path from %d to %d is of length %d.\n", g->nodes[k]->value, g->nodes[m]->value, g->nodes[m]->distance);
      }
    }
    GraphFree(g);
}


int main(int argc, char **argv) {
	if(argc != 4) {
        fprintf(stderr, "Please specify exactly three arguments\n");
        exit(1);
    }
        
    char* inputFile = argv[1];
    char* outputFile = argv[2];
    char* dijkstraFile = argv[3];

    FILE *ofp = fopen(outputFile, "w");
    FILE *ifp = fopen(inputFile, "r");
    FILE *dij_fp = fopen(dijkstraFile, "w");
	
    findShortestPaths(ifp, ofp, dij_fp);

    //    fprintf(ofp, "%d\t%d\t%d\t%d\t%d", 0,1,2,3,4);
    fclose(ifp);
    fclose(ofp);
    return 0;
}
