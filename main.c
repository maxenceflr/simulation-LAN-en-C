#include "graphe.h"
#include "reseau.h"
#include "initReseau.h"
#include "ethernet.h"

int main()
{

    graphe g;
    char *path = "reseau_test.txt";
    Trame trame_test;
    init_graph_from_file(&g, path);
    init_trame(&trame_test, g.sommet[0].station.adrMAC, g.sommet[1].station.adrMAC, 0, g.sommet[0].station.adrIP, NULL, 0);

    printf("Nombre de composantes connexes: %u\n", nb_composantes_connexes(&g));
    // printf("Envoyer Trame: %\n", );
    afficher_graphe(&g);
    deinit_graphe(&g);

    return 0;
}