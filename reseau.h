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

/**
 * @brief Structure représentant une adresse MAC.
 */
typedef struct adresseMAC
{
    uint64_t entier;                  /**< Représentation entière de l'adresse MAC */
    uint8_t bytes[TAILLE_MAC_OCTETS]; /**< Tableau d'octets de l'adresse MAC */
} adresseMAC;

/**
 * @brief Structure représentant une adresse IP.
 */
typedef struct adresseIP
{
    uint32_t entier;                 /**< Représentation entière de l'adresse IP */
    uint8_t bytes[TAILLE_IP_OCTETS]; /**< Tableau d'octets de l'adresse IP */
} adresseIP;

/**
 * @brief Structure représentant une station réseau.
 */
typedef struct Station
{
    size_t id;         /**< Identifiant de la station */
    adresseMAC adrMAC; /**< Adresse MAC de la station */
    adresseIP adrIP;   /**< Adresse IP de la station */
} Station;

/**
 * @brief Structure représentant un port réseau et ses adresses MAC associées.
 */
typedef struct port
{
    size_t nb_adressesMAC;      /**< Nombre d'adresses MAC enregistrées */
    size_t adresses_capacite;   /**< Capacité maximale d'adresses MAC */
    adresseMAC *tab_adresseMAC; /**< Tableau dynamique d'adresses MAC */
} port;

/**
 * @brief Structure représentant un switch réseau.
 */
typedef struct Switch
{
    size_t id;             /**< Identifiant du switch */
    adresseMAC adrMac;     /**< Adresse MAC du switch */
    size_t nb_ports;       /**< Nombre de ports */
    size_t priorite;       /**< Priorité du switch */
    port *tab_commutation; /**< Table de commutation (tableau de ports) */
} Switch;

void afficherIP(adresseIP adresse);
void afficherMAC(adresseMAC adresse);
void afficherSwitch(Switch sswitch);
void afficherStation(Station station);
void afficherPort(port r_port);
void afficherTableCommutation(Switch sswitch);
void init_station(Station *station, adresseIP adrIP, adresseMAC adrMAC);
void init_port(port *r_port);
void init_switch(Switch *r_switch, size_t nbPort, size_t numPriorite, adresseMAC adrMac);
void deinit_switch(Switch *r_switch);
void deinit_port(port *r_port);
void ajouter_adresse_port(port *port, adresseMAC adrMAC);
bool equals_adresseMAC(adresseMAC adr1, adresseMAC adr2);
bool equals_port(port p1, port p2);
bool equals_switch(Switch s1, Switch s2);
bool equals_station(Station s1, Station s2);

#endif