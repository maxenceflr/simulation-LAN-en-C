#ifndef ETHERNET_H
#define ETHERNET_H

#include "reseau.h"
#include "graphe.h"

/**
 * @brief Structure représentant une trame Ethernet.
 */
typedef struct Trame
{
    uint8_t preambule[7]; /**< Préambule Ethernet */
    uint8_t SFD;          /**< Start Frame Delimiter */
    adresseMAC adrDest;   /**< Adresse MAC de destination */
    adresseMAC adrSrc;    /**< Adresse MAC source */
    uint16_t type;        /**< Type de la trame */
    uint8_t *data;        /**< Données de la trame */
    uint32_t FCS;         /**< Frame Check Sequence */
} Trame;

void init_trame(Trame *trame, adresseMAC adrDest, adresseMAC adrSrc, uint16_t type, adresseIP adrIP, const uint8_t *data, size_t data_length);
void deinit_trame(Trame *trame);
void afficher_tram_user(const Trame *trame);
bool envoyer_tram(const Trame *trame, graphe *g);

#endif