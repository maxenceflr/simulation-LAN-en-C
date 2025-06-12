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

    // Charger le graphe depuis un fichier
    init_graph_from_file(&g, path);

    printf("Nombre de composantes connexes: %u\n", nb_composantes_connexes(&g));
    afficher_graphe(&g);





    

    // Trouver la racine (meilleur switch)
    Switch *racine_switch = trouver_racine(&g);
    if (racine_switch == NULL) {
        printf("Aucun switch trouvé dans le graphe.\n");
        deinit_graphe(&g);
        return 1;
    }

    // Trouver l'index de la racine dans le tableau des sommets
    size_t racine_index = SIZE_MAX;
    for (size_t i = 0; i < g.ordre; i++) {
        if (g.sommet[i].type_equipement == TYPE_SWITCH) {
            if (&g.sommet[i].s_switch == racine_switch) {
                racine_index = i;
                break;
            }
        }
    }
    if (racine_index == SIZE_MAX) {
        printf("Erreur : index racine introuvable.\n");
        deinit_graphe(&g);
        return 1;
    }

    // Allocation des tableaux pour Dijkstra
    size_t *predecesseurs = malloc(g.ordre * sizeof(size_t));
    size_t *distances = malloc(g.ordre * sizeof(size_t));
    if (!predecesseurs || !distances) {
        fprintf(stderr, "Erreur d'allocation mémoire.\n");
        deinit_graphe(&g);
        free(predecesseurs);
        free(distances);
        return 1;
    }

    // Calcul des plus courts chemins depuis la racine
    dijkstra(&g, racine_index, distances, predecesseurs);

    // Afficher l'arbre STP
    afficher_arbre_stp(&g, racine_index, predecesseurs, distances);


    // Libération mémoire
    free(predecesseurs);
    free(distances);
    deinit_graphe(&g);

    return 0;
}/*
    for (size_t i = 0; i < g.nb_aretes; i++) {
    arete a = g.aretes[i];

    // Récupérer les types
    const char *type_src = (a.s1.type_equipement == TYPE_SWITCH) ? "Switch" : "Station";
    const char *type_dst = (a.s2.type_equipement == TYPE_SWITCH) ? "Switch" : "Station";

    // Récupérer les index dans le graphe, si tu veux les afficher
    size_t idx1 = index_sommet(&g, a.s1);
    size_t idx2 = index_sommet(&g, a.s2);

    printf("[%zu] %s %zu <--> %s %zu\n", i, type_src, idx1, type_dst, idx2);
}*/