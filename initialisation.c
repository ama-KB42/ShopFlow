#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "initialisation.h"
#include "client.h" 
#include "produit.h"
#include "commande.h" 

/* Modele simplifie d'une ligne et d'une commande de test */
typedef struct { char ref[12]; int qte; } LigneModele;
typedef struct { int idClient; Date date; char statut[20]; LigneModele lignes[5]; int nbLignes; } CommandeModele;

/* Nombre d'elements d'un tableau statique */
#define NB_ELEMENTS(tableau) ((int)(sizeof(tableau) / sizeof(tableau[0])))

static int creerCommandeTest(Boutique *b, const CommandeModele *m);

void initialiserBoutique(Boutique *b) {
    strcpy(b->nom, "ShopFlow"); 
	b->produits = NULL; 
	b->nbProduits = 0; 
	b->capaciteProduits = 0; 
	b->clients = NULL; 
	b->nbClients = 0; 
	b->capaciteClients = 0; 
	b->numeroSuivant = 1001; 
	b->idClientSuivant = 1; 
} 

void initialiserDonneesTest(Boutique *b) {
	/*1. Catalogue boutique : allocation dynamique de 10 emplacements (8 utilises)*/
	b->capaciteProduits = 10; 
	b->produits = (Produit *)malloc(b->capaciteProduits * sizeof(Produit)); 
	if(b->produits == NULL) {
		fprintf(stderr, "Erreur d'allocation (Boutique_produits)\n"); 
		b->capaciteProduits = 0; 
		return; 
	} 
	b->nbProduits = 0; 
	Produit modeleProduits[] = { 
		{"PRD-0001",	"Console switch", 	"Informatique",	499.90,	42,	10,	1}, 
		{"PRD-0002",	"Console 3DS",		"Informatique",	340.90,	 8,	 5,	1}, 
		{"PRD-0003",	"Manettes",			"Informatique",	 24.90,	 6,	15,	1},		/* en alerte */
		{"PRD-0004",	"Zelda", 			"Jeu", 		 	 69.90, 22,	15,	1}, 
		{"PRD-0005", 	"Mario Kart 8",	 	"Jeu",			 59.90,	69,	10,	1}, 
		{"PRD-0006",	"Pochette switch",	"Accessoires", 	 25.00,  8, 10,	1},		/* en alerte */
		{"PRD-0007", 	"Cable HDMI", 		"Accessoires", 	  9.90,	64,	30,	1}, 
		{"PRD-0008",	"Chargeur", 		"Accessoires",	 19.90,	15,	10,	1}, 
	}; 
	for (int i=0; i<8; i++) { 
		b->produits[b->nbProduits++] = modeleProduits[i]; 
	} 

	/*2. Clients : allocation dynamique de 10 emplacements (5 utilises)*/
	b->capaciteClients = 10; 
	b->clients = (Client *)malloc(b->capaciteClients * sizeof(Client)); 
	if (b->clients == NULL) {
		fprintf(stderr, "Erreur d'allocation (Boutique_clients)\n"); 
		b->capaciteClients = 0; 
		return; 
	} 
	b->nbClients = 0; 
	Client modeleClients[] = { 
		{1,	"HARROUSSI",	"Skandine",		"skandineh@gmail.com",	"0667676767",	{"2 rue Montcuq", "Montcuq", "46800", "France"}, 							0, 0, 0, NULL, 1},
		{2,	"ARAKANI",		"Mahmudette",	"mahmudette@gmail.com",	"0666666666",	{"69 rue des Bois de Boulogne", "Boulogne-Billancourt", "92100", "France"},	0, 0, 0, NULL, 1},
		{3,	"LE METAYER",	"Alexis",		"alexisl@gmail.com",	"0612345678",	{"42 quai des Petits Pets", "Brest", "29200", "Bretagne"}, 					0, 0, 0, NULL, 1}, 
		{4,	"MAUREY",		"Aurelia",		"aureliam@gmail.com",	"0669696969",	{"493 cite Pigalle", "Paris", "75009", "France"}, 							0, 0, 0, NULL, 1}, 
		{5,	"D'ALEXIS",		"Anayo",		"anayod@gmail.com",		"0611223344",	{"67 rue de la Basse-Terre", "Basse-Terre", "97100", "Guadeloupe"},			0, 0, 0, NULL, 1},
	};
	for (int i=0; i<5; i++) {
		b->clients[b->nbClients++] = modeleClients[i]; 
	} 
	b->idClientSuivant = 6; 

	/*3. Commandes : 6 commandes, lignes allouees dynamiquement dans creerCommandeTest*/ 
    CommandeModele modeleCommandes[] = {
        /* client, date,           statut,   lignes (ref, quantite),                          nb */
        {1, {12,  9, 2026}, LIVREE,   {{"PRD-0001", 1}, {"PRD-0004", 2}, {"PRD-0007", 2}}, 3}, /* 659,50 HT : remise 15 % */
        {2, {25,  9, 2026}, LIVREE,   {{"PRD-0005", 2}, {"PRD-0008", 1}, {"PRD-0006", 1}}, 3}, /* 164,70 HT : remise 5 % */
        {3, { 8, 10, 2026}, EXPEDIEE, {{"PRD-0002", 1}, {"PRD-0003", 2}},                  2}, /* 390,70 HT : remise 10 % */
        {1, {20, 10, 2026}, EN_COURS, {{"PRD-0007", 2}, {"PRD-0008", 1}},                  2}, /*  39,70 HT : frais de port */
        {4, {15, 10, 2026}, ANNULEE,  {{"PRD-0004", 1}, {"PRD-0003", 1}},                  2}, /* annulee : stock rendu */
        {5, {29, 10, 2026}, EN_COURS, {{"PRD-0001", 1}, {"PRD-0004", 1}, {"PRD-0005", 1}}, 3}  /* 629,70 HT : remise 15 % */
    };
    for (int i = 0; i < NB_ELEMENTS(modeleCommandes); i++) {
        if (creerCommandeTest(b, &modeleCommandes[i]) != 0) {
            fprintf(stderr, "Echec de la creation de la commande de test %d\n", i + 1);
            detruireBoutique(b);
            return;
        } 
    }
}

void detruireBoutique(Boutique *b) {
	for (int i = 0; i < b->nbClients; i++) {
		for (int j = 0; j < b->clients[i].nbCommandes; j++)
			free(b->clients[i].commandes[j].lignes);
		free(b->clients[i].commandes);
	}
	free(b->clients);
	free(b->produits);
	b->clients = NULL;
	b->produits = NULL;
	b->nbClients = 0;
	b->nbProduits = 0;
	b->capaciteClients = 0;
	b->capaciteProduits = 0;
}


/* Cree une commande de test a partir d'un modele et la rattache a son client.
 * Retourne 0 si tout va bien, -1 en cas d'erreur. */
static int creerCommandeTest(Boutique *b, const CommandeModele *m) {
    Client *cl = rechercherClient(b, m->idClient);
    if (cl == NULL) { 
        fprintf(stderr, "Erreur : client %d introuvable\n", m->idClient);
        return -1; 
    }

    Commande c;
    c.numero = b->numeroSuivant++; 
    c.idClient = m->idClient;
    c.dateCommande = m->date; 
    strcpy(c.statut, m->statut);
    c.nbLignes = 0;
    c.capaciteLignes = m->nbLignes;
    c.adresseLivraison = cl->adresse; 
    c.lignes = (LigneCommande *)malloc(c.capaciteLignes * sizeof(LigneCommande));
    if (c.lignes == NULL) {
        fprintf(stderr, "Erreur d'allocation (lignes de commande)\n");
        return -1;
    } 
    for (int i = 0; i < m->nbLignes; i++) {
        Produit *p = trouverProduit(b, m->lignes[i].ref);
        if (p == NULL || m->lignes[i].qte < 1 || m->lignes[i].qte > p->quantiteStock) {
            fprintf(stderr, "Erreur : ligne invalide (%s) dans la commande %d\n", m->lignes[i].ref, c.numero);
            free(c.lignes);
            return -1;
        }
        LigneCommande *l = &c.lignes[c.nbLignes++];
        strcpy(l->refProduit, p->reference);
        strcpy(l->designation, p->designation);
        l->quantite = m->lignes[i].qte;
        l->prixUnitaireHT = p->prixUnitaireHT;
        l->sousTotalHT = l->quantite * l->prixUnitaireHT;
        p->quantiteStock -= l->quantite; 
    } 
    calculCommande(&c);
    if (strcmp(c.statut, ANNULEE) == 0) {
        for (int i = 0; i < c.nbLignes; i++) { 
            Produit *p = trouverProduit(b, c.lignes[i].refProduit);
            if (p != NULL) p->quantiteStock += c.lignes[i].quantite;
        } 
    } else {
        cl->pointsFidelite += POINTS_FIDELITE(c.montantTTC);
    } 
    if (cl->nbCommandes == cl->capaciteCommandes) { /* realloc sur NULL = malloc */
        int nouvelleCap = (cl->capaciteCommandes == 0) ? 2 : cl->capaciteCommandes * 2;
        Commande *tmp = (Commande *)realloc(cl->commandes, nouvelleCap * sizeof(Commande));
        if (tmp == NULL) {
            fprintf(stderr, "Erreur de reallocation (commandes du client %d)\n", m->idClient);
            free(c.lignes);
            return -1;
        }
        cl->commandes = tmp;
        cl->capaciteCommandes = nouvelleCap;
    }
    cl->commandes[cl->nbCommandes++] = c;           /* le client possede desormais c.lignes */
    return 0;
} 
