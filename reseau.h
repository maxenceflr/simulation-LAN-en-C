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

// Fonctions d'affichage

/**
 * @brief Affiche une adresse IP.
 * @param adresse Adresse IP à afficher.
 */
void afficherIP(adresseIP adresse);

/**
 * @brief Affiche une adresse MAC.
 * @param adresse Adresse MAC à afficher.
 */
void afficherMAC(adresseMAC adresse);

/**
 * @brief Affiche les informations d'un switch.
 * @param sswitch Switch à afficher.
 */
void afficherSwitch(Switch sswitch);

/**
 * @brief Affiche les informations d'une station.
 * @param station Station à afficher.
 */
void afficherStation(Station station);

/**
 * @brief Affiche les informations d'un port.
 * @param r_port Port à afficher.
 */
void afficherPort(port r_port);

/**
 * @brief Affiche la table de commutation d'un switch.
 * @param sswitch Switch dont la table doit être affichée.
 */
void afficherTableCommutation(Switch sswitch);

// Fonctions d'initialisation

/**
 * @brief Initialise une station.
 * @param station Pointeur vers la station à initialiser.
 * @param adrIP Adresse IP à affecter.
 * @param adrMAC Adresse MAC à affecter.
 */
void init_station(Station *station, adresseIP adrIP, adresseMAC adrMAC);

/**
 * @brief Initialise un port réseau.
 * @param r_port Pointeur vers le port à initialiser.
 */
void init_port(port *r_port);

/**
 * @brief Initialise un switch réseau.
 * @param r_switch Pointeur vers le switch à initialiser.
 * @param nbPort Nombre de ports.
 * @param numPriorite Priorité du switch.
 * @param adrMac Adresse MAC du switch.
 */
void init_switch(Switch *r_switch, size_t nbPort, size_t numPriorite, adresseMAC adrMac);

/**
 * @brief Libère la mémoire et réinitialise un switch.
 * @param r_switch Pointeur vers le switch à désinitialiser.
 */
void deinit_switch(Switch *r_switch);

/**
 * @brief Libère la mémoire d'un port réseau.
 * @param r_port Pointeur vers le port à désinitialiser.
 */
void deinit_port(port *r_port);

// Fonctions utilitaires

/**
 * @brief Ajoute une adresse MAC à un port.
 * @param port Pointeur vers le port.
 * @param adrMAC Adresse MAC à ajouter.
 */
void ajouter_adresse_port(port *port, adresseMAC adrMAC);

/**
 * @brief Compare deux ports.
 * @param p1 Premier port.
 * @param p2 Deuxième port.
 * @return true si identiques, false sinon.
 */
bool equals_port(port p1, port p2);

/**
 * @brief Compare deux switches.
 * @param s1 Premier switch.
 * @param s2 Deuxième switch.
 * @return true si identiques, false sinon.
 */
bool equals_switch(Switch s1, Switch s2);

/**
 * @brief Compare deux stations.
 * @param s1 Première station.
 * @param s2 Deuxième station.
 * @return true si identiques, false sinon.
 */
bool equals_station(Station s1, Station s2);

#endif