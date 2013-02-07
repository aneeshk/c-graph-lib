#include <stdio.h>
#include "graph.h"
#include <stdlib.h>

void findShortestPaths(FILE *ifp, FILE *ofp) {

    int num_nodes;
    num_nodes = 1;
    Graph *g = GraphAlloc(num_nodes);
    
    for(int i=0; i < 2500; i++) {
	int match;
	int p[22];

	if(fscanf(ifp, "%d,%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
		  &match, p, p+1, p+2, p+3, p+4, p+5, p+6, p+7, p+8, p+9, p+10, p+11, p+12, p+13, p+14, p+15, p+16, p+17, p+18, p+19, p+20, p+21)) {

	    for(int j = 0; j < 22; j++) {
		GraphAddNode(g, p[j]);
		for(int k = 0; k < j; k++) {
		    GraphAddEdge(g, p[j], p[k]);
		    GraphAddEdge(g, p[k], p[j]);
		}
	    }
	}
	else { break; }

	if(i % 100 == 0) printf("finished %d matches\n", i);
    }
    
    printf("ready to print graph\n");
    PrintGraph(g, ofp);
    GraphFree(g);
}


int main(int argc, char **argv) {
	if(argc != 3) {
        fprintf(stderr, "Please specify exactly two arguments\n");
        exit(1);
    }
        
    char* inputFile = argv[1];
    char* outputFile = argv[2];

    FILE *ofp = fopen(outputFile, "w");
    FILE *ifp = fopen(inputFile, "r");
	
    findShortestPaths(ifp, ofp);

    //    fprintf(ofp, "%d\t%d\t%d\t%d\t%d", 0,1,2,3,4);
    fclose(ifp);
    fclose(ofp);
    return 0;
}
