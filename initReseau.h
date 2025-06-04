#ifndef INIT_RESEAU_H
#define INIT_RESEAU_H

#include "reseau.h"
#include "graphe.h"

// Définitions de tailles pour la lecture des fichiers
#define TAILLE_ADR_MAC_STRING 18
#define TAILLE_ADR_IP_STRING 16
#define TAILLE_MAX_LIGNE 256

// Prototypes des fonctions pour initialiser le graphe à partir d'un fichier texte
void recuperer_nbEquipement_nbLien(const char *ligne, size_t *nb_equipement, size_t *nb_lien);
void init_adrIP_from_text(adresseIP *adrIP, const char *ip_str);
void init_adrMAC_from_text(adresseMAC *adrMAC, const char *mac_str);
void init_switch_from_text(Switch *s_switch, char *switch_str);
void init_station_from_text(Station *station, char *station_str);
void init_arete_from_text(graphe *g, arete *a, char *arete_str);
void init_graph_from_file(graphe *g, const char *path);

#endif