#include <stdint.h>
#include <stdio.h>
#include <reseau.c>


void recupererNbEquipement(const char *f)
{
    FILE *fichier = fopen(f, "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    int nbEquipement;
    if (fscanf(fichier, "%d", &nbEquipement) == 1) {
        printf("Nombre d'équipements : %d\n", nbEquipement);
    } else {
        printf("Erreur de lecture du nombre d'équipements.\n");
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
