#include"stp.h"

#define INFINI SIZE_MAX  // Utilisation de SIZE_MAX comme représentation de l'infini

/**
 * Trouve le switch avec la plus petite racine (meilleur Root Bridge).
 * @param g Pointeur vers le graphe.
 * @return Pointeur vers le meilleur switch (NULL si aucun switch trouvé).
 */
Switch* trouver_racine(graphe *g) {
    if (g == NULL || g->ordre == 0) {
        return NULL; // Graphe vide
    }

    Switch *meilleur_switch = NULL;

    for (size_t i = 0; i < g->ordre; i++) {
        if (g->sommet[i].type_equipement == TYPE_SWITCH) {
            Switch *current_switch = &g->sommet[i].s_switch;

            // Si c'est le premier switch rencontré, il devient le meilleur par défaut
            if (meilleur_switch == NULL) {
                meilleur_switch = current_switch;
            } 
            // Sinon, on compare priorité + MAC
            else {
                // Comparaison des priorités
                if (current_switch->priorite < meilleur_switch->priorite) {
                    meilleur_switch = current_switch;
                } 
                // Si priorités égales, comparer les adresses MAC
                else if (current_switch->priorite == meilleur_switch->priorite) {
                    if (current_switch->adrMac.entier < meilleur_switch->adrMac.entier) {
                        meilleur_switch = current_switch;
                    }
                }
            }
        }
    }

    return meilleur_switch; // Retourne NULL si aucun switch n'est trouvé
}



/**
 * Trouve l'index du sommet non visité avec la distance minimale
 */
size_t trouver_sommet_min_distance(size_t distances[], bool visite[], size_t nb_sommets) {
    size_t min_index = UNKNOWN_INDEX;
    size_t min_distance = INFINI;

    for (size_t v = 0; v < nb_sommets; v++) {
        if (!visite[v] && distances[v] <= min_distance) {
            min_distance = distances[v];
            min_index = v;
        }
    }

    return min_index;
}




/**
 * Implémentation de Dijkstra pour trouver les plus courts chemins depuis une source
 * @param g Pointeur vers le graphe
 * @param source Index du sommet source
 * @param distances Tableau de sortie des distances
 * @param predecesseurs Tableau de sortie des prédécesseurs
 */
void dijkstra(graphe *g, size_t source, size_t distances[], size_t predecesseurs[]) {
    bool visite[g->ordre]; // Tableau de suivi des sommets visités

    // 1. Initialisation
    for (size_t i = 0; i < g->ordre; i++) {
        distances[i] = INFINI;
        predecesseurs[i] = UNKNOWN_INDEX;
        visite[i] = false;
    }
    distances[source] = 0; // Distance à la source = 0

    // 2. Boucle principale
    for (size_t count = 0; count < g->ordre - 1; count++) {
        // 2a. Choisir le sommet non visité avec la plus petite distance
        size_t u = trouver_sommet_min_distance(distances, visite, g->ordre);
        if (u == UNKNOWN_INDEX) break; // Tous les sommets accessibles ont été visités

        visite[u] = true; // 2b. Marquer comme visité

        // 2c. Parcourir tous les voisins
        for (size_t v = 0; v < g->ordre; v++) {
            // Vérifier si c'est un voisin (arête existe)
            arete test_arete = {g->sommet[u], g->sommet[v], 0};
            if (!existe_arete(g, test_arete)) continue;

            // Trouver le poids réel de l'arête
            size_t poids = 0;
            for (size_t i = 0; i < g->nb_aretes; i++) {
                if (equalsArete(g->aretes[i], test_arete)) {
                    poids = g->aretes[i].poids;
                    break;
                }
            }

            // Mettre à jour la distance si un chemin plus court est trouvé
            if (!visite[v] && distances[u] != INFINI &&
                distances[u] + poids < distances[v]) {
                distances[v] = distances[u] + poids;
                predecesseurs[v] = u;
            }
        }
    }
}