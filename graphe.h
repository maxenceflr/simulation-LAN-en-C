#ifndef GRAPHE_H
#define GRAPHE_H

#pragma once

#include "reseau.h"
#define NOMBRE_ARETE_MAX 500
#define NOMBRE_SOMMETS_MAX 500

static const size_t UNKNOWN_INDEX = -1;

typedef enum
{
    TYPE_SWITCH,
    TYPE_STATION,
} TypeTag;

typedef struct sommet
{
    TypeTag type_equipement;
    union
    {
        Station station;
        Switch s_switch;
    };
} sommet;

typedef struct arete
{
    sommet s1;
    sommet s2;
    size_t poids;
} arete;

typedef struct graphe
{
    size_t nb_switch;
    size_t nb_station;

    size_t ordre;
    size_t sommet_capacite;
    sommet *sommet;

    arete *aretes;
    size_t aretes_capacite;
    size_t nb_aretes;

} graphe;

void init_graphe(graphe *g);
void deinit_graphe(graphe *g);

size_t ordre(graphe const *g);
size_t nb_aretes(graphe const *g);

void ajouter_sommet_station(graphe *g, Station station);
void ajouter_sommet_switch(graphe *g, Switch s_switch);
void ajouter_sommet(graphe *g, sommet s_sommet);
size_t index_sommet(graphe const *g, sommet s);
size_t index_equipement(graphe const *g, sommet s);
bool existe_arete(graphe const *g, arete a);
bool ajouter_arete(graphe *g, arete a);
bool equals_sommet(sommet s1, sommet s2);
size_t index_arete(graphe const *g, arete a);

size_t sommets_adjacents(graphe const *g, sommet s, sommet sa[]);
void afficher_graphe(graphe const *g);

#endif