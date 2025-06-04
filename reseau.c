#include "reseau.h"

/**
 * Affiche une adresse IP au format classique (ex: 192.168.1.1)
 * @param adresse Structure adresseIP à afficher.
 */
void afficherIP(adresseIP adresse)
{
    printf("%u.%u.%u.%u", adresse.bytes[0], adresse.bytes[1], adresse.bytes[2], adresse.bytes[3]);
}

/**
 * Affiche une adresse MAC au format classique (ex: 01:23:45:67:89:ab)
 * @param adresse Structure adresseMAC à afficher.
 */
void afficherMAC(adresseMAC adresse)
{
    for (int i = 0; i < TAILLE_MAC_OCTETS; i++)
    {
        printf("%02x", adresse.bytes[i]);
        if (i < TAILLE_MAC_OCTETS - 1)
            printf(":");
    }
}

/**
 * Affiche les informations d'un switch réseau.
 * @param sswitch Structure Switch à afficher.
 */
void afficherSwitch(Switch sswitch)
{
    printf("Switch SW%lu", sswitch.id);
    printf("\n\t- Adresse MAC: ");
    afficherMAC(sswitch.adrMac);
    printf("\n\t- Nombre de ports: %lu", sswitch.nb_ports);
    printf("\n\t- Priorité: %lu\n", sswitch.priorite);
}

/**
 * Affiche les informations d'une station réseau.
 * @param station Structure Station à afficher.
 */
void afficherStation(Station station)
{
    printf("Station ST%lu", station.id);
    printf("\n\t- Adresse MAC: ");
    afficherMAC(station.adrMac);
    printf("\n\t- Adresse IP: ");
    afficherIP(station.adrIP);
    printf("\n");
}

/**
 * Affiche la table de commutation d'un switch (adresses MAC connues par port).
 * @param r_switch Structure Switch à afficher.
 */
void afficherTableCommutation(Switch r_switch)
{
    for (size_t i = 0; i < r_switch.nb_ports; i++)
    {
        printf("Port %lu:\t", i);
        size_t nb_adresseMac = r_switch.tab_commutation[i].nb_adressesMAC;

        for (size_t j = 0; j < nb_adresseMac; j++)
        {
            afficherMAC(r_switch.tab_commutation[i].tab_adresseMAC[j]);
            printf("\n        \t");
        }

        printf("\n");
    }
}

/**
 * Initialise une structure Station avec une adresse IP et une adresse MAC.
 * @param station Pointeur vers la structure Station à initialiser.
 * @param adrIP Adresse IP à affecter.
 * @param adrMAC Adresse MAC à affecter.
 */
void init_station(Station *station, adresseIP adrIP, adresseMAC adrMAC)
{
    station->id = 0;
    station->adrIP = adrIP;
    station->adrMac = adrMAC;
}

/**
 * Initialise une structure Switch avec un nombre de ports, une priorité et une adresse MAC.
 * Alloue la mémoire pour la table de commutation.
 * @param r_switch Pointeur vers la structure Switch à initialiser.
 * @param nbPort Nombre de ports du switch.
 * @param numPriorite Priorité STP du switch.
 * @param adrMac Adresse MAC du switch.
 */
void init_switch(Switch *r_switch, size_t nbPort, size_t numPriorite, adresseMAC adrMac)
{
    r_switch->id = 0;
    r_switch->adrMac = adrMac;
    r_switch->nb_ports = nbPort;
    r_switch->priorite = numPriorite;
    r_switch->tab_commutation = (port *)malloc(nbPort * sizeof(port));

    for (size_t i = 0; i < nbPort; i++)
    {
        init_port(&(r_switch->tab_commutation)[i]);
    }
}

/**
 * Libère la mémoire allouée pour un switch et ses ports.
 * @param r_switch Pointeur vers la structure Switch à désallouer.
 */
void deinit_switch(Switch *r_switch)
{
    r_switch->nb_ports = 0;
    r_switch->priorite = 0;
    r_switch->adrMac.entier = 0;
    r_switch->id = 0;

    r_switch->adrMac.bytes[0] = 0;
    r_switch->adrMac.bytes[1] = 0;
    r_switch->adrMac.bytes[2] = 0;
    r_switch->adrMac.bytes[3] = 0;
    r_switch->adrMac.bytes[4] = 0;
    r_switch->adrMac.bytes[5] = 0;

    for (size_t i = 0; i < r_switch->nb_ports; i++)
    {
        deinit_port(&(r_switch->tab_commutation)[i]);
    }

    free(r_switch->tab_commutation);
    r_switch = NULL;
}

/**
 * Initialise un port réseau (tableau d'adresses MAC).
 * @param r_port Pointeur vers la structure port à initialiser.
 */
void init_port(port *r_port)
{
    r_port->nb_adressesMAC = 0;
    r_port->adresses_capacite = TAILLE_ADRESSE_MAX;
    r_port->tab_adresseMAC = (adresseMAC *)malloc(TAILLE_ADRESSE_MAX * sizeof(adresseMAC));
}

/**
 * Libère la mémoire allouée pour un port réseau.
 * @param r_port Pointeur vers la structure port à désallouer.
 */
void deinit_port(port *r_port)
{
    r_port->nb_adressesMAC = 0;
    r_port->adresses_capacite = 0;
    free(r_port->tab_adresseMAC);
    r_port->tab_adresseMAC = NULL;
}

/**
 * Ajoute une adresse MAC à la table d'un port.
 * @param port Pointeur vers le port.
 * @param adrMAC Adresse MAC à ajouter.
 */
void ajouter_adresse_port(port *port, adresseMAC adrMAC)
{
    if (port->nb_adressesMAC >= port->adresses_capacite)
    {
        fprintf(stderr, "Erreur: capacité maximale d'adresses MAC atteinte pour ce port.\n");
        exit(EXIT_FAILURE);
    }

    port->tab_adresseMAC[port->nb_adressesMAC] = adrMAC;
    port->nb_adressesMAC++;
}

/**
 * Compare deux ports pour vérifier s'ils sont identiques (mêmes adresses MAC).
 * @param p1 Premier port.
 * @param p2 Deuxième port.
 * @return true si les ports sont identiques, false sinon.
 */
bool equals_port(port p1, port p2)
{
    bool first_step = p1.nb_adressesMAC == p2.nb_adressesMAC;

    if (first_step)
    {
        for (size_t i = 0; i < p1.nb_adressesMAC; i++)
        {
            if (p1.tab_adresseMAC[i].entier != p2.tab_adresseMAC[i].entier)
            {
                return false;
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * Compare deux stations pour vérifier si elles sont identiques (même IP et même MAC).
 * @param s1 Première station.
 * @param s2 Deuxième station.
 * @return true si les stations sont identiques, false sinon.
 */
bool equals_station(Station s1, Station s2)
{
    return s1.adrIP.entier == s2.adrIP.entier && s1.adrMac.entier == s2.adrMac.entier;
}

/**
 * Compare deux switchs pour vérifier s'ils sont identiques (même MAC, même nombre de ports, même priorité, mêmes ports).
 * @param s1 Premier switch.
 * @param s2 Deuxième switch.
 * @return true si les switchs sont identiques, false sinon.
 */
bool equals_switch(Switch s1, Switch s2)
{
    bool first_step = s1.adrMac.entier == s2.adrMac.entier && s1.nb_ports == s2.nb_ports && s1.priorite == s2.priorite;

    if (first_step)
    {
        for (size_t i = 0; i < s1.nb_ports; i++)
        {
            if (!equals_port(s1.tab_commutation[i], s2.tab_commutation[i]))
            {
                return false;
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}