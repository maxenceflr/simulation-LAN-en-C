# IUT Robert Schuman - Université de Strasbourg  
## Département Informatique  
### SAE 2.3 - Réseaux  
### Situation d’apprentissage et d’évaluation

---

## Objectifs

Ce projet a pour objectif de démontrer votre capacité à :

- Manipuler une structure de données représentant un graphe étiqueté.
- Concevoir de nouvelles structures pour modéliser des équipements réseaux (station, switch).
- Utiliser ces structures pour modéliser une architecture de réseau local.
- Comprendre le protocole **Ethernet** et coder l’algorithme de diffusion d’un message.
- Comprendre le protocole **STP** et coder l’algorithme de synchronisation des switchs (BPDU).

> 🧑‍🤝‍🧑 Travail en trinôme. Présentation obligatoire en fin de module.  
> 🎯 Langage utilisé : **C**, basé sur la librairie de graphes vue en M23.

---

## Étape 1 : Structures de Données

### Pour les stations :
- Adresse MAC (Ethernet)
- Adresse IP

### Pour les switchs :
- Adresse MAC (Ethernet)
- Nombre de ports
- Priorité (STP)
- Table de commutation

### Pour le réseau local :
- S'appuyer sur la structure de graphe étiqueté de M23.

> ⚙️ Prévoir des fonctions d'affichage pour :
> - Adresse IP (notation décimale pointée)
> - Adresse MAC (hexadécimal)
> - Table de commutation

---

## Étape 2 : Fichier de Configuration

Un fichier texte décrit le réseau local :

```
<nb_équipements> <nb_liens>
<type>;<mac>;<...>
...
<id1>;<id2>;<poids>
```

### Type d’équipement :
- `1` : Station
- `2` : Switch

### Poids (débit ↔ coût STP) :
| Débit      | Poids |
|------------|-------|
| 10 Mb/s    | 100   |
| 100 Mb/s   | 19    |
| 1 Gb/s     | 4     |

### Exemple :
```
4 3
2;01:45:23:a6:f7:ab;8;1024
1;54:d6:a6:82:c5:23;130.79.80.21
1;c8:69:72:5e:43:af;130.79.80.27
1;77:ac:d6:82:12:23;130.79.80.42
0;1;4
0;2;19
0;3;4
```

---

## Étape 3 : Trames Ethernet

Créer une structure représentant une trame Ethernet, conforme au modèle réel :

| Champ           | Taille    | Détails                        |
|------------------|-----------|-------------------------------|
| Préambule        | 7 octets  | `10101010`                    |
| SFD              | 1 octet   | `10101011`                    |
| Adresse dest     | 6 octets  |                               |
| Adresse source   | 6 octets  |                               |
| Type             | 2 octets  | Ex : `0x0800` (IPv4), etc.    |
| Données          | 0-1500    | Inclut éventuellement padding |
| FCS              | 4 octets  | Vérification (CRC)            |

> 📋 Créez une fonction d’affichage :
> - Mode utilisateur (lisible)
> - Mode hexadécimal (brut)

---

## Étape 4 : Protocole STP

Implémenter **Spanning Tree Protocol** :

- Charger un réseau depuis un fichier de config.
- Tables d’association vides au début.
- Ports à l’état **inconnu**.
- Synchronisation via **échange de BPDU** jusqu’à convergence.

### Objectifs :
- Identifier le port racine.
- Bloquer les ports pour éviter les cycles.
- Ouvrir les ports désignés.

---

## Évaluation

- Qualité du code (noms, organisation, commentaires, `Makefile`, etc.)
- Présentation orale avec démonstration.