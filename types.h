#ifndef TYPE_H
#define TYPE_H

#define TVA 20f
#define Frais-de-port 5,90f
#define Frais-de-port-offert 60f
#define remise_0  0f
#define remise_5 100f
#define remise_10 300f
#define remise_15 600f
#define EN_COURS "EN COURS"
#define EXPEDIEE "EXPEDIEE"
#define LIVREE   "LIVREE"
#define ANNULEE  "ANNULEE"

typedef struct { int Jour ; int Mois ; int Annee} Date;
typedef struct { char[80] rue ; char[50] ville ; char[6] codePostal ; char[50] pays} Adresse;
typedef struct { char[12] reference  ; char[60] designation ; char[30] categorie ; float prixUnitaireHT ; int quantiteStock ; int seuilAlerte ; int actif } Produit;
typedef struct { char[12] refProduit ; char[60] designation ;  int quantite ; float prixUnitaireHT ; float sousTotal} LigneCommande;
typedef struct { int numero ; int idClient ; Date dateCommande ; char[20] statut ; int nbligne ; int capaciteLigns ;  lignes ligneCommande*  ;  float montantBrutHT ; float tauxRemise ; float montantRemise ; float montantHT ; float montantTVA ; float montantTTC ; float frais de port ; Adresse*  adresseLivraison }Commande;
typedef { int idClient ; char[50] nom  ;  char[50] prenom ;  char[80] email ; char[15] telephone ; Adresse* adresse ; int  pointsFidelite ;  int nbCommande ;  int capaciteCommandes ;  commande* commande ;  int actif }CLIENT;
typedef { char [50] nom ;  Produit* produit ;  int nbproduits ;  int capaciteProduits ; CLIENT* client ; int  nbclients ;  int  capaciteClient ;  int numerosuivant ;  int idCLientSuivant }BOUTIQUE;


