#include <stdint.h>
#include <stddef.h>

typedef uint32_t adresseIP;
typedef uint8_t adresseMAC[6];

// Structure de donnée représentant une station

typedef struct station
{
    adresseMAC adr_mac;
    adresseIP adr_ip;
}station;

// Structure de donnée représentant un switch

typedef struct s_switch
{
    adresseMAC adr_mac;
    size_t nb_ports;
    size_t priorite;

}s_switch;