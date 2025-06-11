#include "reseau.h"

/**

 * @brief Affiche une adresse IP au format standard.
 * @param adresse L'adresse IP à afficher.
 */
void afficherIP(adresseIP adresse)
{
    printf("%u.%u.%u.%u", adresse.bytes[0], adresse.bytes[1], adresse.bytes[2], adresse.bytes[3]);
}

/**

 * @brief Affiche une adresse MAC au format hexadécimal séparé par des ':'.
 * @param adresse L'adresse MAC à afficher.
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

 * @brief Affiche les informations d'un switch réseau.
 * @param sswitch Le switch à afficher.
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

 * @brief Affiche les informations d'une station réseau.
 * @param station La station à afficher.
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

 * @brief Affiche la table de commutation d'un switch.
 * @param r_switch Le switch dont la table de commutation doit être affichée.
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

 * @brief Initialise une station avec une adresse IP et MAC.
 * @param station Pointeur vers la station à initialiser.
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

 * @brief Initialise un switch avec un nombre de ports, une priorité et une adresse MAC.
 * @param r_switch Pointeur vers le switch à initialiser.
 * @param nbPort Nombre de ports.
 * @param numPriorite Priorité du switch.
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

 * @brief Libère la mémoire et réinitialise un switch.
 * @param r_switch Pointeur vers le switch à désinitialiser.
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

 * @brief Initialise un port réseau.
 * @param r_port Pointeur vers le port à initialiser.
 */
void init_port(port *r_port)
{
    r_port->nb_adressesMAC = 0;
    r_port->adresses_capacite = TAILLE_ADRESSE_MAX;
    r_port->tab_adresseMAC = (adresseMAC *)malloc(TAILLE_ADRESSE_MAX * sizeof(adresseMAC));
    r_port->etat=STP_INCONNU; 
}

/**

 * @brief Libère la mémoire d'un port réseau.
 * @param r_port Pointeur vers le port à désinitialiser.
 */
void deinit_port(port *r_port)
{
    r_port->nb_adressesMAC = 0;
    r_port->adresses_capacite = 0;
    free(r_port->tab_adresseMAC);
    r_port->tab_adresseMAC = NULL;
}

/**
 * @brief Ajoute une adresse MAC à un port, si la capacité n'est pas dépassée.
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
 * @brief Compare deux adresses MAC pour vérifier si elles sont identiques.
 * @param adr1 Première adresse MAC.
 * @param adr2 Deuxième adresse MAC.
 * @return true si les adresses MAC sont identiques, false sinon.
 */
bool equals_adresseMAC(adresseMAC adr1, adresseMAC adr2)
{
    return adr1.entier == adr2.entier;
}

/**
 * @brief Compare deux ports pour vérifier s'ils sont identiques.
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