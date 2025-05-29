#include "graphe.h"
#include "string.h"
#include <stdio.h>

void init_graphe(graphe *g, size_t nb_sommet, size_t nbAretes)
{
    g->ordre = nb_sommet;
    g->sommet_capacite = NOMBRE_SOMMETS_MAX;
    g->sommet = malloc(g->sommet_capacite * sizeof(sommet));

    g->nb_aretes = nbAretes;
    g->aretes_capacite = NOMBRE_ARETE_MAX;
    g->aretes = malloc(g->aretes_capacite * sizeof(arete));
}

void deinit_graphe(graphe *g)
{
    free(g->aretes);
    free(g->sommet);

    g->aretes = NULL;
    g->ordre = 0;
    g->sommet_capacite = 0;
    g->nb_aretes = 0;
    g->aretes_capacite = 0;
}

void init_sommet_station(sommet* s, adresseIP adrIP, adresseMAC adrMAC)
{
    s->type_equipement = TYPE_STATION;
    init_station(&(s->station), adrIP, adrMAC);
}

void init_sommet_switch(sommet* s, adresseMAC adrMAC, size_t nbPort, size_t numPriorite, adresseMAC adrMac)
{
    s->type_equipement = TYPE_SWITCH;
    init_switch(&(s->s_switch), nbPort, numPriorite, adrMAC);
}

void ajouter_sommet_switch(graphe *g, Switch sswitch)
{
    sommet s;
    s.type_equipement = TYPE_SWITCH;
    s.s_switch = sswitch;
    g->sommet[g->ordre] = s;
    g->ordre += 1;
}

void ajouter_sommet_station(graphe *g, Station station)
{
    sommet s;
    s.type_equipement = TYPE_STATION;
    s.station = station;
    g->sommet[g->ordre] = s;
    g->ordre += 1;   
}

size_t ordre(graphe const *g)
{
    return g->ordre;
}

size_t nb_aretes(graphe const *g)
{
    return g->nb_aretes;
}

void ajouter_sommet(graphe *g, sommet s_sommet)
{
    g->sommet[g->ordre] = s_sommet;
    g->ordre += 1;
}

size_t index_sommet(graphe const *g, sommet s)
{
    for (size_t index = 0; index < g->ordre; index++)
    {
        if(equals_sommet(g->sommet[index], s))
        {
            return index;
        }
    }

    return UNKNOWN_INDEX;
}

bool equals_sommet(sommet s1, sommet s2)
{
    if (s1.type_equipement == s2.type_equipement)
    {
        if (s1.type_equipement == TYPE_STATION)
        {
            return equals_station(s1.station, s2.station);
        }
        else
        {
            return equals_switch(s1.s_switch, s2.s_switch);
        }
    }
    else 
    {
        return false;
    }
}



// Une fonction locale "static arete swap_sommets(arete a)" pourra être utile
// cette fonction retourne une nouvelle arête dont les sommets sont les même que l'arête reçue mais inversés

arete swap_sommets(arete a)
{
    return (arete){a.s2,a.s1};
}

// Une fonction locale pour comparer si 2 arêtes sont égales.
// Elle retourne true si a1 = a2 et false sinon

bool equalsArete(arete a1, arete a2)
{
    return (equals_sommet(a1.s1, a2.s1) && equals_sommet(a1.s2, a2.s2)) || (equals_sommet(a1.s1, a2.s2) && equals_sommet(a1.s2, a2.s1)) && a1.poid == a2.poid;
}

// Une fonction locale "static arete swap_sommets(arete a)" pourra être utile
// cette fonction retourne une nouvelle arête dont les sommets sont les même que l'arête reçue mais inversés

bool existe_arete(graphe const *g, arete a)
{
    // retourne true si l'arête a est contenue dans le graphe g, false sinon
    // /!\ l'arête (s1,s2) et l'arête (s2,s1) sont considérées équivalentes

    size_t taille = sizeof(g->nb_aretes);

    for (size_t i = 0; i < taille; ++i) {
        if(equalsArete(g->aretes[i], a)){
            return true;
        }
    }

    return false;
}

bool ajouter_arete(graphe *g, arete a)
{
    // l'arête a n'est ajoutée que si les conditions suivantes sont remplies :
    //  - les sommets s1 et s2 de a existent dans g
    //  - les sommets s1 et s2 de a sont distincts
    //  - l'arête a n'existe pas dans g

    if( !equals_sommet(a.s1, a.s2) && (index_sommet(g, a.s1) != UNKNOWN_INDEX && index_sommet(g, a.s2) != UNKNOWN_INDEX) && !existe_arete(g, a) )
    {
        if (g->nb_aretes == g->aretes_capacite)
        {
            size_t ancienne_capacite = g->aretes_capacite;
            g->aretes_capacite *= 2;
            
            arete* new_tab = malloc(g->aretes_capacite * sizeof(arete));
            memcpy(new_tab, g->aretes, ancienne_capacite * sizeof(arete));

            free(g->aretes);

            g->aretes = new_tab;
        }
    
        g->aretes[g->nb_aretes] = a;
        g->nb_aretes += 1;

        return true;
    }

    return false;
}

size_t index_arete(graphe const *g, arete a)
{
    // retourne l'index de l'arête au sein du tableau d'arêtes de g si l'arête a existe dans g,
    // la valeur UNKNOWN_INDEX sinon

    if (existe_arete(g, a))
    {
        for (size_t index = 0; index < g->nb_aretes; index++)
        {
            if (equalsArete(a, g->aretes[index]))
            {
                return index;
            }
        }
    }

    return UNKNOWN_INDEX;
}

size_t sommets_adjacents(graphe const *g, sommet s, sommet sa[])
{
    // remplit le tableau sa avec les sommets adjacents de s dans g
    // et retourne le nombre de sommets ainsi stockés
    // (on suppose que s fait bien partie du graphe g)
    // (le tableau sa est supposé assez grand pour contenir les sommets adjacents de s)

    int nb_sommets_stockes = 0;

    for(size_t i = 0; i < g->nb_aretes; i++)
    {
        if (equals_sommet(g->aretes[i].s1, s))
        {
            sa[nb_sommets_stockes] = g->aretes[i].s2;
            nb_sommets_stockes ++;
        }
        else if (equals_sommet(g->aretes[i].s2, s))
        {
            sa[nb_sommets_stockes] = g->aretes[i].s1;
            nb_sommets_stockes ++;
        }
    }

    return nb_sommets_stockes;
}

void afficher_tab(size_t tab[], size_t taille)
{
    for (size_t i = 0; i < taille; i++)
    {
        printf("%lu ", tab[i]);
    }
}

void afficher_arete(arete a)
{
    printf("%lu - %lu\n", a.s1, a.s2);
}

void afficher(graphe const *g)
{
    printf("# sommets = %lu\n", g->ordre);
    printf("# arêtes  = %lu\n", g->nb_aretes);
    printf("--SOMMETS--\n");

    for (size_t index = 0; index < g->ordre; index++)
    {   
        size_t sa[NOMBRE_SOMMETS_MAX];
        size_t nb_sommet_ajd = sommets_adjacents(g, g->sommet[index], sa);

        printf("%lu (degré: %lu) <-> ", index, degre(g, g->sommet[index]));
        afficher_tab(sa, nb_sommet_ajd);
        printf("\n");
    }

    printf("--ARÊTES--\n");

    for (size_t i = 0; i < g->nb_aretes; i++)
    {   
        afficher_arete(g->aretes[i]);
    }
}