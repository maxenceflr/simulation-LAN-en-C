#pragma once

#include <reseau.h>
#include <stdlib.h>
#include <stdbool.h>

#define NOMBRE_ARETE_MAX 500
#define NOMBRE_SOMMETS_MAX 500

static const size_t UNKNOWN_INDEX = -1;

typedef enum {
    TYPE_SWITCH,
    TYPE_STATION,
} TypeTag;

typedef union sommet
{   
    TypeTag type_equipement;
    Station station;
    Switch  s_switch;
} sommet;

typedef struct arete
{
    sommet s1;
    sommet s2;   
} arete;

typedef struct graphe
{
    size_t ordre;
    size_t sommet_capacite;
    sommet* sommet;
    arete* aretes; 
    size_t aretes_capacite;
    size_t nb_aretes;

    
} graphe;


void init_graphe(graphe *g, size_t nb_sommet);
void deinit_graphe(graphe *g);

size_t ordre(graphe const *g);
size_t nb_aretes(graphe const *g);

void ajouter_sommet(graphe *g, sommet s_sommet);
size_t index_sommet(graphe const *g, sommet s);
bool existe_arete(graphe const *g, arete a);
bool ajouter_arete(graphe *g, arete a);
size_t index_arete(graphe const *g, arete a);

size_t sommets_adjacents(graphe const *g, sommet s, sommet sa[]);
