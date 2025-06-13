#include "graphe.h"
#include "reseau.h"
#include "initReseau.h"
#include "ethernet.h"
#include "stp.h"

int main()
{
    graphe g;
    char *path = "reseau_test.txt";
    
    init_graphe(&g);
    // Charger le graphe depuis le fichier de configuration.
    init_graph_from_file(&g, path);
    
    printf("Nombre de composantes connexes: %u\n", nb_composantes_connexes(&g));
    afficher_graphe(&g);
    
    // Phase 4 : Exécution du protocole STP (Spanning Tree Protocol)
    run_stp(&g);
    afficher_stp_resultats(&g);
    
    // Simulation d'envoi de trame Ethernet (phase 3)
    Trame trame_test;
    init_trame(&trame_test, g.sommet[0].s_switch.adrMac,
               g.sommet[1].s_switch.adrMac, 0,
               g.sommet[9].station.adrIP, NULL, 0);
    afficher_tram_user(&trame_test);
    envoyer_tram(&trame_test, &g);
    deinit_trame(&trame_test);
    deinit_graphe(&g);
    
    return 0;
}
