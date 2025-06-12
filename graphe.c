#include "graphe.h"

/**
 * Initialise la structure du graphe :
 * - Alloue la mémoire pour les sommets et les arêtes
 * - Initialise les compteurs à zéro
 * @param g Pointeur vers la structure graphe à initialiser.
 */
void init_graphe(graphe *g)
{
    g->nb_switch = 0;
    g->nb_station = 0;
    g->ordre = 0;
    g->sommet_capacite = NOMBRE_SOMMETS_MAX;
    g->sommet = (sommet *)malloc(g->sommet_capacite * sizeof(sommet));
    g->nb_aretes = 0;
    g->aretes_capacite = NOMBRE_ARETE_MAX;
    g->aretes = (arete *)malloc(g->aretes_capacite * sizeof(arete));
}

/**
 * Libère la mémoire allouée pour le graphe.
 * Appelle aussi les fonctions de désallocation pour chaque switch.
 * @param g Pointeur vers la structure graphe à désallouer.
 */
void deinit_graphe(graphe *g)
{
    for (size_t index = 0; index < g->ordre; index++)
    {
        if (g->sommet[index].type_equipement == TYPE_SWITCH)
        {
            deinit_switch(&g->sommet[index].s_switch);
        }
    }

    free(g->aretes);
    free(g->sommet);
    g->aretes = NULL;
    g->sommet = NULL;
    g->ordre = 0;
    g->sommet_capacite = 0;
    g->nb_aretes = 0;
    g->aretes_capacite = 0;
    g->nb_switch = 0;
    g->nb_station = 0;
}

/**
 * Ajoute un sommet de type switch au graphe.
 * Incrémente le nombre de switchs et l'ordre du graphe.
 * @param g Pointeur vers le graphe.
 * @param sswitch Structure Switch à ajouter.
 */
void ajouter_sommet_switch(graphe *g, Switch sswitch)
{
    if (g->ordre >= g->sommet_capacite)
    {
        fprintf(stderr, "Erreur: capacité max de sommets atteinte\n");
        exit(EXIT_FAILURE);
    }
    sswitch.id = g->nb_switch + 1;
    g->sommet[g->ordre].type_equipement = TYPE_SWITCH;
    g->sommet[g->ordre].s_switch = sswitch;
    g->nb_switch += 1;
    g->ordre += 1;
}

/**
 * Ajoute un sommet de type station au graphe.
 * Incrémente le nombre de stations et l'ordre du graphe.
 * @param g Pointeur vers le graphe.
 * @param station Structure Station à ajouter.
 */
void ajouter_sommet_station(graphe *g, Station station)
{
    if (g->ordre >= g->sommet_capacite)
    {
        fprintf(stderr, "Erreur: capacité max de sommets atteinte\n");
        exit(EXIT_FAILURE);
    }
    station.id = g->nb_station + 1;
    g->sommet[g->ordre].type_equipement = TYPE_STATION;
    g->sommet[g->ordre].station = station;
    g->nb_station += 1;
    g->ordre += 1;
}

/**
 * Retourne l'ordre du graphe (nombre de sommets).
 * @param g Pointeur vers le graphe.
 * @return Nombre de sommets dans le graphe.
 */
size_t ordre(graphe const *g)
{
    return g->ordre;
}

/**
 * Retourne le nombre d'arêtes du graphe.
 * @param g Pointeur vers le graphe.
 * @return Nombre d'arêtes dans le graphe.
 */
size_t nb_aretes(graphe const *g)
{
    return g->nb_aretes;
}

/**
 * Ajoute un sommet générique (station ou switch) au graphe.
 * @param g Pointeur vers le graphe.
 * @param s_sommet Structure sommet à ajouter.
 */
void ajouter_sommet(graphe *g, sommet s_sommet)
{
    if (g->ordre >= g->sommet_capacite)
    {
        fprintf(stderr, "Erreur: capacité max de sommets atteinte\n");
        exit(EXIT_FAILURE);
    }
    g->sommet[g->ordre] = s_sommet;
    if (s_sommet.type_equipement == TYPE_STATION)
        g->nb_station += 1;
    else
        g->nb_switch += 1;
    g->ordre += 1;
}

/**

 * @brief Retourne l'index d'un sommet dans le graphe, ou UNKNOWN_INDEX s'il n'existe pas.
 * @param g Pointeur constant vers le graphe.
 * @param s Sommet à rechercher.
 * @return Index du sommet ou UNKNOWN_INDEX.
 */
size_t index_sommet(graphe const *g, sommet s)
{
    for (size_t index = 0; index < g->ordre; index++)
    {
        if (equals_sommet(g->sommet[index], s))
        {
            return index;
        }
    }
    return UNKNOWN_INDEX;
}

/**

 * @brief Compare deux sommets pour vérifier s'ils sont identiques.
 * @param s1 Premier sommet.
 * @param s2 Deuxième sommet.
 * @return true si les sommets sont identiques, false sinon.
 */
bool equals_sommet(sommet s1, sommet s2)
{
    if (s1.type_equipement == s2.type_equipement)
    {
        if (s1.type_equipement == TYPE_STATION)
            return equals_station(s1.station, s2.station);
        else
            return equals_switch(s1.s_switch, s2.s_switch);
    }
    else
    {
        return false;
    }
}

/**

 * @brief Inverse les sommets d'une arête.
 * @param a Arête à inverser.
 * @return Nouvelle arête avec les sommets inversés.
 */
arete swap_sommets(arete a)
{
    return (arete){a.s2, a.s1, a.poids};
}

/**

 * @brief Compare deux arêtes pour vérifier si elles sont identiques.
 * @param a1 Première arête.
 * @param a2 Deuxième arête.
 * @return true si les arêtes sont identiques, false sinon.
 */
bool equalsArete(arete a1, arete a2)
{
    return ((equals_sommet(a1.s1, a2.s1) && equals_sommet(a1.s2, a2.s2)) ||
            (equals_sommet(a1.s1, a2.s2) && equals_sommet(a1.s2, a2.s1))) &&
           (a1.poids == a2.poids);
}

/**

 * @brief Vérifie si une arête existe déjà dans le graphe.
 * @param g Pointeur constant vers le graphe.
 * @param a Arête à rechercher.
 * @return true si l'arête existe, false sinon.
 */
bool existe_arete(graphe const *g, arete a)
{
    for (size_t i = 0; i < g->nb_aretes; ++i)
    {
        if (equalsArete(g->aretes[i], a))
        {
            return true;
        }
    }
    return false;
}

/**

 * @brief Ajoute une arête au graphe si elle est valide et n'existe pas déjà.
 * @param g Pointeur vers le graphe.
 * @param a Arête à ajouter.
 * @return true si l'arête a été ajoutée, false sinon.
 */
bool ajouter_arete(graphe *g, arete a)
{
    if (g->nb_aretes >= g->aretes_capacite)
    {
        fprintf(stderr, "Capacité maximale d'arêtes atteinte\n");
        return false;
    }
/*FAIT BUGGER LE CODE
    if (existe_arete(g, a)) // Optionnel : éviter doublons
    {
        fprintf(stderr, "Arête déjà existante\n");
        return false;
    }
*/
    g->aretes[g->nb_aretes] = a;
    g->nb_aretes++;
    return true;
}


/**

 * @brief Retourne l'index d'une arête dans le graphe, ou UNKNOWN_INDEX si elle n'existe pas.
 * @param g Pointeur constant vers le graphe.
 * @param a Arête à rechercher.
 * @return Index de l'arête ou UNKNOWN_INDEX.
 */
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

/**
 * @brief Remplit un tableau avec les sommets adjacents à un sommet donné.
 * @param g Pointeur constant vers le graphe.
 * @param s Sommet dont on cherche les adjacents.
 * @param sa Tableau de sommets à remplir.
 * @return Nombre de sommets adjacents trouvés.
 */
size_t sommets_adjacents(graphe const *g, sommet s, sommet sa[])
{
    int nb_sommets_stockes = 0;

    for (size_t i = 0; i < g->nb_aretes; i++)
    {
        if (equals_sommet(g->aretes[i].s1, s))
        {
            sa[nb_sommets_stockes] = g->aretes[i].s2;
            nb_sommets_stockes++;
        }
        else if (equals_sommet(g->aretes[i].s2, s))
        {
            sa[nb_sommets_stockes] = g->aretes[i].s1;
            nb_sommets_stockes++;
        }
    }

    return nb_sommets_stockes;
}

/**
 * @brief Visite récursivement les sommets d'une composante connexe.
 * @param g Pointeur constant vers le graphe.
 * @param s Sommet de départ pour la visite.
 * @param visite Tableau de booléens indiquant si un sommet a été visité.
 */
void visite_composante_connexe(graphe const *g, sommet s, bool *visite)
{
    visite[index_sommet(g, s)] = true;
    sommet sa[NOMBRE_SOMMETS_MAX];
    int nb_sommetadj = sommets_adjacents(g, s, sa);

    for (int i = 0; i < nb_sommetadj; i++)
    {
        if (!visite[index_sommet(g, sa[i])])
        {
            visite_composante_connexe(g, sa[i], visite);
        }
    }
}

/**
 * @brief Compte le nombre de composantes connexes dans le graphe.
 * @param g Pointeur constant vers le graphe.
 * @return Nombre de composantes connexes.
 */
uint32_t nb_composantes_connexes(graphe const *g)
{
    bool *visite = malloc(g->ordre * sizeof(bool));

    for (size_t i = 0; i < g->ordre; i++)
    {
        visite[i] = false;
    }

    uint32_t nb_composante_conn = 0;

    for (size_t index_sommet = 0; index_sommet < g->ordre; index_sommet++)
    {
        if (!visite[index_sommet])
        {
            visite_composante_connexe(g, g->sommet[index_sommet], visite);
            nb_composante_conn++;
        }
    }

    free(visite);

    return nb_composante_conn;
}

/**
 * @brief Vérifie si deux sommets sont connectés dans le graphe.
 * @param g Pointeur constant vers le graphe.
 * @param s1 Premier sommet.
 * @param s2 Deuxième sommet.
 * @return true si les sommets sont connectés, false sinon.
 */
bool sont_connectes(graphe const *g, sommet s1, sommet s2)
{
    bool *sommets_visite = calloc(g->ordre, sizeof(bool));

    if (sommets_visite == NULL)
    {
        exit(EXIT_FAILURE);
    }

    visite_composante_connexe(g, s1, sommets_visite);

    bool est_connecte = sommets_visite[index_sommet(g, s2)];

    free(sommets_visite);

    return est_connecte;
}

/**
 * @brief Affiche un tableau d'entiers.
 * @param tab Tableau à afficher.
 * @param taille Taille du tableau.
 */
void afficher_tab(size_t tab[], size_t taille)
{
    for (size_t i = 0; i < taille; i++)
    {
        printf("%lu ", tab[i]);
    }
}

/**
 * @brief Affiche un sommet (station ou switch).
 * @param s Sommet à afficher.
 */
void afficher_sommet(sommet s)
{
    if (s.type_equipement == TYPE_STATION)
    {
        printf("STA%lu", s.station.id);
    }
    else
    {
        printf("SW%lu", s.s_switch.id);
    }
}

/**

 * @brief Affiche une arête du graphe.
 * @param a Arête à afficher.
 */
void afficher_arete(arete a)
{
    printf("(");
    afficher_sommet(a.s1);
    printf(" <--> ");
    afficher_sommet(a.s2);
    printf(") Poids: %lu\n", a.poids);
}

/**

 * @brief Affiche l'ensemble du graphe (équipements et liens).
 * @param g Pointeur constant vers le graphe à afficher.
 */
void afficher_graphe(graphe const *g)
{
    printf("# Nombre d'équipements : %lu\n", g->ordre);
    printf("# Nombre de liens : %lu\n\n", g->nb_aretes);
    printf("--EQUIPEMENTS--\n\n");
    for (size_t index = 0; index < g->ordre; index++)
    {
        if (g->sommet[index].type_equipement == TYPE_STATION)
            afficherStation(g->sommet[index].station);
        else
            afficherSwitch(g->sommet[index].s_switch);
    }
    printf("\n--LIENS--\n\n");
    for (size_t i = 0; i < g->nb_aretes; i++)
    {
        afficher_arete(g->aretes[i]);
    }
    printf("\n");
}