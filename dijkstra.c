#include <stdio.h>
#include "graph.h"
#include <stdlib.h>

void findShortestPaths(char *inputFile, int ** PlayerToPlayerShortestPaths) {
    FILE *ifp = fopen(inputFile, "r");
    int num_nodes, num_edges;
    num_nodes = 1;
    Graph *g = GraphAlloc(num_nodes);
	/*
    for(int i = 0; i < num_edges; i++){
        int n1, n2;
        if(fscanf(ifp, "%d %d", &n1, &n2))
            GraphAddEdge(g, n1, n2);
    }
	*/
    GraphFree(g);
    fclose(ifp);
}


int main(int argc, char **argv) {
	if(argc != 3) {
        fprintf(stderr, "Please specify exactly two arguments\n");
        exit(1);
    }
        
    char* inputFile = argv[1];
    char* outputFile = argv[2];
	
    int numPlayers = 1;
    int **playerToPlayerShortestPaths = 
		(int **)malloc(numPlayers * numPlayers * sizeof(int));
	
    findShortestPaths(inputFile, playerToPlayerShortestPaths);

        
    // Output the first 5 sccs into a file.
    FILE *ofp = fopen(outputFile, "w");

    fprintf(ofp, "%d\t%d\t%d\t%d\t%d", 0,1,2,3,4);
    fclose(ofp);
    return 0;
}
