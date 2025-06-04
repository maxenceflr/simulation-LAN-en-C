#include "initReseau.h"

/**
 * Récupère le nombre d'équipements et de liens à partir de la première ligne du fichier.
 * @param ligne Ligne lue du fichier contenant deux entiers séparés par un espace.
 * @param nb_equipement Pointeur vers la variable où stocker le nombre d'équipements.
 * @param nb_lien Pointeur vers la variable où stocker le nombre de liens.
 */
void recuperer_nbEquipement_nbLien(const char *ligne, size_t *nb_equipement, size_t *nb_lien)
{
    if (sscanf(ligne, "%lu %lu", nb_equipement, nb_lien) == 2)
    {
        // Lecture réussie
    }
    else
    {
        fprintf(stderr, "Erreur de lecture du nombre d'équipements et de liens\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * Initialise une adresse IP à partir d'une chaîne de caractères.
 * @param adrIP Pointeur vers la structure adresseIP à remplir.
 * @param ip_str Chaîne de caractères représentant l'adresse IP (ex: "192.168.1.1").
 */
void init_adrIP_from_text(adresseIP *adrIP, const char *ip_str)
{
    uint32_t a, b, c, d;
    if (sscanf(ip_str, "%u.%u.%u.%u", &a, &b, &c, &d) != 4)
    {
        fprintf(stderr, "Erreur de format IP\n");
        exit(EXIT_FAILURE);
    }
    adrIP->bytes[0] = (uint8_t)a;
    adrIP->bytes[1] = (uint8_t)b;
    adrIP->bytes[2] = (uint8_t)c;
    adrIP->bytes[3] = (uint8_t)d;
}

/**
 * Initialise une adresse MAC à partir d'une chaîne de caractères.
 * @param adrMAC Pointeur vers la structure adresseMAC à remplir.
 * @param mac_str Chaîne de caractères représentant l'adresse MAC (ex: "01:23:45:67:89:ab").
 */
void init_adrMAC_from_text(adresseMAC *adrMAC, const char *mac_str)
{
    unsigned int values[TAILLE_MAC_OCTETS];
    if (sscanf(mac_str, "%x:%x:%x:%x:%x:%x", &values[0], &values[1], &values[2], &values[3], &values[4], &values[5]) != 6)
    {
        fprintf(stderr, "Erreur de format MAC\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < TAILLE_MAC_OCTETS; i++)
        adrMAC->bytes[i] = (uint8_t)values[i];
}

/**
 * Initialise une structure Switch à partir d'une ligne de texte.
 * @param s_switch Pointeur vers la structure Switch à remplir.
 * @param switch_str Ligne de texte décrivant le switch (ex: "2;01:45:23:a6:f7:ab;8;1024").
 */
void init_switch_from_text(Switch *s_switch, char *switch_str)
{
    size_t bin = 0, nb_port = 0, num_priorite = 0;
    char adr_mac_str[TAILLE_ADR_MAC_STRING];
    adresseMAC adr_MAC;

    if (sscanf(switch_str, "%lu;%[^;];%lu;%lu", &bin, adr_mac_str, &nb_port, &num_priorite) != 4)
    {
        fprintf(stderr, "Format de la ligne switch invalide : %s\n", switch_str);
        return;
    }

    init_adrMAC_from_text(&adr_MAC, adr_mac_str);
    init_switch(s_switch, nb_port, num_priorite, adr_MAC);
}

/**
 * Initialise une structure Station à partir d'une ligne de texte.
 * @param station Pointeur vers la structure Station à remplir.
 * @param station_str Ligne de texte décrivant la station (ex: "1;54:d6:a6:82:c5:23;130.79.80.21").
 */
void init_station_from_text(Station *station, char *station_str)
{
    int bin = 0;
    char adr_mac_str[TAILLE_ADR_MAC_STRING];
    char adr_ip_str[TAILLE_ADR_IP_STRING];
    adresseMAC adr_MAC;
    adresseIP adr_IP;

    if (sscanf(station_str, "%d;%[^;];%[^\n]", &bin, adr_mac_str, adr_ip_str) != 3)
    {
        fprintf(stderr, "Format de la ligne station invalide : %s\n", station_str);
        return;
    }

    init_adrMAC_from_text(&adr_MAC, adr_mac_str);
    init_adrIP_from_text(&adr_IP, adr_ip_str);
    init_station(station, adr_IP, adr_MAC);
}

/**
 * Initialise une structure arete à partir d'une ligne de texte et du graphe.
 * @param g Pointeur vers le graphe contenant les sommets.
 * @param a Pointeur vers la structure arete à remplir.
 * @param arete_str Ligne de texte décrivant l'arête (ex: "0;1;4").
 */
void init_arete_from_text(graphe *g, arete *a, char *arete_str)
{
    size_t index_sommet_1, index_sommet_2, poids = 0;

    if (sscanf(arete_str, "%lu;%lu;%lu", &index_sommet_1, &index_sommet_2, &poids) != 3)
    {
        fprintf(stderr, "Format de la ligne arete invalide : %s\n", arete_str);
        return;
    }

    a->s1 = g->sommet[index_sommet_1];
    a->s2 = g->sommet[index_sommet_2];
    a->poids = poids;
}

/**
 * Initialise un graphe à partir d'un fichier texte.
 * @param g Pointeur vers la structure graphe à initialiser.
 * @param path Chemin du fichier texte à lire.
 *
 * Le fichier doit contenir :
 *  - Une première ligne avec le nombre d'équipements et de liens.
 *  - Les lignes suivantes pour chaque équipement (station ou switch).
 *  - Les dernières lignes pour chaque lien (arête).
 */
void init_graph_from_file(graphe *g, const char *path)
{
    FILE *f = fopen(path, "r");
    if (f == NULL)
    {
        perror("Erreur d'ouverture");
        return;
    }

    size_t nb_equipement = 0, nb_lien = 0, num_ligne = 0, num_machine = 0;

    char ligne[TAILLE_MAX_LIGNE];

    while (fgets(ligne, sizeof(ligne), f) != NULL)
    {
        if (num_ligne == 0)
        {
            // Lecture de la première ligne : nombre d'équipements et de liens
            recuperer_nbEquipement_nbLien(ligne, &nb_equipement, &nb_lien);
            init_graphe(g);
        }
        // Lecture des lignes d'équipements (stations ou switchs)
        else if (num_ligne > 0 && num_ligne <= nb_equipement)
        {
            if (ligne[0] == '1')
            {
                Station machine;
                init_station_from_text(&machine, ligne);
                ajouter_sommet_station(g, machine);
                num_machine++;
            }
            else if (ligne[0] == '2')
            {
                Switch s_switch;
                init_switch_from_text(&s_switch, ligne);
                ajouter_sommet_switch(g, s_switch);
                num_machine++;
            }
        }
        // Lecture des lignes de liens (arêtes)
        else
        {
            arete a;
            init_arete_from_text(g, &a, ligne);
            ajouter_arete(g, a);
        }

        num_ligne++;
    }

    fclose(f);
}
