#include <stdio.h>
#include "produit.h"
#include "types.h"
#include <string.h>

void ajouterProduit(Boutique *b, Produit p) {
    for (int i = 0; i < b->nbProduits; i++) {
        if (b->produits[i].reference == p.reference) {
            printf("Erreur : un produit avec la meme reference existe deja.\n");
            return;
        }
    }
    if (p.quantiteStock < 0) {
        printf("Erreur : la quantite en stock ne peut pas etre negative.\n");
        return;
    }
    if (p.prixUnitaireHT < 0) {
        printf("Erreur : le prix unitaire ne peut pas etre negatif.\n");
        return;
    }
    if (p.designation[0] == '\0') {
        printf("Erreur : la designation ne peut pas être vide.\n");
        return;
    }
    if (p.categorie[0] == '\0') {
        printf("Erreur : la categorie ne peut pas etre vide.\n");
        return;
    }
    if (b->produits == b->capaciteProduits){
        b ->capaciteProduits *= 2;
        b->produits = realloc(b->produits, b->capaciteProduits * sizeof(Produit));
        if (b->produits == NULL) {
            printf("Erreur : impossible d'allouer de la memoire pour les produits.\n");
            exit(1);
        }
    }
    printf("Produit ajoute.\n");
}

void modifierProduit(Boutique *b, Produit p) {
    for (int i = 0; i < b->nbProduits; i++) {
        if (b->produits[i].reference == p.reference) {
            b->produits[i] = p;
            printf("Produit modifie.\n");
            return;
        } 
    }
    printf("Erreur : produit non trouve.\n");
}

void supprimerProduit(Boutique *b, Produit p){
    for (int i = 0; i < b->nbProduits; i++) {
        if (b->produits[i].reference == p.reference) {
            for (int j = i; j < b->nbProduits - 1; j++) {
                b->produits[j] = b->produits[j + 1];
            }
            b->nbProduits--;
            printf("Produit supprime.\n");
            return;
        }
        printf("Erreur : produit non trouve.\n");
    }
}

void afficherProduit(Produit p) {
    printf("Reference : %s\n", p.reference);
    printf("Designation : %s\n", p.designation);
    printf("Categorie : %s\n", p.categorie);
    printf("Prix unitaire HT : %.2f\n", p.prixUnitaireHT);
    printf("Quantite en stock : %d\n", p.quantiteStock);
    printf("Seuil d'alerte : %d\n", p.seuilAlerte);
    printf("Actif : %d\n", p.actif);
}

void rechercherProduit(Boutique *b, Produit p) {
    int modeRecherche;
    printf("Choisissez le mode de recherche :\n");
    printf("1. Par reference\n");
    printf("2. Par mots cles\n");
    scanf("%d", &modeRecherche);
    if (modeRecherche == 1) {
        for (int i = 0; i < b->nbProduits; i++) {
            if (b->produits[i].reference == p.reference) {
                afficherProduit(b->produits[i]);
                return;
            }
        }
    } else if (modeRecherche == 2) {
        char motsCles[100];
        printf("Entrez les mots clés de recherche : ");
        scanf("%s", motsCles);
        for (int i = 0; i < b->nbProduits; i++) {
            return;
        } 
    } else { 
        printf("Erreur : mode de recherche invalide.\n");
        return;
    } 
} 
