#include <reseau.h>
#include <string.h>

void afficherIP(adresseIP adresse)
{
    printf("%u.%u.%u.%u", adresse.bytes[0], adresse.bytes[1], adresse.bytes[2], adresse.bytes[3]);
}

void afficherMAC(adresseMAC adresse)
{
    for(int i = 0; i < TAILLE_MAC_OCTETS; i++)
    {
        if (i == TAILLE_MAC_OCTETS -1)
        {
            printf("%02x", adresse.bytes[i]);
        }
        else
        {
            printf("%02x:", adresse.bytes[i]);
        }
    }
}

void afficherTableCommutation(Switch r_switch)
{
    for(size_t i = 0; i < r_switch.nb_ports; i++)
    {
        printf("Port %u:\t", i);
        size_t nb_adresseMac = r_switch.tab_commutation[i].nb_adressesMAC; 

        for(size_t j = 0; j < nb_adresseMac; j++)
        {
            afficherMAC(r_switch.tab_commutation[i].tab_adresseMAC[j]);
            printf("\n        \t");
        }

        printf("\n");
    }
}


void init_station(Station* station, adresseIP adrIP, adresseMAC adrMAC)
{
    memcpy(&(station->adrIP), &adrIP, TAILLE_IP_OCTETS);
    memcpy(&(station->adrMAC), &adrMAC, TAILLE_MAC_OCTETS);
}

void init_port(port* r_port, size_t numPort, size_t nbAdresses)
{
    r_port->num_port = numPort;
    r_port->nb_adressesMAC = nbAdresses;
    r_port->tab_adresseMAC = malloc(TAILLE_ADRESSE_MAX * sizeof(adresseMAC));
}

void init_switch(Switch* r_switch, size_t nbPort, size_t numPriorite)
{
    r_switch -> nb_ports = nbPort;
    r_switch -> priorite = numPriorite;
    r_switch -> tab_commutation = malloc(nbPort * sizeof(port));

    for(size_t i = 0; i < nbPort; i++)
    {
        init_port(&(r_switch->tab_commutation)[i], i, 0);
    }
}

void ajouter_adresse_port(port* port, adresseMAC adrMAC)
{   
    port->tab_adresseMAC[port->nb_adressesMAC] = adrMAC;
}