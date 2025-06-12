#ifndef GRAPHE_H
#define GRAPHE_H

#pragma once

#include "reseau.h"

// Définition des capacités maximales pour le graphe
#define NOMBRE_ARETE_MAX 500
#define NOMBRE_SOMMETS_MAX 500

// Constante pour indiquer qu'un index n'a pas été trouvé
static const size_t UNKNOWN_INDEX = -1;


/**
 * @brief Type d'équipement pour un sommet.
 */
typedef enum
{
    TYPE_SWITCH,  /**< Switch réseau */
    TYPE_STATION, /**< Station réseau */
} TypeTag;


/**
 * @brief Structure représentant un sommet du graphe (station ou switch).
 */
typedef struct sommet
{
    TypeTag type_equipement; /**< Type d'équipement */
    union
    {
        Station station; /**< Station */
        Switch s_switch; /**< Switch */
    };
} sommet;


typedef struct arete
{
    sommet s1;    /**< Premier sommet */
    sommet s2;    /**< Deuxième sommet */
    size_t poids; /**< Poids de l'arête */
} arete;

/**
 * @brief Structure représentant un graphe réseau.
 */
typedef struct graphe
{
    size_t nb_switch;  /**< Nombre de switches */
    size_t nb_station; /**< Nombre de stations */

    size_t ordre;           /**< Nombre de sommets */
    size_t sommet_capacite; /**< Capacité du tableau de sommets */
    sommet *sommet;         /**< Tableau de sommets */

    arete *aretes;          /**< Tableau d'arêtes */
    size_t aretes_capacite; /**< Capacité du tableau d'arêtes */
    size_t nb_aretes;       /**< Nombre d'arêtes */

} graphe;

// Déclarations des fonctions de gestion du graphe

void init_graphe(graphe *g);
void deinit_graphe(graphe *g);
size_t ordre(graphe const *g);
size_t nb_aretes(graphe const *g);
void ajouter_sommet_station(graphe *g, Station station);
void ajouter_sommet_switch(graphe *g, Switch s_switch);
void ajouter_sommet(graphe *g, sommet s_sommet);
size_t index_sommet(graphe const *g, sommet s);
bool existe_arete(graphe const *g, arete a);
bool ajouter_arete(graphe *g, arete a);
bool equals_sommet(sommet s1, sommet s2);

bool equalsArete(arete a1, arete a2);

size_t index_arete(graphe const *g, arete a);

size_t sommets_adjacents(graphe const *g, sommet s, sommet sa[]);
void afficher_graphe(graphe const *g);
uint32_t nb_composantes_connexes(graphe const *g);
void visite_composante_connexe(graphe const *g, sommet s, bool *visite);
bool sont_connectes(graphe const *g, sommet s1, sommet s2);

#endif
