#include "initReseau.h"

/**

 * @brief Récupère le nombre d'équipements et de liens à partir d'une ligne de texte.
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
 * @brief Initialise une adresse IP à partir d'une chaîne de caractères.
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

 * @brief Initialise une adresse MAC à partir d'une chaîne de caractères.
 * @param adrMAC Pointeur vers l'adresse MAC à initialiser.
 * @param mac_str Chaîne de caractères représentant l'adresse MAC.
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
 * @brief Initialise un switch à partir d'une ligne de texte.
 * @param s_switch Pointeur vers le switch à initialiser.
 * @param switch_str Ligne de texte décrivant le switch.
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

 * @brief Initialise une station à partir d'une ligne de texte.
 * @param station Pointeur vers la station à initialiser.
 * @param station_str Ligne de texte décrivant la station.
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
            printf("Il y a %zu liens, et %zu equipement\n", nb_lien, nb_equipement);
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
