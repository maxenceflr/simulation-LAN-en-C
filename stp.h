
#ifndef STP_H
#define STP_H

#include <stddef.h>
#include <stdbool.h>
#include"graphe.h"
#include "reseau.h"  // suppose que graphe, Switch, arete, etc. sont définis ici

#define INFINI SIZE_MAX
#define UNKNOWN_INDEX ((size_t)-1)

/**
 * @brief Trouve le switch avec la plus petite racine (meilleur Root Bridge).
 * @param g Pointeur vers le graphe.
 * @return Pointeur vers le meilleur switch (NULL si aucun switch trouvé).
 */
Switch* trouver_racine(graphe *g);

/**
 * @brief Trouve l'index du sommet non visité avec la distance minimale.
 * @param distances Tableau des distances.
 * @param visite Tableau des sommets visités.
 * @param nb_sommets Nombre total de sommets dans le graphe.
 * @return Index du sommet avec la plus petite distance non visité.
 */
size_t trouver_sommet_min_distance(size_t distances[], bool visite[], size_t nb_sommets);

/**
 * @brief Implémentation de l'algorithme de Dijkstra pour calculer les plus courts chemins.
 * @param g Pointeur vers le graphe.
 * @param source Index du sommet source.
 * @param distances Tableau de sortie des distances.
 * @param predecesseurs Tableau de sortie des prédécesseurs.
 */
void dijkstra(graphe *g, size_t source, size_t *distances, size_t *predecesseurs);


/**
 * @brief Affiche l'arbre STP (Spanning Tree Protocol) à partir de la racine.
 * @param g Pointeur vers le graphe.
 * @param racine Index du sommet racine.
 * @param predecesseurs Tableau des prédécesseurs.
 */
void afficher_arbre_stp(graphe *g, size_t racine, size_t predecesseurs[], size_t distances[]);




#endif // STP_H
