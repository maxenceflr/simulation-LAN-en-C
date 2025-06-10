#ifndef RESEAU_H
#define RESEAU_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define TAILLE_MAC_OCTETS 6
#define TAILLE_IP_OCTETS 4
#define TAILLE_ADRESSE_MAX 500
#define TAILLE_ADR_IP_STRING 16

// Structure de donnée représentant une adresse MAC

typedef struct adresseMAC
{
    uint64_t entier;
    uint8_t bytes[TAILLE_MAC_OCTETS];
} adresseMAC;

// Structure de donnée représentant une adresse IP

typedef struct adresseIP
{
    uint32_t entier;
    uint8_t bytes[TAILLE_IP_OCTETS];
} adresseIP;

// Structure de donnée représentant une station

typedef struct Station
{
    size_t id;
    adresseMAC adrMAC;
    adresseIP adrIP;
} Station;

// Structure de donnée représentant un port et les adresses MAC associées

typedef struct port
{
    size_t nb_adressesMAC;
    size_t adresses_capacite;
    adresseMAC *tab_adresseMAC;
} port;

// Structure de donnée représentant un switch

typedef struct Switch
{
    size_t id;
    adresseMAC adrMac;
    size_t nb_ports;
    size_t priorite;
    port *tab_commutation;
} Switch;

// Fonctions d'affichage

void afficherIP(adresseIP adresse);
void afficherMAC(adresseMAC adresse);
void afficherSwitch(Switch sswitch);
void afficherStation(Station station);
void afficherPort(port r_port);
void afficherTableCommutation(Switch sswitch);

// Fonctions d'initialisation

void init_station(Station *station, adresseIP adrIP, adresseMAC adrMAC);
void init_port(port *r_port);
void init_switch(Switch *r_switch, size_t nbPort, size_t numPriorite, adresseMAC adrMac);
void deinit_switch(Switch *r_switch);
void deinit_port(port *r_port);

// Fonctions utilitaires

void ajouter_adresse_port(port *port, adresseMAC adrMAC);
bool equals_port(port p1, port p2);
bool equals_switch(Switch s1, Switch s2);
bool equals_station(Station s1, Station s2);

#endif