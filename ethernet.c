#include "ethernet.h"

void init_trame(Trame *trame, adresseMAC adrDest, adresseMAC adrSrc, uint16_t type, adresseIP adrIP, const uint8_t *data, size_t data_length)
{
    if (trame == NULL)
    {
        fprintf(stderr, "Erreur d'initialisation de la trame\n");
        return;
    }

    
    for (int i = 0; i < 7; i++)
    {
        trame->preambule[i] = 0xAA;
    }
    trame->SFD = 0xAB;
    trame->adrDest = adrDest;
    trame->adrSrc = adrSrc;trame->type = type;

    // Initialisation des données
    if (data != NULL && data_length > 0)
    {
        trame->data = malloc(data_length);
        if (trame->data == NULL)
        {
            fprintf(stderr, "Erreur d'allocation de mémoire pour les données\n");
            return;
        }
        memcpy(trame->data, data, data_length);
    }
    else
    {
        trame->data = NULL;
    }

    // Calcul du FCS (Frame Check Sequence)
    trame->FCS = 0; // Pour simplifier, on initialise à 0. Le calcul réel dépendra des données.
}

void deinit_trame(Trame *trame)
{
    if (trame == NULL)
    {
        fprintf(stderr, "Erreur de désinitialisation de la trame\n");
        return;
    }

    // Libération de la mémoire allouée pour les données
    if (trame->data != NULL)
    {
        free(trame->data);
        trame->data = NULL;
    }

    // Réinitialisation des champs de la trame

    memset(trame->preambule, 0, sizeof(trame->preambule));
    trame->SFD = 0;
    memset(&trame->adrDest, 0, sizeof(adresseMAC));
    memset(&trame->adrSrc, 0, sizeof(adresseMAC));
    trame->type = 0;
    trame->FCS = 0;
}

void afficher_tram_user(const Trame *trame)
{
    if (trame == NULL)
    {
        fprintf(stderr, "Erreur de lecture de la trame\n");
        return;
    }

    printf("Preambule: ");
    for (int i = 0; i < 7; i++)
    {
        printf("%02x ", trame->preambule[i]);
    }

    printf("\n");

    printf("SFD: %02x\n", trame->SFD);

    printf("Adresse Destinataire: ");
    for (int i = 0; i < 6; i++)
    {
        printf("%02x ", trame->adrDest.bytes[i]);
    }
    printf("\n");

    printf("Adresse Source: ");
    for (int i = 0; i < 6; i++)
    {
        printf("%02x ", trame->adrSrc.bytes[i]);
    }
    printf("\n");

    printf("Type: %02x\n", trame->type);

    if (trame->data != NULL)
    {
        printf("Data: ");
        for (size_t i = 0; i < sizeof(trame->data); i++)
        {
            printf("%02x ", trame->data[i]);
        }
        printf("\n");
    }
    else
    {
        printf("Data: NULL\n");
    }

    printf("FCS: %08x\n", trame->FCS);
}
bool detct_cycle_recursif(graphe *g, int current, int parent, bool visited[]) {
    visited[current] = true;
    sommet voisins[NOMBRE_SOMMETS_MAX];
    size_t nb_voisins = sommets_adjacents(g, g->sommet[current], voisins);
    
    for (size_t i = 0; i < nb_voisins; i++) {
        int idx = index_sommet(g, voisins[i]);
        if (!visited[idx]) {
            if (detct_cycle_recursif(g, idx, current, visited))
                return true;
        } else if (idx != parent) {
            // Cycle détecté : le voisin déjà visité n'est pas le parent.
            return true;
        }
    }
    return false;
}

bool detecter_cycle(graphe *g) {
    bool *visited = malloc(g->ordre * sizeof(bool));
    if (visited==NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < g->ordre; i++) {
        visited[i] = false; // 0 en C
    }

    bool cycle = false;
    for (size_t i = 0; i < g->ordre; i++) {
        if (!visited[i]) {
            if (detct_cycle_recursif(g, i, -1, visited)) {
                cycle = true;
                break;
            }
        }
    }
    free(visited);
    return cycle;
}



bool envoyer_tram(const Trame *trame, graphe *g) {
    if (trame == NULL) {
        printf("Erreur d'envoi de la trame : trame est NULL\n");
        return false;
    }


    adresseMAC adrDest = trame->adrDest;
    adresseMAC adrSrc = trame->adrSrc;

    sommet *source = NULL;
    sommet *destination = NULL;

    // Recherche des sommets correspondants aux adresses MAC source et destination
    for (size_t i = 0; i < g->ordre; i++) {
        if (equals_adresseMAC(g->sommet[i].station.adrMac, adrSrc)) {
            source = &g->sommet[i];
        }
        if (equals_adresseMAC(g->sommet[i].station.adrMac, adrDest)) {
            destination = &g->sommet[i];
        }
    }

    if (!source || !destination) {
        printf("Erreur : Source ou Destination introuvable dans le réseau.\n");
        return false;
    }

    // Affichage correct des adresses MAC
    printf("Adresse MAC source : ");
    afficherMAC(source->station.adrMac);
    printf("\nAdresse MAC destination : ");
    afficherMAC(destination->station.adrMac);
    printf("\n");

    if (sont_connectes(g, *source, *destination)) {
        if(detecter_cycle(g))//retourne vrai si cycle
        {
            printf("La trame de ");
            afficherMAC(source->station.adrMac);
            printf(", vers ");
            afficherMAC(destination->station.adrMac);
            printf(" n'est pas garanti si le reseau n'est pas correctement initialisé");
            printf("\n");
            return true;
        }
        else
        {
            printf("Envoi de la trame de ");
            afficherMAC(source->station.adrMac);
            printf(", vers ");
            afficherMAC(destination->station.adrMac);
            printf("\n");
            return true;
        }
    }

    printf("Échec de l'envoi : Stations non connectées.\n");
    return false;
}
