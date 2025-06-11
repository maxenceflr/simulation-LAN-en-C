#ifndef RESEAU_H
#define RESEAU_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Définitions des tailles pour les adresses
#define TAILLE_MAC_OCTETS 6
#define TAILLE_IP_OCTETS 4
#define TAILLE_ADRESSE_MAX 500
#define TAILLE_ADR_IP_STRING 16

// Structure représentant une adresse MAC (6 octets)
typedef struct adresseMAC
{
    uint64_t entier;
    uint8_t bytes[TAILLE_MAC_OCTETS];
} adresseMAC;

// Structure représentant une adresse IP (4 octets)
typedef struct adresseIP
{
    uint32_t entier;
    uint8_t bytes[TAILLE_IP_OCTETS];
} adresseIP;

// Structure représentant une station (équipement terminal)
typedef struct Station
{
    adresseIP adrIP;   // Adresse IP de la station
    adresseMAC adrMac; // Adresse MAC de la station
    size_t id;         // Identifiant unique de la station
} Station;

typedef enum {
    STP_INCONNU,
    STP_RACINE,
    STP_DESIGNE,
    STP_BLOQUE
} etatPortSTP;
// Structure représentant un port de switch
typedef struct port
{
    size_t nb_adressesMAC;
    size_t adresses_capacite;   // Nombre d'adresses MAC connues sur ce port
    etatPortSTP etat;
    adresseMAC *tab_adresseMAC; // Tableau dynamique d'adresses MAC connues sur ce port
} port;

// Structure représentant un switch réseau
typedef struct Switch
{
    port *tab_commutation; // Table de commutation (tableau de ports)
    adresseMAC adrMac;     // Adresse MAC du switch
    size_t nb_ports;       // Nombre de ports
    size_t priorite;       // Priorité STP
    size_t id;             // Identifiant unique du switch
} Switch;

// Fonctions d'affichage
void afficherIP(adresseIP adresse);
void afficherMAC(adresseMAC adresse);
void afficherSwitch(Switch sswitch);
void afficherStation(Station station);
void afficherPort(port r_port);
void afficherTableCommutation(Switch sswitch);

// Fonctions d'initialisation
void deinit_switch(Switch *r_switch);
void init_switch(Switch *r_switch, size_t nbPort, size_t numPriorite, adresseMAC adrMac);
void init_station(Station *station, adresseIP adrIP, adresseMAC adrMAC);
void init_port(port *r_port);
void deinit_port(port *r_port);

// Fonctions de comparaison
bool equals_port(port p1, port p2);
bool equals_station(Station s1, Station s2);
bool equals_switch(Switch s1, Switch s2);

// Fonctions de gestion des ports

void ajouter_adresse_port(port *port, adresseMAC adrMAC);

#endif