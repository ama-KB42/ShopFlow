#ifndef COMMANDE_H
#define COMMANDE_H
#include "types.h"

/* Fonctions de base */
void initialiserLigneCommande(Commande *commande);
int ajouterLigneCommande(Commande *commande, LigneCommande ligne);
void libererLignesCommande(Commande *commande);
void calculCommande(Commande *c);
Commande *rechercherCommande(Boutique *b, int numero);
void afficherDetailCommande(Commande *c);

/* Fonctionnalites du menu */
void creerCommande(Boutique *b);
void ajouterLigne(Boutique *b);
void supprimerLigne(Boutique *b);
void changerStatut(Boutique *b);
void annulerCommande(Boutique *b);
void afficherCommande(Boutique *b);

#endif
