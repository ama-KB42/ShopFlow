#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "initialisation.h" 

void initialiserBoutique(Boutique *b) {
    strcpy(b->nom, "ShopFlow"); 
	b->produits = 0; 
	b->nbProduits = 0; 
	b->capaciteProduits = 0; 
	b->clients = 0; 
	b->nbClients = 0; 
	b->capaciteClients = 0; 
	b->numeroSuivant = 1001; 
	b->idClientSuivant = 1; 
} 

void initialiserDonneesTest(Boutique *b) { 
	/*1. Catalogue boutique : allocation dynamique de 8 emplacements*/
	b->capaciteProduits = 8; 
	b->produits = (Produit *)malloc(b->capaciteProduits * sizeof(Produit)); 
	if(b->produits == NULL) {
		fprintf(stderr, "Erreur d'allocation (Boutique_produits)\n"); 
		return; 
	} 
	b->nbProduits = 0; 
	Produit modeleProduits[] = { 
		{"PRD-0001",	"Console switch", 	"Informatique",	499.90,	42,	10,	1}, 
		{"PRD-0002",	"Console 3DS",		"Informatique",	340.90,	 8,	 5,	1}, 
		{"PRD-0003",	"Manettes",			"Informatique",	 24.90,	 6,	15,	1}, 
		{"PRD-0004",	"Zelda", 			"Jeu", 		 	 69.90, 22,	15,	1}, 
		{"PRD-0005", 	"Mario Kart 8",	 	"Jeu",			 59.90,	69,	10,	1}, 
		{"PRD-0006",	"Pochette switch",	"Accessoires", 	 25.00,  8, 10,	1},
		{"PRD-0007", 	"Cable HDMI", 		"Accessoires", 	  9.90,	64,	30,	1}, 
		{"PRD-0008",	"Chargeur", 		"Accessoires",	 19.90,	15,	10,	1}, 
	}; 
	for (int i=0; i<8; i++) { 
		b->produits[b->nbProduits++] = modeleProduits[i]; 
	} 

	/*2. Clients : allocation dynamique de 5 emplacements*/
	b->capaciteClients = 5; 
	b->clients = (Client *)malloc(b->capaciteClients * sizeof(Client)); 
	if (b->clients == NULL) {
		fprintf(stderr, "Erreur d'allocation (Boutique_clients)"); 
		return; 
	} 
	b->nbClients = 0; 
	Client modeleClients[] = {
		{1001,	"HARROUSSI",	"Skandine",		"skandineh@gmail.com",	"0667676767",	{"2 rue Montcuq", "Montcuq", "46800", "France"}, 							0, 0, 0, NULL, 1},
		{1002,	"ARAKANI",		"Mahmudette",	"mahmudette@gmail.com",	"0666666666",	{"69 rue des Bois de Boulogne", "Boulogne-Billancourt", "92100", "France"},	0, 0, 0, NULL, 1},
		{1003,	"LE METAYER",	"Alexis",		"alexisl@gmail.com",	"0612345678",	{"42 quai des Petits Pets", "Brest", "29200", "Bretagne"}, 					0, 0, 0, NULL, 1}, 
		{1004,	"MAUREY",		"Aurelia",		"aureliam@gmail.com",	"0669696969",	{"493 cité Pigalle", "Paris", "75009", "France"}, 							0, 0, 0, NULL, 1}, 
		{1005,	"D'ALEXIS",		"Anayo",		"anayod@gmail.com",		"0611223344",	{"66 rue de la Basse-Terre", "Basse-Terre", "97100", "Guadeloupe"},			0, 0, 0, NULL, 1}, 
	};
	for (int i=0; i=b->nbClients; i++) {
		b->clients[b->nbClients] = modeleClients[i]; 
	} 
	b->idClientSuivant = 6; 

	/*3. Commande 01 : allocation dynamique de 6 emplacements*/
	Commande c1; 
		c1.numero = 1001; 
		c1.idClient = 1; 
		c1.dateCommande = (Date){31/10/2026}; 
		strcpy(c1.statut, "LIVREE"); 
		c1.capaciteLignes = 6;
		c1.lignes = (LigneCommande *)malloc(c1.capaciteLignes * sizeof(LigneCommande)); 
		c1.nbLignes = 0; 
		/*A suivre*/
} 

void detruireBoutique(Boutique *b) {
	for (int i = 0; i < b->nbClients; i++) {
		for (int j = 0; j < b->clients[i].nbCommandes; j++) {
			free(b->clients[i].commandes[j].lignes); 
		} 
		free(b->clients[i].commandes);
	} 
	free(b->clients); 
	free(b->produits); 
} 
