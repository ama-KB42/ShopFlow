#ifndef PRODUIT_H
#define PRODUIT_H

#include "types.h"

Produit *trouverProduit(Boutique *b, const char *reference);

void ajouterProduit(Boutique *b);
void modifierProduit(Boutique *b);
void supprimerProduit(Boutique *b);
void afficherProduit(Produit p);
void afficherCatalogue(Boutique *b);
void rechercherProduit(Boutique *b);
void rechercheMultiCriteres(Boutique *b);

#endif
