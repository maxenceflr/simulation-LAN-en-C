#ifndef ETHERNET_H
#define ETHERNET_H

#include "reseau.h"

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

/**
 * @brief Initialise une trame Ethernet.
 * @param trame Pointeur vers la trame à initialiser.
 * @param adrDest Adresse MAC de destination.
 * @param adrSrc Adresse MAC source.
 * @param type Type de la trame.
 * @param adrIP Adresse IP (optionnel).
 * @param data Pointeur vers les données.
 * @param data_length Taille des données.
 */
void init_trame(Trame *trame, adresseMAC adrDest, adresseMAC adrSrc, uint16_t type, adresseIP adrIP, const uint8_t *data, size_t data_length);

/**
 * @brief Libère la mémoire d'une trame Ethernet.
 * @param trame Pointeur vers la trame à désinitialiser.
 */
void deinit_trame(Trame *trame);

/**
 * @brief Affiche le contenu d'une trame Ethernet.
 * @param trame Pointeur constant vers la trame à afficher.
 */
void afficher_tram_user(const Trame *trame);

#endif