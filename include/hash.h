#ifndef _HASH_H_
#define _HASH_H_

#include <stdlib.h>
#include <inttypes.h>


typedef struct{
    uint64_t* tab;
    size_t tcap;
}hash_t;

static inline hash_t* new_hash(int M){
    hash_t* ht = (hash_t*)malloc(sizeof(*ht));
    if(!ht) return NULL;
    ht->tcap = M;
    ht->tab = (uint64_t*)calloc(M, sizeof(uint64_t));
    if(!ht->tab) {free(ht); return NULL;}

    return ht;
}

static inline void free_hash(hash_t** ht){
    if(!ht || !*ht) return;
    free((*ht)->tab);
    free(*ht);
    *ht = NULL;
}

static inline int set_pair(hash_t* ht, int a, int b, int N){
    size_t key = (size_t)a*N + b + 1;
    size_t i = key % ht->tcap;
    
    while(ht->tab[i]){
        if(ht->tab[i] == key) return 0;
        i = (i + 1) % ht->tcap;
    }
    ht->tab[i] = key;
    return 1;
}

#endif
