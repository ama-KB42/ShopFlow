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
	b->capaciteProduits = 8; 
	b->produits = (Produit *)malloc(b->capaciteProduits * sizeof(Produit)); 

	if(b->produits == NULL) {
		fprintf(stderr, "Erreur d'allocation\n"); 
		return; 
	} 
	
	b->nbProduits = 0; 
	Produit modele[] = { 
		{"PRD-0001",	"Console switch", "Informatique",	499.90,	42,	10,	1}, 
		{"PRD-0002",	"Console 3DS",	  "Informatique",	340.90,	 8,	 5,	1}, 
		{"PRD-0003",	"Manettes",			  "Informatique",	 24.90,	 6,	15,	1}, 
		{"PRD-0004",	"Zelda", 			    "Jeu", 		 	     69.90, 22,	15,	1}, 
		{"PRD-0005", 	"Mario Kart 8",	 	"Jeu",           59.90,	69,	10,	1}, 
		{"PRD-0006",	"Pochette switch","Accessoires", 	 25.00,  8, 10,	1},
		{"PRD-0007", 	"Cable HDMI", 		"Accessoires", 	  9.90,	64,	30,	1}, 
		{"PRD-0008",	"Chargeur", 		  "Accessoires",	 19.90,	15,	10,	1}, 
	}; 

	for (int i = 0; i < 8; i++) { 
		b->produits[b->nbProduits++] = modele[i]; 
	} 
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
