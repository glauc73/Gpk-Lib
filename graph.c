#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint32_t uint;
#define EDGE_LEN 3


typedef struct{
    int id, weight;
}Edge;

typedef struct{
    Edge* neighbors;
    int num_neighbors;
}Vertex;

typedef struct{
    Vertex* at;
    int num_vertex;
}Graph;

Graph* new_Graph(int n_vertex, uint Gg[][EDGE_LEN]){
    Graph* gp = malloc(sizeof(Graph));
    if(!gp) return NULL;

    gp->num_vertex = n_vertex;
    gp->at = calloc(n_vertex, sizeof(Vertex));
    if(!gp->at){
        exit(1);
    }

    for(int i = 0; i < n_vertex; i++){
        gp->at[i].neighbors = calloc((n_vertex - 1),  sizeof(Edge));
        if(!gp->at[i].neighbors) return NULL;
    }
   
    for(int i = 0; i < n_vertex; i++){
        int from = Gg[i][0];
        int to = Gg[i][1];
        int w = Gg[i][2];

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
    for(int i = 0; i < (*gp)->num_vertex; i++){
        free((*gp)->at[i].neighbors);
    }
    free((*gp)->at);
    free(*gp);
    *gp = NULL;
}

int main(){
    uint a[][EDGE_LEN] = {
        {0, 1, 2},
        {0, 3, 7},
        {0, 4, 4},
        {1, 2, 8},
        {2, 3, 11}
    };
    Graph* gp = new_Graph(5, a);
    
    for(int i = 0; i < gp->num_vertex; i++){
        printf("%d : [", i);
        for(int j = 0; j < gp->at[i].num_neighbors; j++){
            printf("(%d, %d) ; ", gp->at[i].neighbors[j].id, gp->at[i].neighbors[j].weight);
        }
        printf("]\n");
    }

    return 0;
}
