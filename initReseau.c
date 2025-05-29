#include <stdint.h>
#include <stdio.h>
#include <reseau.c>
#include <string.h>
#include <graphe.c>

#define TAILLE_ADR_MAC_STRING 18
#define TAILLE_ADR_IP_STRING 16

void recuperer_nbEquipement_nbLien(const char *ligne, size_t* nb_equipement, size_t* nb_lien)
{
    if (sscanf(ligne, "%lu %lu", nb_equipement, nb_lien) == 2)
    {
        printf("Nombre d'équipements : %d\n", *nb_equipement);
        printf("Nombre de liens : %d\n", *nb_lien);
    } 
    else 
    {
        fprintf(stderr, "Erreur de lecture du nombre d'équipements.\n");
        return;
    }
}   


void init_adrIP_from_text(adresseIP* adrIP, const char* ip_str)
{
    uint32_t a, b, c, d;

    if (sscanf(ip_str, "%u.%u.%u.%u", &a, &b, &c, &d) != 4) {
        fprintf(stderr, "Format IP invalide : %s\n", ip_str);
        return ;
    }

    else if (a > 255 || b > 255 || c > 255 || d > 255) {
        fprintf(stderr, "Valeur d’octet invalide dans l’adresse IP : %s\n", ip_str);
        return ;
    }

    adrIP -> bytes[0] = (uint8_t)a;
    adrIP -> bytes[1] = (uint8_t)b;
    adrIP -> bytes[2] = (uint8_t)c;
    adrIP -> bytes[3] = (uint8_t)d;

    adrIP -> entier = (a << 24) | (b << 16) | (c << 8) | d;
}

void init_adrMAC_from_text(adresseMAC* adrMAC, const char* mac_str)
{
    int values[TAILLE_MAC_OCTETS];
    
    if (sscanf(mac_str, "%x:%x:%x:%x:%x:%x", &values[0], &values[1], &values[2], &values[3], &values[4], &values[5]) != 6) 
    {
        fprintf(stderr, "Format MAC invalide : %s\n", mac_str);
        return;
    }

    for (int i = 0; i < TAILLE_MAC_OCTETS; i++) 
    {
        if (values[i] < 0 || values[i] > 255)
        {
            fprintf(stderr, "Valeur d’octet invalide dans l’adresse MAC : %s\n", mac_str);
            return;
        }

        adrMAC->bytes[i] = (uint8_t)values[i];
        adrMAC->entier = (adrMAC->entier << 8) | (uint64_t)values[i];
    }
}

void init_switch_from_text(Switch* s_switch, char* switch_str)
{
    size_t bin, nb_port, num_priorite = 0;
    char adr_mac_str[TAILLE_ADR_MAC_STRING];
    adresseMAC adr_MAC;

    if(sscanf(switch_str, "%d;%[^;];%d;%d", &bin, adr_mac_str, &nb_port, &num_priorite) != 4)
    {
        fprintf(stderr, "Format de la ligne switch invalide : %s\n", switch_str);
        return;
    }
    
    init_adrMAC_from_text(&adr_MAC, adr_mac_str);
    init_switch(s_switch, nb_port, num_priorite, adr_MAC);
}

void init_station_from_text(Station* station, char* station_str)
{
    size_t bin = 0;
    char adr_mac_str[TAILLE_ADR_MAC_STRING];
    char adr_ip_str[TAILLE_ADR_IP_STRING];
    adresseMAC adr_MAC;
    adresseIP adr_IP;

    if(sscanf(station, "%d;%[^;];%[^\n]", &bin, adr_mac_str, adr_ip_str != 3))
    {
        fprintf(stderr, "Format de la ligne station invalide : %s\n", station_str);
        return;
    }

    init_adrMAC_from_text(&adr_MAC, adr_mac_str);
    init_adrIP_from_text(&adr_IP, adr_ip_str);
}

void init_arete_from_text(graphe* g, arete* a, char* arete_str)
{
    size_t index_sommet_1, index_sommet_2, poids = 0;

    if(sscanf(arete_str, "%d;%d;%d", &index_sommet_1, &index_sommet_2, &poids != 3))
    {
        fprintf(stderr, "Format de la ligne arete invalide : %s\n", arete_str);
        return;
    }    
    
    a->s1 = g->sommet[index_sommet_1];
    a->s2 = g->sommet[index_sommet_2];
    a->poid = poids;
}

/*
void init_graphe_from_file(graphe* g, const char *f) 
{
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }
}



void recupererSwitch(const char *f)
{
    FILE *fichier = fopen(f, "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    char ligne[256];
    boolean a = true;
    while(a)
    {
        if (fgets(ligne, sizeof(ligne), fichier)) {
            int entier1, entier2, entier3;
            unsigned int hex1, hex2, hex3, hex4, hex5, hex6;
            
            int nbLus = sscanf(ligne, "%d %x %x %x %x %x %x %d %d",&entier1, &hex1, &hex2, &hex3, &hex4, &hex5, &hex6, &entier2, &entier3);
            if (entier1!=1)
            {
                a = false;
            }
            else if (nbLus == 9) {
                

                printf("Entier1 : %d\n", entier1);
                printf("Hexas   : %02x %02x %02x %02x %02x %02x\n", hex1, hex2, hex3, hex4, hex5, hex6);
                printf("Entier2 : %d\n", entier2);
                printf("Prioriter :  %d", entierBinaire);
            } else {
                printf("Erreur : données manquantes ou format incorrect.\n");
            }
            }
    }

    fclose(fichier);
}


void recupererEquipement(const char *f)
{
    FILE *fichier = fopen(f, "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }
    char ligne[256];
    boolean a = true;
    while(a)
    {
        if (fgets(ligne, sizeof(ligne), fichier)) {
            int entier1, entier2, entier3, entier4, entier5;
            unsigned int hex1, hex2, hex3, hex4, hex5, hex6;
            int nbLus = sscanf(ligne, "%d %x %x %x %x %x %x %d %d %d %d",&entier1, &hex1, &hex2, &hex3, &hex4, &hex5, &hex6, &entier2, &entier3);
            if (entier1!=1)
            {
                a = false;
            }
            else if (nbLus == 11) {
                

                printf("Entier1 : %d\n", entier1);
                printf("Hexas   : %02x %02x %02x %02x %02x %02x\n", hex1, hex2, hex3, hex4, hex5, hex6);
                printf("adresse Mac : %d\n", entier2, entier3, entier4, entier5);
            } else {
                printf("Erreur : données manquantes ou format incorrect.\n");
            }
        }
    }
    fclose(fichier);
}


void recupererNbLien(const char *f)
{
    FILE *fichier = fopen(f, "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    int temp, nbLien;
    if (fscanf(fichier, "%d %d", &temp, &nbLien) == 2) {
        printf("Nombre de liens : %d\n", nbLien);
    } else {
        printf("Erreur de lecture du nombre de liens.\n");
    }

    fclose(fichier);
}

void recupererSwitch(const char *f)
{
    FILE *fichier = fopen(f, "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    int verif, nbLien;
    if (fscanf(fichier, "%d %d", &temp, &nbLien) == 2) {
        printf("Nombre de liens : %d\n", nbLien);
    } else {
        perror("Erreur de lecture du nombre de liens.\n");
    }

    fclose(fichier);
}
*/