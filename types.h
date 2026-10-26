#ifndef TYPE_H
#define TYPE_H

#define TVA 20
#define Frais-de-port 5,90
#define Frais-de-port-offert 60
#define remise_0  0
#define remise_5 100
#define remise_10 300
#define remise_15 600
#define EN_COURS "EN COURS"
#define EXPEDIEE "EXPEDIEE"
#define LIVREE "LIVREE"
#define ANNULEE "ANNULEE"

typedef { int Jour ; int Mois ; int Annee}Date;
typedef { char[80] rue ; char[50] ville ; char[6] codePostal ; char[50] pays}Adresse;
typedef { char[12] reference  ; char[60] designation ; char[30] categorie ; float prixUnitaireHT ; int quantiteStock ; int seuilAlerte ; int actif }Produit;
typedef { char[12] refProduit ; char[60] designation ;  int quantite ; float prixUnitaireHT ; float sousTotal}LigneCommande;

