#ifndef _GPKRAND_H_
#define _GPKRAND_H_

#include <stdlib.h>
#include <stdint.h>
#include "Gpk.h"
#include "hash.h"

static inline int rnd(int N);
static inline Graph* Gpk_RandomConnected(int nV, int nE, int maxWeight, int seed);

static inline int rnd(int N){
    long long r = ((rand() & 0x7FFF) << 15) | (rand() & 0x7FFF);
    return r % N;
}

static inline Graph* Gpk_RandomConnected(int nV, int nE, int maxWeight, int seed){
    if(nV < 1 || maxWeight < 1) return NULL;
    srand(seed);

    long long maxnE = (long long)(nV * (nV - 1)) >> 1;
    if(nE < nV - 1) nE = nV - 1; //minimo pra ser conectado
    if(nE > maxnE) nE = maxnE; //grafo completo, sem arestas duplas ou multiplas
    hash_t* ht = new_hash(2*nE + 1);
    Graph* G = new_blank_graph(nV);
    if(!G || !ht) {free_hash(&ht); Gpk_free(&G); return NULL;}

    //cria a arvore inicial pra garantir que o grafo é conectado
    for(int i = 1; i < nV; i++){
        int j = rnd(i);
        set_pair(ht, j, i, nV); // j < i
        AbsEdge e = {i, j, 1 + rnd(maxWeight)};
        Gpk_addEdge(G, e);
    }

    //sorteia as arestas restantes
    while(G->nE < nE){
        int a = rnd(nV), b = rnd(nV);
        if(a == b) continue;
        if(a > b){int t = a; a = b; b = t;}
        if(!set_pair(ht, a, b, nV)) continue;
        AbsEdge e = {a, b, 1 + rnd(maxWeight)};
        Gpk_addEdge(G, e);
    }
    free_hash(&ht);    
    return G;
}


#endif
