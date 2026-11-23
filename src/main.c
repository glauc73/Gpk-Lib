#include "../include/Gpk_rnd.h"
#include <stdio.h>
#include <time.h>

#define SEED 13
#define REPS 6
#define SIZE(X) ((int)(sizeof(X) / sizeof((X)[0])))

int main(){
    int nv[] = {20, 100, 200, 500, 1000, 2000};
    float density[] = {0.1, 0.25, 0.5, 0.75, 1};

    FILE* fp = fopen("primm.csv", "w");
    FILE* fk = fopen("kruskal.csv", "w");
    if(!fp || !fk){ perror("fopen"); return 1; }
    fprintf(fp, "N,E,tempo_s\n");
    fprintf(fk, "N,E,tempo_s\n");

    for(int i = 0; i < SIZE(nv); i++){
        int N = nv[i];
        int M = N*(N - 1)/2;

        for(int j = 0; j < SIZE(density); j++){
            int E = (int)(density[j] * M);
            if(E < N - 1) E = N - 1;
            int maxW = N + E;

            for(int r = 0; r < REPS; r++){
                Graph* G = Gpk_RandomConnected(N, E, maxW, SEED + r);
                if(!G){
                    fprintf(stderr, "falha ao gerar grafo N=%d E=%d\n", N, E);
                    continue;
                }
                printf("Grafo: {nV = %d; nE = %d} rep %d\n", G->nV, G->nE, r);

                clock_t start = clock();
                Graph* T = Gpk_Primm(G);
                clock_t end = clock();
                fprintf(fp, "%d,%d,%.9f\n", N, G->nE, (double)(end - start)*1000.0 / CLOCKS_PER_SEC);
                fflush(fp);
                Gpk_free(&T);

                start = clock();
                T = Gpk_Kruskal(G);
                end = clock();
                fprintf(fk, "%d,%d,%.9f\n", N, G->nE, (double)(end - start)*1000.0 / CLOCKS_PER_SEC);
                fflush(fk);
                Gpk_free(&T);

                Gpk_free(&G);
            }
        }
    }

    fclose(fp);
    fclose(fk);
    return 0;
}
