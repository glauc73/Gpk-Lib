#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Gpk.h"

//o programador deve colocar sequencialmente 0, 1, .., N vertices.
//caso contrario, uma aresta nao declarada entre duas declaradas vai ser tida como existente, mas sem conexão
Graph* new_Graph(GraphGenerator* Gg){
    if(!Gg) return NULL;
    int max = 0;
    for(int i = 0; i < Gg->n_edges; i++){
        if(Gg->edges[i].to > max) max = Gg->edges[i].to;
        if(Gg->edges[i].from > max) max = Gg->edges[i].from;
    }
    Graph* gp = new_blank_graph(max + 1);   // listas crescem sob demanda
    if(!gp) return NULL;
    for(int i = 0; i < Gg->n_edges; i++) Gpk_addEdge(gp, Gg->edges[i]);
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
        int max = 0;
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

void print_graph(Graph* gp){
    for(int i = 0; i < gp->nV; i++){
        printf("%d : [", i);
        for(int j = 0; j < gp->at[i].num_neighbors; j++){
            printf("(%d, %d)", gp->at[i].neighbors[j].id, gp->at[i].neighbors[j].weight);
            printf("%s", j == gp->at[i].num_neighbors - 1 ? "" : " ; ");
        }
        printf("]\n");
    }
    putchar('\n');
}

void print_graph_generator(GraphGenerator* Gg){
    for(int i = 0; i < Gg->n_edges; i++){
        printf("{%d, %d, %d}\n", Gg->edges[i].to, Gg->edges[i].from, Gg->edges[i].weight);
    }
    putchar('\n');
}

int qcmp(const void* a, const void* b){
    AbsEdge A = *(AbsEdge*)a;
    AbsEdge B = *(AbsEdge*)b;
    return A.weight - B.weight;
}

int Gpk_addEdge(Graph* G, AbsEdge edge){
    if(!G || !G->at) return 0;
    
    int to_nb = G->at[edge.to].num_neighbors;
    int from_nb = G->at[edge.from].num_neighbors;
    
    Vertex* v = &G->at[edge.from];
    if(v->num_neighbors >= v->cap){
        int new_cap = v->cap ? 2 * v->cap : 4;
        RelEdge* tmp = realloc(v->neighbors, new_cap * sizeof(RelEdge));
        if(!tmp) return 0;
        v->neighbors = tmp;
        v->cap = new_cap;
    }
    
    v = &G->at[edge.to];
    if(v->num_neighbors >= v->cap){
        int new_cap = v->cap ? 2 * v->cap : 4;
        RelEdge* tmp = realloc(v->neighbors, new_cap * sizeof(RelEdge));
        if(!tmp) return 0;
        v->neighbors = tmp;
        v->cap = new_cap;
    }
    G->at[edge.to].neighbors[to_nb].id = edge.from;
    G->at[edge.to].neighbors[to_nb].weight = edge.weight;
    G->at[edge.to].num_neighbors++;

    G->at[edge.from].neighbors[from_nb].id = edge.to;
    G->at[edge.from].neighbors[from_nb].weight = edge.weight;
    G->at[edge.from].num_neighbors++;

    G->nE++;
    return 1;
}

bool Gpk_hasCycle(Graph* G){
    if(!G || !G->at) exit(1);
    bool* removed = calloc(G->nV, sizeof(bool));
    bool* in_the_way = calloc(G->nV, sizeof(bool));
    int* next_neighboard = calloc(G->nV, sizeof(int));

    stack* ret = new_stack();
    bool res = false;
    //itera o algoritmo começando de todos os vertices (caso de ser desconexo)
    for(int s = 0; s < G->nV; s++){
        if(removed[s]) continue;
        int idx = s;
        in_the_way[idx] = true;

        while(1){
            bool jump = false;
            //olha todos os vizinhos
            for(int i = next_neighboard[idx]; i < G->at[idx].num_neighbors; i++){
                int neigh = G->at[idx].neighbors[i].id;
                
                if(removed[neigh]) continue; 
                //se o vizinho for o topo do retorno, voce volta de onde veio, o que nao deve acontecer
                if(!stk_isempty(ret) && stk_top(ret) == neigh) continue; //se a pilha de retorno estiver vazia vai pra qualquer um
                if(in_the_way[neigh]) {
                    res = true; //se o vizinho nao é o ultimo vertice mas ja foi visitado, temos um ciclo
                    goto fim;
                }
                jump = true;
                next_neighboard[idx] = i + 1;
                stk_push(ret, idx);
                idx = neigh;
                in_the_way[idx] = true;
                break;
            }
            if(!jump){
                //se nao teve saída, removemos o vertice do caminho e do grafo, e retornamos
                in_the_way[idx] = false;
                removed[idx] = true;
                if(stk_isempty(ret)) break;
                idx = stk_pop(ret);
            }
        }
    }     
    
fim:
    free_stk(&ret);
    free(in_the_way);
    free(next_neighboard);
    free(removed);
    return res;
}

Graph* new_blank_graph(int nV){
    if(nV <= 0) return NULL;
    Graph* G = malloc(sizeof(Graph));
    G->at = calloc(nV, sizeof(Vertex));

    for(int i = 0; i < nV; i++){
        G->at[i].cap = 4;
        G->at[i].neighbors = calloc(4, sizeof(RelEdge));
        if(!G->at[i].neighbors) {
            for(int k = 0; k < i; k++){
                free(G->at[k].neighbors);
                free(G->at);
                free(G);
            }
            return NULL;
        }
    }
    G->nV = nV;
    G->nE = 0;
    return G;
}


Graph* Gpk_Kruskal(Graph* G){
    if(!G || !G->at) return NULL;
    Graph* Min_Tree = new_blank_graph(G->nV);
    
    GraphGenerator* Gg = Gpk_GraphToGenerator(G);
    if(!Gg) return NULL;

    qsort(Gg->edges, Gg->n_edges, sizeof(AbsEdge), qcmp);

    for(int i = 0; i < Gg->n_edges; i++){
        if(G->nV - 1 == Min_Tree->nE) break;
        AbsEdge e = Gg->edges[i];
        if(!Gpk_addEdge(Min_Tree, e)) continue;
        if(Gpk_hasCycle(Min_Tree) > 0){
            Min_Tree->at[e.to].num_neighbors--;
            Min_Tree->at[e.from].num_neighbors--;
            Min_Tree->nE--;
        }
    }

    free(Gg->edges);
    free(Gg);
    return Min_Tree;
}

Graph* Gpk_Primm(Graph* G){
    if(!G || !G->at) return NULL;
    AbsEdge* edges = malloc((G->nV - 1) * sizeof(AbsEdge));
    
    GraphGenerator Gg = { 
        edges, 
        0,
    };

    bool* inT = calloc(G->nV, sizeof(bool));
    int* W = malloc(G->nV*sizeof(int)), nW = 1;
    W[0] = 0;
    inT[0] = true;

    while(nW < G->nV){
        AbsEdge e_min;
        int find = false;
        for(int i = 0; i < nW; i++){
            int u = W[i];
            for(int j = 0; j < G->at[u].num_neighbors; j++){
                int v = G->at[u].neighbors[j].id;
                int w = G->at[u].neighbors[j].weight;
                if(inT[v]) continue;
    
                if(!find || w < e_min.weight){
                    e_min.from = u;
                    e_min.weight = w;
                    e_min.to = v;
                    find = true;
                }
            }
        }
        if(!find) break;
        Gg.edges[Gg.n_edges++] = e_min;
        W[nW++] = e_min.to;
        inT[e_min.to] = true;
    }

    Graph* T = new_Graph(&Gg);
    
    free(edges);
    free(W);
    free(inT);

    return T;
}

