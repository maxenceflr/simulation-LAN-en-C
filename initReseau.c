#include "initReseau.h"

void recuperer_nbEquipement_nbLien(const char *ligne, size_t *nb_equipement, size_t *nb_lien)
{
    if (sscanf(ligne, "%lu %lu", nb_equipement, nb_lien) == 2)
    {
    }
    else
    {
        fprintf(stderr, "Erreur de lecture du nombre d'équipements.\n");
        return;
    }
}

void init_adrIP_from_text(adresseIP *adrIP, const char *ip_str)
{
    uint32_t a, b, c, d;

    if (sscanf(ip_str, "%u.%u.%u.%u", &a, &b, &c, &d) != 4)
    {
        fprintf(stderr, "Format IP invalide : %s\n", ip_str);
        return;
    }

    else if (a > 255 || b > 255 || c > 255 || d > 255)
    {
        fprintf(stderr, "Valeur d’octet invalide dans l’adresse IP : %s\n", ip_str);
        return;
    }

    adrIP->bytes[0] = (uint8_t)a;
    adrIP->bytes[1] = (uint8_t)b;
    adrIP->bytes[2] = (uint8_t)c;
    adrIP->bytes[3] = (uint8_t)d;

    adrIP->entier = (a << 24) | (b << 16) | (c << 8) | d;
}

void init_adrMAC_from_text(adresseMAC *adrMAC, const char *mac_str)
{
    unsigned int values[TAILLE_MAC_OCTETS];

    if (sscanf(mac_str, "%x:%x:%x:%x:%x:%x", &values[0], &values[1], &values[2], &values[3], &values[4], &values[5]) != 6)
    {
        fprintf(stderr, "Format MAC invalide : %s\n", mac_str);
        return;
    }

    for (int i = 0; i < TAILLE_MAC_OCTETS; i++)
    {
        if (values[i] > 255)
        {
            fprintf(stderr, "Valeur d’octet invalide dans l’adresse MAC : %s\n", mac_str);
            return;
        }

        adrMAC->bytes[i] = (uint8_t)values[i];
        adrMAC->entier = (adrMAC->entier << 8) | (uint64_t)values[i];
    }
}

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
            recuperer_nbEquipement_nbLien(ligne, &nb_equipement, &nb_lien);
            init_graphe(g);
        }

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
