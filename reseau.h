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

// Structure représentant une adresse MAC (6 octets)
typedef struct adresseMAC {
    uint64_t entier;                  // Représentation entière (les 48 bits sont utiles)
    uint8_t bytes[TAILLE_MAC_OCTETS];   // Tableau d'octets
} adresseMAC;

// Structure représentant une adresse IP (4 octets)
typedef struct adresseIP {
    uint32_t entier;                  // Représentation entière
    uint8_t bytes[TAILLE_IP_OCTETS];    // Tableau d'octets
} adresseIP;

// États possibles d'un port dans STP
typedef enum {
    STP_INCONNU,
    STP_RACINE,
    STP_DESIGNE,
    STP_BLOQUE
} etatPortSTP;

// Structure représentant un port, modifiée pour stocker l’équipement connecté.
typedef struct port {
    size_t nb_adressesMAC;
    size_t adresses_capacite;   // Capacité du tableau d'adresses MAC
    etatPortSTP etat;
    adresseMAC *tab_adresseMAC; // Adresses MAC apprises sur ce port
    int connectedEquipment;     // Index de l'équipement relié dans le graphe (-1 si non associé)
} port;

/**
 * Structure représentant une station réseau.
 */
typedef struct Station {
    size_t id;
    adresseMAC adrMac;
    adresseIP adrIP;
} Station;

/**
 * Structure représentant un switch réseau.
 */
typedef struct Switch {
    size_t id;
    adresseMAC adrMac;
    size_t nb_ports;
    size_t priorite;
    port *tab_commutation; // Table de commutation (tableau de ports)
} Switch;

// Fonctions d'affichage
void afficherIP(adresseIP adresse);
void afficherMAC(adresseMAC adresse);
void afficherSwitch(Switch sswitch);
void afficherStation(Station station);
void afficherPort(port r_port);
void afficherTableCommutation(Switch sswitch);

// Fonctions d'initialisation et gestion
void deinit_switch(Switch *r_switch);
void init_switch(Switch *r_switch, size_t nbPort, size_t numPriorite, adresseMAC adrMac);
void init_station(Station *station, adresseIP adrIP, adresseMAC adrMAC);
void init_port(port *r_port);
void deinit_port(port *r_port);
void ajouter_adresse_port(port *port, adresseMAC adrMAC);
bool equals_adresseMAC(adresseMAC adr1, adresseMAC adr2);
bool equals_port(port p1, port p2);
bool equals_station(Station s1, Station s2);
bool equals_switch(Switch s1, Switch s2);

// Double déclaration volontaire (pour adapter le code existant)
void ajouter_adresse_port(port *port, adresseMAC adrMAC);

#endif
