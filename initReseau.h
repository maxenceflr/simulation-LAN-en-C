#ifndef INIT_RESEAU_H
#define INIT_RESEAU_H

#include "reseau.h"
#include "graphe.h"

#define TAILLE_ADR_MAC_STRING 18
#define TAILLE_ADR_IP_STRING 16
#define TAILLE_MAX_LIGNE 256

/**
 * @brief Récupère le nombre d'équipements et de liens à partir d'une ligne de texte.
 * @param ligne Ligne à analyser.
 * @param nb_equipement Pointeur vers le nombre d'équipements.
 * @param nb_lien Pointeur vers le nombre de liens.
 */
void recuperer_nbEquipement_nbLien(const char *ligne, size_t *nb_equipement, size_t *nb_lien);

/**
 * @brief Initialise une adresse IP à partir d'une chaîne.
 * @param adrIP Pointeur vers l'adresse IP à initialiser.
 * @param ip_str Chaîne représentant l'adresse IP.
 */
void init_adrIP_from_text(adresseIP *adrIP, const char *ip_str);

/**
 * @brief Initialise une adresse MAC à partir d'une chaîne.
 * @param adrMAC Pointeur vers l'adresse MAC à initialiser.
 * @param mac_str Chaîne représentant l'adresse MAC.
 */
void init_adrMAC_from_text(adresseMAC *adrMAC, const char *mac_str);

/**
 * @brief Initialise un switch à partir d'une ligne de texte.
 * @param s_switch Pointeur vers le switch à initialiser.
 * @param switch_str Ligne de texte décrivant le switch.
 */
void init_switch_from_text(Switch *s_switch, char *switch_str);

/**
 * @brief Initialise une station à partir d'une ligne de texte.
 * @param station Pointeur vers la station à initialiser.
 * @param station_str Ligne de texte décrivant la station.
 */
void init_station_from_text(Station *station, char *station_str);

/**
 * @brief Initialise une arête à partir d'une ligne de texte.
 * @param g Pointeur vers le graphe.
 * @param a Pointeur vers l'arête à initialiser.
 * @param arete_str Ligne de texte décrivant l'arête.
 */
void init_arete_from_text(graphe *g, arete *a, char *arete_str);

/**
 * @brief Initialise un graphe à partir d'un fichier texte.
 * @param g Pointeur vers le graphe à initialiser.
 * @param path Chemin du fichier à lire.
 */
void init_graph_from_file(graphe *g, const char *path);

#endif