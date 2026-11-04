/* produit.c - catalogue des produits : ajout, modification, suppression logique,
 * affichage, recherche et gestion du stock */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "produit.h"
#include "utils.h"

/* Recherche un produit par sa reference exacte (actif ou non).
 * Retourne NULL si la reference n'existe pas. */
Produit *trouverProduit(Boutique *b, const char *reference) {
    for (int i = 0; i < b->nbProduits; i++) {
        if (strcmp(b->produits[i].reference, reference) == 0) {
            return &b->produits[i];
        }
    }
    return NULL;
}

/* Retourne 1 si le produit apparait dans une commande non annulee */
static int produitUtilise(Boutique *b, const char *reference) {
    for (int i = 0; i < b->nbClients; i++) {
        for (int j = 0; j < b->clients[i].nbCommandes; j++) {
            Commande *c = &b->clients[i].commandes[j];
            if (strcmp(c->statut, ANNULEE) != 0) {
                for (int k = 0; k < c->nbLignes; k++) {
                    if (strcmp(c->lignes[k].refProduit, reference) == 0) {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

/* Met une lettre majuscule en minuscule (pour une recherche sans casse) */
static char minuscule(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + ('a' - 'A');
    }
    return c;
}

/* Retourne 1 si 'motCle' apparait dans 'texte' (sans tenir compte des majuscules) */
static int contientMotCle(const char *texte, const char *motCle) {
    /* On essaie chaque position de depart dans le texte */
    for (int i = 0; texte[i] != '\0'; i++) {
        int j = 0;
        while (motCle[j] != '\0' && minuscule(texte[i + j]) == minuscule(motCle[j])) {
            j++;
        }
        if (motCle[j] == '\0') {
            return 1;   /* tout le mot cle a ete retrouve */
        }
    }
    return 0;
}

void ajouterProduit(Boutique *b) {
    Produit p;
    printf("\n===== AJOUTER UN PRODUIT ====\n");
    lireChaineNonVide("REFERENCE : ", p.reference, sizeof(p.reference));
    /* Unicite : on cherche parmi TOUS les produits, y compris desactives */
    if (trouverProduit(b, p.reference) != NULL) {
        printf("Erreur : un produit avec la meme reference existe deja.\n");
        return;
    }
    lireChaineNonVide("DESIGNATION : ", p.designation, sizeof(p.designation));
    lireChaineNonVide("CATEGORIE : ", p.categorie, sizeof(p.categorie));
    while (lireReel("PRIX UNITAIRE HT : ", &p.prixUnitaireHT) == 0 || p.prixUnitaireHT <= 0) {
        printf("Erreur : le prix doit etre strictement positif.\n");
    }
    while (lireEntier("QUANTITE EN STOCK : ", &p.quantiteStock) == 0 || p.quantiteStock < 0) {
        printf("Erreur : la quantite en stock ne peut pas etre negative.\n");
    }
    while (lireEntier("SEUIL D'ALERTE : ", &p.seuilAlerte) == 0 || p.seuilAlerte < 0) {
        printf("Erreur : le seuil d'alerte ne peut pas etre negatif.\n");
    }
    p.actif = 1;

    /* Agrandissement du catalogue si plein */
    if (b->nbProduits == b->capaciteProduits) {
        int nouvelleCapacite;
        if (b->capaciteProduits == 0) {
            nouvelleCapacite = 2;
        } else {
            nouvelleCapacite = b->capaciteProduits * 2;
        }
        Produit *temp = realloc(b->produits, nouvelleCapacite * sizeof(Produit));
        if (temp == NULL) {
            printf("Erreur : impossible d'allouer de la memoire pour les produits.\n");
            return;
        }
        b->produits = temp;
        b->capaciteProduits = nouvelleCapacite;
    }
    b->produits[b->nbProduits] = p;
    b->nbProduits++;
    printf("Produit ajoute.\n");
}

/* Modification du prix, du stock et du seuil d'alerte */
void modifierProduit(Boutique *b) {
    char reference[12];
    printf("\n===== MODIFIER UN PRODUIT ====\n");
    lireChaineNonVide("REFERENCE : ", reference, sizeof(reference));
    Produit *p = trouverProduit(b, reference);
    if (p == NULL || p->actif == 0) {
        printf("Erreur : produit non trouve.\n");
        return;
    }
    afficherProduit(*p);
    printf("\n");
    while (lireReel("NOUVEAU PRIX UNITAIRE HT : ", &p->prixUnitaireHT) == 0 || p->prixUnitaireHT <= 0) {
        printf("Erreur : le prix doit etre strictement positif.\n");
    }
    while (lireEntier("NOUVELLE QUANTITE EN STOCK : ", &p->quantiteStock) == 0 || p->quantiteStock < 0) {
        printf("Erreur : la quantite en stock ne peut pas etre negative.\n");
    }
    while (lireEntier("NOUVEAU SEUIL D'ALERTE : ", &p->seuilAlerte) == 0 || p->seuilAlerte < 0) {
        printf("Erreur : le seuil d'alerte ne peut pas etre negatif.\n");
    }
    printf("Produit modifie.\n");
}

/* Suppression logique : le produit reste en memoire avec actif = 0 */
void supprimerProduit(Boutique *b) {
    char reference[12];
    printf("\n===== SUPPRIMER UN PRODUIT ====\n");
    lireChaineNonVide("REFERENCE : ", reference, sizeof(reference));
    Produit *p = trouverProduit(b, reference);
    if (p == NULL || p->actif == 0) {
        printf("Erreur : produit non trouve.\n");
        return;
    }
    if (produitUtilise(b, reference) == 1) {
        printf("Suppression impossible : le produit est utilise dans une commande non annulee.\n");
        return;
    }
    if (lireOuiNon("Etes-vous sur de vouloir supprimer ce produit ? (y/n) : ") == 1) {
        p->actif = 0;
        printf("Produit supprime.\n");
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

/* Affiche une ligne du tableau du catalogue */
static void afficherLigneProduit(Produit p) {
    printf("%-10s %-20s %-14s %9.2f %6d %6d %11.2f\n", p.reference, p.designation, p.categorie,
           p.prixUnitaireHT, p.quantiteStock, p.seuilAlerte, p.prixUnitaireHT * p.quantiteStock);
}

static void afficherEnteteCatalogue(void) {
    printf("%-10s %-20s %-14s %9s %6s %6s %11s\n", "REF", "DESIGNATION", "CATEGORIE", "PRIX HT", "STOCK", "SEUIL", "VALEUR");
    printf("-------------------------------------------------------------------------------------\n");
}

/* Vue complete du catalogue avec valorisation du stock (prix HT x quantite) */
void afficherCatalogue(Boutique *b) {
    float valeurTotale = 0.0f;
    printf("\n==== CATALOGUE DES PRODUITS ====\n");
    afficherEnteteCatalogue();
    for (int i = 0; i < b->nbProduits; i++) {
        if (b->produits[i].actif == 1) {
            afficherLigneProduit(b->produits[i]);
            valeurTotale = valeurTotale + b->produits[i].prixUnitaireHT * b->produits[i].quantiteStock;
        }
    }
    printf("-------------------------------------------------------------------------------------\n");
    printf("Valorisation totale du stock : %.2f euros HT\n", valeurTotale);
}

void rechercherProduit(Boutique *b) {
    int modeRecherche;
    printf("Choisissez le mode de recherche :\n");
    printf("1. Par reference\n");
    printf("2. Par mot cle\n");
    if (lireEntier("Votre choix : ", &modeRecherche) == 0) {
        modeRecherche = 0;
    }
    if (modeRecherche == 1) {
        char reference[12];
        lireChaineNonVide("Reference : ", reference, sizeof(reference));
        Produit *p = trouverProduit(b, reference);
        if (p == NULL || p->actif == 0) {
            printf("Aucun produit avec cette reference.\n");
            return;
        }
        afficherProduit(*p);
    } else if (modeRecherche == 2) {
        char motCle[60];
        int nbTrouves = 0;
        lireChaineNonVide("Entrez le mot cle de recherche : ", motCle, sizeof(motCle));
        for (int i = 0; i < b->nbProduits; i++) {
            if (b->produits[i].actif == 1 && contientMotCle(b->produits[i].designation, motCle) == 1) {
                if (nbTrouves == 0) {
                    afficherEnteteCatalogue();
                }
                afficherLigneProduit(b->produits[i]);
                nbTrouves++;
            }
        }
        printf("%d produit(s) trouve(s).\n", nbTrouves);
    } else {
        printf("Erreur : mode de recherche invalide.\n");
    }
}

/* CREATIVITE 1 : recherche multi-criteres (categorie, prix maximum, disponibilite) */
void rechercheMultiCriteres(Boutique *b) {
    char categorie[30];
    float prixMax;
    int enStockSeulement;
    int nbTrouves = 0;
    printf("\n==== RECHERCHE MULTI-CRITERES ====\n");
    lireChaine("Categorie (vide = toutes) : ", categorie, sizeof(categorie));
    while (lireReel("Prix HT maximum (0 = pas de limite) : ", &prixMax) == 0 || prixMax < 0) {
        printf("Erreur : prix invalide.\n");
    }
    enStockSeulement = lireOuiNon("Seulement les produits en stock ? (y/n) : ");

    for (int i = 0; i < b->nbProduits; i++) {
        Produit *p = &b->produits[i];
        int correspond = (p->actif == 1);
        if (categorie[0] != '\0' && contientMotCle(p->categorie, categorie) == 0) correspond = 0;
        if (prixMax > 0 && p->prixUnitaireHT > prixMax) correspond = 0;
        if (enStockSeulement == 1 && p->quantiteStock == 0) correspond = 0;
        if (correspond == 1) {
            if (nbTrouves == 0) {
                afficherEnteteCatalogue();
            }
            afficherLigneProduit(*p);
            nbTrouves++;
        }
    }
    printf("%d produit(s) trouve(s).\n", nbTrouves);
}
