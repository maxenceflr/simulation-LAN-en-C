#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define TAILLE_MAC_OCTETS 6
#define TAILLE_IP_OCTETS 4
#define TAILLE_ADRESSE_MAX 500

// Structure de donnée représentant une adresse MAC

typedef struct {
    uint64_t entier;
    uint8_t bytes[TAILLE_MAC_OCTETS];
} adresseMAC;

// Structure de donnée représentant une adresse IP

typedef struct {
    uint32_t entier;
    uint8_t bytes[TAILLE_IP_OCTETS];
} adresseIP;

// Structure de donnée représentant une station

typedef struct Station
{
    adresseMAC adrMAC;
    adresseIP adrIP;
}Station;

// Structure de donnée représentant un port et les adresses MAC associées

typedef struct port
{
    size_t num_port;
    size_t nb_adressesMAC;
    adresseMAC* tab_adresseMAC;
}port;

// Structure de donnée représentant un switch 

typedef struct Switch
{
    adresseMAC adrMac;
    size_t nb_ports;
    size_t priorite;
    port* tab_commutation;
}Switch;  

// Fonctions d'affichage

void afficherIP(adresseIP adresse);
void afficherMAC(adresseMAC adresse);
void afficherTableCommutation(Switch sswitch);

// Fonctions d'initialisation 

void init_station(Station* station, adresseIP adrIP, adresseMAC adrMAC);
void init_port(port* r_port, size_t num_port, size_t nbAdresses);
void init_switch(Switch* r_switch, size_t nbPort, size_t numPriorite, adresseMAC adrMac);

// Fonctions utilitaires

void ajouter_adresse_port(port* port, adresseMAC adrMAC);
void ajouter_sommet_station(graphe *g, Station station);
void ajouter_sommet_switch(graphe *g, Switch s_switch);
bool equals_port(port p1, port p2);
bool equals_switch(Switch s1, Switch s2);
bool equals_station(Station s1, Station s2);