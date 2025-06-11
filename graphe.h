#ifndef GRAPHE_H
#define GRAPHE_H

#pragma once

#include "reseau.h"

// Définition des capacités maximales pour le graphe
#define NOMBRE_ARETE_MAX 500
#define NOMBRE_SOMMETS_MAX 500

// Constante pour indiquer qu'un index n'a pas été trouvé
static const size_t UNKNOWN_INDEX = -1;

// Enumération pour distinguer le type d'équipement d'un sommet
typedef enum
{
    TYPE_SWITCH,
    TYPE_STATION,
} TypeTag;

// Structure représentant un sommet du graphe (station ou switch)
typedef struct sommet
{
    TypeTag type_equipement; // Tag pour savoir quel type d'équipement est stocké
    union
    {
        Station station; // Si c'est une station
        Switch s_switch; // Si c'est un switch
    };
} sommet;

// Structure représentant une arête (lien) entre deux sommets
typedef struct arete
{
    sommet s1;    // Premier sommet
    sommet s2;    // Second sommet
    size_t poids; // Poids de l'arête (ex : coût, distance, etc.)
} arete;

// Structure principale du graphe
typedef struct graphe
{
    size_t nb_switch;       // Nombre de switchs dans le graphe
    size_t nb_station;      // Nombre de stations dans le graphe
    size_t ordre;           // Nombre total de sommets
    size_t sommet_capacite; // Capacité allouée pour les sommets
    sommet *sommet;         // Tableau dynamique de sommets
    size_t nb_aretes;       // Nombre d'arêtes
    size_t aretes_capacite; // Capacité allouée pour les arêtes
    arete *aretes;          // Tableau dynamique d'arêtes
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
size_t sommets_adjacents(graphe const *g, sommet s, sommet sa[]);
void afficher_graphe(graphe const *g);

#endif