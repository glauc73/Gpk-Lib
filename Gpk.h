#ifndef GPK_GRAPH_H
#define GPK_GRAPH_H

#include <stdint.h>
#include "stack.h"

typedef uint32_t uint;

typedef enum {false, true} bool;

typedef struct{
    int id, weight;
}RelEdge;

typedef struct{
    int to, from, weight;
}AbsEdge;

typedef struct{
    RelEdge* neighbors;
    int num_neighbors, cap;
}Vertex;

typedef struct{
    Vertex* at;
    int nV, nE;
}Graph;

typedef struct{
    AbsEdge* edges;
    int n_edges;
}GraphGenerator;

/* criação e destruição */
Graph* new_Graph(GraphGenerator* Gg);
Graph* new_blank_graph(int nV);
void Gpk_free(Graph** gp);

/* conversão */
GraphGenerator* Gpk_GraphToGenerator(Graph* G);

/* impressão */
void print_graph(Graph* gp);
void print_graph_generator(GraphGenerator* Gg);

/* auxiliares */
int qcmp(const void* a, const void* b);
int Gpk_addEdge(Graph* G, AbsEdge edge);
bool Gpk_hasCycle(Graph* G);

/* árvore geradora mínima */
Graph* Gpk_Kruskal(Graph* G);
Graph* Gpk_Primm(Graph* G);

#endif
