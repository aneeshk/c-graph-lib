#include <stdio.h>
#include "graph.h"
#include <stdlib.h>

void findShortestPaths(FILE *ifp, FILE *ofp) {

    int num_nodes;
    num_nodes = 1;
    Graph *g = GraphAlloc(num_nodes);
	/*
    for(int i = 0; i < num_edges; i++){
        int n1, n2;
        if(fscanf(ifp, "%d %d", &n1, &n2))
            GraphAddEdge(g, n1, n2);
    }
	*/
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
