#ifndef COMMANDE_H
#define COMMANDE_H
#include "types.h"

void creerCommande(Commande *commande);
void initialiserLigneCommande(Commande *commande);
int ajouterLigneCommande(Commande *commande, LigneCommande ligne);
void supprimerLigneCommande(Commande *commande);
void annulerCommande(Commande *commande);
void afficherCommande(Commande *commande);

#endif
