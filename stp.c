#include "stp.h"
#include <limits.h>
#include <stdbool.h>

#define INF 1000000000  // Valeur infinie utilisée pour Dijkstra


static SwitchCompareResult compare_switch_stp(Switch s1, Switch s2) {
    if (s1.priorite < s2.priorite)
        return AVANT;
    else if (s1.priorite > s2.priorite)
        return APRES;
    else {
        if (s1.adrMac.entier < s2.adrMac.entier)
            return AVANT;
        else if (s1.adrMac.entier > s2.adrMac.entier)
            return APRES;
        else
            return EGUALE;
    }
}




void run_stp(graphe *g) {
    //  Constituer un tableau des indices des switches dans le graphe.
    int nb_switch = 0;
    for (size_t i = 0; i < g->ordre; i++) {

        if (g->sommet[i].type_equipement == TYPE_SWITCH)
            nb_switch++;
            
    }
    if (nb_switch == 0) {
        printf("Aucun switch dans le réseau.\n");
        return;
    }
    int *tabl_switch = malloc(nb_switch * sizeof(int));  // Mappe l'indice local des switches vers g->sommet
    int idx = 0;
    for (size_t i = 0; i < g->ordre; i++) {
        if (g->sommet[i].type_equipement == TYPE_SWITCH)
            tabl_switch[idx++] = i;
    }
    
    //Élection du switch racine.
    int root_sw_idx = 0;
    for (int i = 1; i < nb_switch; i++) {
        int curIndex = tabl_switch[i];
        int bestIndex = tabl_switch[root_sw_idx];
        if (compare_switch_stp(g->sommet[curIndex].s_switch, g->sommet[bestIndex].s_switch) == AVANT)
            root_sw_idx = i;
    }

    int root_g_index = tabl_switch[root_sw_idx];
    printf("Switch racine élu : Switch %zu (priorité %zu)\n",
           g->sommet[root_g_index].s_switch.id, g->sommet[root_g_index].s_switch.priorite);
           
    
    int **cost = malloc(nb_switch * sizeof(int *));//creer "matrice"(tableau de tableau)  de taille  nb_switch*nb_switch
    for (int i = 0; i < nb_switch; i++) {
        cost[i] = malloc(nb_switch * sizeof(int));
        for (int j = 0; j < nb_switch; j++) {
            cost[i][j] = (i == j) ? 0 : INF;//init a l infini sauf sur lui meme
        }
    }
    // Pour chaque arete reliant deux switches, mettre à jour la matrice.
    for (size_t a = 0; a < g->nb_aretes; a++) {
        arete current = g->aretes[a];
        if (current.s1.type_equipement == TYPE_SWITCH && current.s2.type_equipement == TYPE_SWITCH) {
            int idx1 = -1, idx2 = -1;
            for (int i = 0; i < nb_switch; i++) {//recupere les 2 switch de l'arrete courant
                int realIndex = tabl_switch[i];
                if (equals_switch(g->sommet[realIndex].s_switch, current.s1.s_switch))
                    idx1 = i;
                if (equals_switch(g->sommet[realIndex].s_switch, current.s2.s_switch))
                    idx2 = i;
            }
            if (idx1 != -1 && idx2 != -1) {
                // Cast pour comparer deux int afin d'éviter les warnings de type signé/unsigned.
                if ((int)current.poids < cost[idx1][idx2]) {
                    cost[idx1][idx2] = current.poids;
                    cost[idx2][idx1] = current.poids;
                }
            }
        }
    }
    
    // Exécution de l'algorithme de Dijkstra à partir du switch racine.
    int *dist = malloc(nb_switch * sizeof(int));//distance a la racine
    int *prev = malloc(nb_switch * sizeof(int));//antecedent
    bool *visited = malloc(nb_switch * sizeof(bool));
    for (int i = 0; i < nb_switch; i++) {
        dist[i] = INF;
        prev[i] = -1;
        visited[i] = false;
    }
    dist[root_sw_idx] = 0;
    
    for (int i = 0; i < nb_switch; i++) {
        int u = -1;
        int minDist = INF;
        for (int j = 0; j < nb_switch; j++) {
            if (!visited[j] && dist[j] < minDist) {
                minDist = dist[j];
                u = j;
            }
        }
        if (u == -1)
            break;
        visited[u] = true;
        for (int v = 0; v < nb_switch; v++) {
            if (!visited[v] && cost[u][v] < INF) {
                if (dist[u] + cost[u][v] < dist[v]) {
                    dist[v] = dist[u] + cost[u][v];
                    prev[v] = u;
                }
            }
        }
    }
    
    // Affichage des résultats de l'arbre couvrant (STP).
    printf("\nArbre couvrant (d'après Dijkstra) :\n");
    for (int i = 0; i < nb_switch; i++) {
        int realIndex = tabl_switch[i];
        if (i == root_sw_idx) {
            printf("Switch %zu -> Racin: coût = 0\n", g->sommet[realIndex].s_switch.id);
        } else {
            if (prev[i] != -1) {
                int parent_realIndex = tabl_switch[prev[i]];
                printf("Switch %zu -> Parent: Switch %zu, coût = %d\n",
                       g->sommet[realIndex].s_switch.id,
                       g->sommet[parent_realIndex].s_switch.id, dist[i]);
            } else {
                printf("Switch %zu : pas connecté à la racine.\n", g->sommet[realIndex].s_switch.id);
            }
        }
    }
    
    // Mise à jour des états des ports selon l'arbre couvrant.
    mise_a_jour_ports_stp(g, prev, nb_switch, tabl_switch);



    
    // Libération de la mémoire allouée.
    for (int i = 0; i < nb_switch; i++) {
        free(cost[i]);
    }
    free(cost);
    free(dist);
    free(prev);
    free(visited);
    free(tabl_switch);
}
// Fonction e qui retourne l'indice du premier port libre 
int assigner_port(Switch *s) {
    for (size_t p = 0; p < s->nb_ports; p++) {
        if (s->tab_commutation[p].etat == STP_INCONNU) {
            return p;
        }
    }
    return -1;
}

void mise_a_jour_ports_stp(graphe *g, int *prev, int nb_switch, int *tabl_switch) {
    printf("\nMise à jour des états des ports STP :\n");

    // Réinitialise tous les ports de chaque switch à STP_INCONNU.
    for (int i = 0; i < nb_switch; i++) {
        int real_idx = tabl_switch[i];
        for (size_t p = 0; p < g->sommet[real_idx].s_switch.nb_ports; p++) {
            g->sommet[real_idx].s_switch.tab_commutation[p].etat = STP_INCONNU;
        }
    }

    // Parcourt arete reliant deux switches.
    for (size_t a = 0; a < g->nb_aretes; a++) {
        arete current = g->aretes[a];
        if (current.s1.type_equipement == TYPE_SWITCH && current.s2.type_equipement == TYPE_SWITCH) {
            int idx1 = -1, idx2 = -1;
            // Recherche dans tabl_switch les indices des switches de l'arête.
            for (int i = 0; i < nb_switch; i++) {
                if (equals_switch(g->sommet[tabl_switch[i]].s_switch, current.s1.s_switch))
                    idx1 = i;
                if (equals_switch(g->sommet[tabl_switch[i]].s_switch, current.s2.s_switch))
                    idx2 = i;
            }
            if (idx1 != -1 && idx2 != -1) {
                // Si l'arête fait partie de l'arbre couvrant...
                if (prev[idx1] == idx2) {
                    // idx1 est l'enfant, idx2 est le parent.
                    int port_child = assigner_port(&g->sommet[tabl_switch[idx1]].s_switch);
                    int port_parent = assigner_port(&g->sommet[tabl_switch[idx2]].s_switch);
                    if (port_child >= 0 && port_parent >= 0) {
                        g->sommet[tabl_switch[idx1]].s_switch.tab_commutation[port_child].etat = STP_RACINE;
                        g->sommet[tabl_switch[idx2]].s_switch.tab_commutation[port_parent].etat = STP_DESIGNE;
                        printf("Switch %zu (port %d) a pour antecedant Switch %zu (port %d)\n",
                               g->sommet[tabl_switch[idx1]].s_switch.id, port_child,
                               g->sommet[tabl_switch[idx2]].s_switch.id, port_parent);
                    }
                } else if (prev[idx2] == idx1) {
                    // idx2 est l'enfant, idx1 est le parent.
                    int port_child = assigner_port(&g->sommet[tabl_switch[idx2]].s_switch);
                    int port_parent = assigner_port(&g->sommet[tabl_switch[idx1]].s_switch);
                    if (port_child >= 0 && port_parent >= 0) {
                        g->sommet[tabl_switch[idx2]].s_switch.tab_commutation[port_child].etat = STP_RACINE;
                        g->sommet[tabl_switch[idx1]].s_switch.tab_commutation[port_parent].etat = STP_DESIGNE;
                        printf("Switch %zu (port %d) a pour antecedant Switch %zu (port %d)\n",
                               g->sommet[tabl_switch[idx2]].s_switch.id, port_child,
                               g->sommet[tabl_switch[idx1]].s_switch.id, port_parent);
                    }
                } else {
                    // L'arête ne fait pas partie de l'arbre couvrant :
                    // Affecte l'état BLOQUÉ aux ports correspondants.
                    int port1 = assigner_port(&g->sommet[tabl_switch[idx1]].s_switch);
                    int port2 = assigner_port(&g->sommet[tabl_switch[idx2]].s_switch);
                    if (port1 >= 0 && port2 >= 0) {
                        g->sommet[tabl_switch[idx1]].s_switch.tab_commutation[port1].etat = STP_BLOQUE;
                        g->sommet[tabl_switch[idx2]].s_switch.tab_commutation[port2].etat = STP_BLOQUE;
                        printf("Switch %zu (port %d) et Switch %zu (port %d) -> Port BLOQUÉ\n",
                               g->sommet[tabl_switch[idx1]].s_switch.id, port1,
                               g->sommet[tabl_switch[idx2]].s_switch.id, port2);
                    }
                }
            }
        }
    }
}

void afficher_stp_resultats(graphe *g) {
    printf("\nEtat final des ports STP par switch :\n");
    for (size_t i = 0; i < g->ordre; i++) {
        if (g->sommet[i].type_equipement == TYPE_SWITCH) {
            printf("Switch %zu : ", g->sommet[i].s_switch.id);
            for (size_t p = 0; p < g->sommet[i].s_switch.nb_ports; p++) {
                printf("Port %zu -> ", p);
                switch (g->sommet[i].s_switch.tab_commutation[p].etat) {
                    case STP_INCONNU:  printf("INCONNU, "); break;
                    case STP_RACINE:   printf("RACINE, "); break;
                    case STP_DESIGNE:  printf("DESIGNE, "); break;
                    case STP_BLOQUE:   printf("BLOQUE, "); break;
                    default:           printf("???, ");
                }
            }
            printf("\n");
        }
    }
}
