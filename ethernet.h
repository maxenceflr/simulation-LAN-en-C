#ifndef ETHERNET_H
#define ETHERNET_H

#include "reseau.h"

typedef struct Trame
{
    uint8_t preambule[7];
    uint8_t SFD;
    adresseMAC adrDest;
    adresseMAC adrSrc;
    uint16_t type;
    uint8_t *data;
    uint32_t FCS;
} Trame;

void init_trame(Trame *trame, adresseMAC adrDest, adresseMAC adrSrc, uint16_t type, adresseIP adrIP, const uint8_t *data, size_t data_length);
void deinit_trame(Trame *trame);
void afficher_tram_user(const Trame *trame);

#endif