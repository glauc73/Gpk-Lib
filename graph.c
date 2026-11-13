#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint32_t uint;
#define EDGE_LEN 3


typedef struct{
    int id, weight;
}RelEdge;

typedef struct{
    int to, from, weight;
}AbsEdge;

typedef struct{
    RelEdge* neighbors;
    int num_neighbors;
}Vertex;

typedef struct{
    Vertex* at;
    int nV, nE;
}Graph;

typedef struct{
    AbsEdge* edges;
    int n_edges;
}GraphGenerator;

//o programador deve colocar sequencialmente 0, 1, .., N vertices.
//caso contrario, uma aresta nao declarada entre duas declaradas vai ser tida como existente, mas sem conexão
Graph* new_Graph(GraphGenerator* Gg){
    if(!Gg) return NULL;
    int n_edges = Gg->n_edges;

    Graph* gp = malloc(sizeof(Graph));
    if(!gp) return NULL;

    int max = 0;
    for(int i = 0; i < n_edges; i++){
        if(Gg->edges[i].to > max) max = Gg->edges[i].to;
        if(Gg->edges[i].from > max) max = Gg->edges[i].from;
    }
    gp->nV = max + 1;
    gp->nE = n_edges;

    gp->at = calloc(gp->nV , sizeof(Vertex));
    if(!gp->at){
        exit(1);
    }

    for(int i = 0; i < gp->nV; i++){
        gp->at[i].neighbors = calloc((gp->nV - 1),  sizeof(RelEdge));
        if(!gp->at[i].neighbors) return NULL;
    }
   
    for(int i = 0; i < gp->nE; i++){
        int from = Gg->edges[i].from;
        int to = Gg->edges[i].to;
        int w = Gg->edges[i].weight;

        int nb_from = gp->at[from].num_neighbors;
        int nb_to = gp->at[to].num_neighbors;

        gp->at[from].neighbors[nb_from].id = to;
        gp->at[to].neighbors[nb_to].id = from;
        
        gp->at[from].neighbors[nb_from].weight = w;
        gp->at[to].neighbors[nb_to].weight = w;

        gp->at[from].num_neighbors++;
        gp->at[to].num_neighbors++;
    }

    return gp;
}

void Gpk_free(Graph** gp){
    if(!gp || !*gp) return;
    for(int i = 0; i < (*gp)->nV; i++){
        free((*gp)->at[i].neighbors);
    }
    free((*gp)->at);
    free(*gp);
    *gp = NULL;
}

GraphGenerator* Gpk_GraphToGenerator(Graph* G){
    AbsEdge* A = calloc(G->nE, sizeof(AbsEdge));
    int Acont = 0;
    for(int i = 0; i < G->nV; i++){
        Vertex v_cur = G->at[i];
        for(int j = 0; j < G->at[i].num_neighbors; j++){
            if(v_cur.neighbors[j].id < i) continue;
            A[Acont].to = i;
            A[Acont].from = v_cur.neighbors[j].id;
            A[Acont].weight = v_cur.neighbors[j].weight;
            Acont++;
        }
    }

    GraphGenerator* Gg = malloc(sizeof(GraphGenerator));
    Gg->edges = A, Gg->n_edges = Acont;
    return Gg;
}

int main(){
    AbsEdge A[] = {
        {0, 1, 2},
        {0, 3, 7},
        {0, 4, 4},
        {1, 2, 8},
        {2, 3, 11},
        {1, 4, 7},
        {5, 7, 5}
    };

    GraphGenerator Gg = {
        A,
        sizeof(A) / sizeof(A[0])
    };

    Graph* gp = new_Graph(&Gg);
    
    for(int i = 0; i < gp->nV; i++){
        printf("%d : [", i);
        for(int j = 0; j < gp->at[i].num_neighbors; j++){
            printf("(%d, %d)", gp->at[i].neighbors[j].id, gp->at[i].neighbors[j].weight);
            printf("%s", j == gp->at[i].num_neighbors - 1 ? "" : " ; ");
        }
        printf("]\n");
    }

    Gg = *Gpk_GraphToGenerator(gp);

    for(int i = 0; i < Gg.n_edges; i++){
        printf("{%d, %d, %d}\n", Gg.edges[i].to, Gg.edges[i].from, Gg.edges[i].weight);
    }

    return 0;
}
