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
    memcpy(&station->adrIP, &adrIP, TAILLE_IP_OCTETS);
    memcpy(&station->adrMAC, &adrMAC, TAILLE_MAC_OCTETS);
}

void init_port(port* r_port)
{
    r_port->nb_adressesMAC = 0;
    r_port->num_port = 0;
    r_port->tab_adresseMAC;
};

void init_switch(Switch* r_switch)
{
    r_switch
};