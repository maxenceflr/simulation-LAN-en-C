#include <stdint.h>
#include <stddef.h>

#define TAILLE_MAC_OCTETS 6
#define TAILLE_IP_OCTETS 4
#define TAILLE_MAX_ADRESSEMAC 500

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
    adresseMAC adrMac;
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

