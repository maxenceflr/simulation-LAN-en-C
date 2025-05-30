#include "graphe.h"
#include "reseau.h"
#include "initReseau.h"


int main(){

    graphe g;
    char* path = "reseau_test.txt";

    init_graph_from_file(&g, path);

    afficher_graphe(&g);

    deinit_graphe(&g);

    return 0;
}