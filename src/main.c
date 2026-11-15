#include "../include/Gpk_rnd.h"

int main(){
    Graph* gp = Gpk_RandomConnected(10, 14, 20, 3);
    printf("Grafo:\n");
    print_graph(gp);    
    
    Graph* Min_Tree = Gpk_Primm(gp);
    printf("Primm:\n");
    print_graph(Min_Tree);
    Gpk_free(&Min_Tree);

    Min_Tree = Gpk_Kruskal(gp);
    printf("Kruskal:\n");
    print_graph(Min_Tree);

    Gpk_free(&Min_Tree);
    Gpk_free(&gp);
    return 0;
}
