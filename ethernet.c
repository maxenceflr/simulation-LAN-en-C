#include "ethernet.h"

/**
 * @brief Initialise une trame Ethernet avec les paramètres donnés.
 * @param trame Pointeur vers la trame à initialiser.
 * @param adrDest Adresse MAC de destination.
 * @param adrSrc Adresse MAC source.
 * @param type Type de la trame.
 * @param adrIP Adresse IP (non utilisée ici, mais peut servir pour les données).
 * @param data Pointeur vers les données à inclure dans la trame.
 * @param data_length Taille des données.
 */

void init_trame(Trame *trame, adresseMAC adrDest, adresseMAC adrSrc, uint16_t type, adresseIP adrIP, const uint8_t *data, size_t data_length)
{
    if (trame == NULL)
    {
        fprintf(stderr, "Erreur d'initialisation de la trame\n");
        return;
    }

    // Initialisation du preambule
    for (int i = 0; i < 7; i++)
    {
        trame->preambule[i] = 0xAA;
    }

    // Initialisation du SFD
    trame->SFD = 0xAB;

    // Initialisation des adresses MAC
    trame->adrDest = adrDest;
    trame->adrSrc = adrSrc;

    // Initialisation du type
    trame->type = type;

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

/**
 * @brief Affiche le contenu d'une trame Ethernet pour l'utilisateur.
 * @param trame Pointeur constant vers la trame à afficher.
 */
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

bool envoyer_tram(const Trame *trame, graphe *g)
{
    if (trame == NULL)
    {
        fprintf(stderr, "Erreur d'envoi de la trame: trame est NULL\n");
        return false; // Retourne false si la trame est NULL
    }

    adresseMAC adrDest = trame->adrDest;
    adresseMAC adrSrc = trame->adrSrc;

    sommet source;
    sommet destination;

    for (size_t i = 0; i < g->ordre; i++)
    {
        if (g->sommet[i].station.adrMAC.entier == adrSrc.entier)
        {
            source = g->sommet[i];
        }
        else if (g->sommet[i].station.adrMAC.entier == adrDest.entier)
        {
            destination = g->sommet[i];
        }
    }

    return sont_connectes(g, source, destination); // Retourne true si l'envoi a réussi
}