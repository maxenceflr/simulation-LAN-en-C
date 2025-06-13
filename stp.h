#ifndef STP_H
#define STP_H

#include "graphe.h"

typedef enum {
    AVANT,
    EGUALE,
    APRES,
} SwitchCompareResult;

void mise_a_jour_ports_stp(graphe *g, int *prev, int switchCount, int *switch_map);

void run_stp(graphe *g);

int assigner_port(Switch *sw);

void afficher_stp_resultats(graphe *g);

#endif
