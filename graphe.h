#ifndef GRAPHE_H
#define GRAPHE_H

#pragma once

#include "reseau.h"
#define NOMBRE_ARETE_MAX 500
#define NOMBRE_SOMMETS_MAX 500

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

/**
 * @brief Structure représentant une arête du graphe.
 */
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

/**
 * @brief Initialise un graphe.
 * @param g Pointeur vers le graphe à initialiser.
 */
void init_graphe(graphe *g);

/**
 * @brief Libère la mémoire d'un graphe.
 * @param g Pointeur vers le graphe à désinitialiser.
 */
void deinit_graphe(graphe *g);

/**
 * @brief Retourne le nombre de sommets du graphe.
 * @param g Pointeur constant vers le graphe.
 * @return Nombre de sommets.
 */
size_t ordre(graphe const *g);

/**
 * @brief Retourne le nombre d'arêtes du graphe.
 * @param g Pointeur constant vers le graphe.
 * @return Nombre d'arêtes.
 */
size_t nb_aretes(graphe const *g);

/**
 * @brief Ajoute une station au graphe.
 * @param g Pointeur vers le graphe.
 * @param station Station à ajouter.
 */
void ajouter_sommet_station(graphe *g, Station station);

/**
 * @brief Ajoute un switch au graphe.
 * @param g Pointeur vers le graphe.
 * @param s_switch Switch à ajouter.
 */
void ajouter_sommet_switch(graphe *g, Switch s_switch);

/**
 * @brief Ajoute un sommet générique au graphe.
 * @param g Pointeur vers le graphe.
 * @param s_sommet Sommet à ajouter.
 */
void ajouter_sommet(graphe *g, sommet s_sommet);

/**
 * @brief Retourne l'index d'un sommet dans le graphe.
 * @param g Pointeur constant vers le graphe.
 * @param s Sommet à rechercher.
 * @return Index du sommet ou UNKNOWN_INDEX.
 */
size_t index_sommet(graphe const *g, sommet s);

/**
 * @brief Retourne l'index d'un équipement dans le graphe.
 * @param g Pointeur constant vers le graphe.
 * @param s Sommet à rechercher.
 * @return Index de l'équipement.
 */
size_t index_equipement(graphe const *g, sommet s);

/**
 * @brief Vérifie si une arête existe dans le graphe.
 * @param g Pointeur constant vers le graphe.
 * @param a Arête à rechercher.
 * @return true si l'arête existe, false sinon.
 */
bool existe_arete(graphe const *g, arete a);

/**
 * @brief Ajoute une arête au graphe.
 * @param g Pointeur vers le graphe.
 * @param a Arête à ajouter.
 * @return true si ajoutée, false sinon.
 */
bool ajouter_arete(graphe *g, arete a);

/**
 * @brief Compare deux sommets.
 * @param s1 Premier sommet.
 * @param s2 Deuxième sommet.
 * @return true si identiques, false sinon.
 */
bool equals_sommet(sommet s1, sommet s2);

/**
 * @brief Retourne l'index d'une arête dans le graphe.
 * @param g Pointeur constant vers le graphe.
 * @param a Arête à rechercher.
 * @return Index de l'arête ou UNKNOWN_INDEX.
 */
size_t index_arete(graphe const *g, arete a);

/**
 * @brief Remplit un tableau avec les sommets adjacents à un sommet donné.
 * @param g Pointeur constant vers le graphe.
 * @param s Sommet dont on cherche les adjacents.
 * @param sa Tableau de sommets à remplir.
 * @return Nombre de sommets adjacents trouvés.
 */
size_t sommets_adjacents(graphe const *g, sommet s, sommet sa[]);

/**
 * @brief Affiche l'ensemble du graphe.
 * @param g Pointeur constant vers le graphe à afficher.
 */
void afficher_graphe(graphe const *g);

#endif