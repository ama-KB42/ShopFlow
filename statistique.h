#ifndef STATISTIQUE_H
#define STATISTIQUE_H
#include "types.h"

void afficherChiffreAffaires(Boutique b);
void afficherChiffreAffairesPeriode(Boutique b, int mois, int annee);
void afficherPalmaresProduits(Boutique b);
void afficherPalmaresClients(Boutique b);
void afficherAlertesStock(Boutique *b);

#endif 
