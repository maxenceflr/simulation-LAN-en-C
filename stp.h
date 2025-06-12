
#ifndef STP_H
#define STP_H

#include <stddef.h>
#include <stdbool.h>
#include"graphe.h"
#include "reseau.h"  // suppose que graphe, Switch, arete, etc. sont définis ici

#define INFINI SIZE_MAX
#define UNKNOWN_INDEX ((size_t)-1)

Switch* trouver_racine(graphe *g);

size_t trouver_sommet_min_distance(size_t distances[], bool visite[], size_t nb_sommets);

void dijkstra(graphe *g, size_t source, size_t *distances, size_t *predecesseurs);

void afficher_arbre_stp(graphe *g, size_t racine, size_t predecesseurs[], size_t distances[]);




#endif 
