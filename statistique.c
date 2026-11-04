#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "statistique.h"
#include "initialisation.h"
#include "client.h"

/* FONCTION 1 : Calculer le chiffre d'affaires total HT et TTC */
void afficherChiffreAffaires(Boutique *b) {
    float totalHT = 0.0;
    float totalTTC = 0.0;
    for (int i = 0; i < b->nbClients; i++) {
        for (int j = 0; j < b->clients[i].nbCommandes; j++) {
            Commande *c = &b->clients[i].commandes[j];
                if (strcmp(c->statut, ANNULEE) != 0) {
                    totalHT = totalHT + c->montantHT;
                    totalTTC = totalTTC + c->montantTTC;
                }
        }
    }
    printf("\n==== CHIFFRE D'AFFAIRES ====\n");
    printf("Total HT  : %.2f euros\n", totalHT);
    printf("Total TTC : %.2f euros\n", totalTTC);
}

/* FONCTION 2 : Calculer le chiffre d'affaires d'un mois et d'une annee */
void afficherChiffreAffairesPeriode(Boutique *b, int mois, int annee) {
    float totalHT = 0.0;
    float totalTTC = 0.0;
    for (int i = 0; i < b->nbClients; i++) {
        for (int j = 0; j < b->clients[i].nbCommandes; j++) {
            Commande *c = &b->clients[i].commandes[j];
            if (strcmp(c->statut, ANNULEE) != 0){
                if (c->dateCommande.annee == annee && (mois == 0 || c->dateCommande.mois == mois)) {
                    totalHT = totalHT + c->montantHT;
                    totalTTC = totalTTC + c->montantTTC;
                }
            }
        }
    }
    printf("\n==== CHIFFRE D'AFFAIRES PAR PERIODE ====\n");
    printf("Mois : %d / Annee : %d\n", mois, annee);
    printf("Total HT  : %.2f euros\n", totalHT);
    printf("Total TTC : %.2f euros\n", totalTTC);
}

/* FONCTION 3 : Afficher les 5 produits les plus vendus */
void afficherPalmaresProduits(Boutique *b) {
    int nb = b->nbProduits;
    if (nb == 0){
        printf("Aucun produit dans le catalogue.\n");
        return; 
    }
    int *quantites = calloc(nb, sizeof(int));
    int *dejaAffiche = calloc(nb, sizeof(int));
    if (quantites == NULL || dejaAffiche == NULL){
        printf("Erreur d'allocation memoire.\n");
        free(quantites);
        free(dejaAffiche);
        return;
    }

    /* Compter les quantites vendues pour chaque produit */
    for (int i = 0; i < b->nbClients; i++){
        for (int j = 0; j < b->clients[i].nbCommandes; j++) {
            Commande *c = &b->clients[i].commandes[j];
            if (strcmp(c->statut, ANNULEE) != 0) {
                for (int k = 0; k < c->nbLignes; k++) {
                    for (int p = 0; p < nb; p++) {
                        if (strcmp(c->lignes[k].refProduit, b->produits[p].reference) == 0) {
                            quantites[p] = quantites[p] + c->lignes[k].quantite;
                        }
                    }
                }
            }
        }
    }
    printf("\n==== TOP 5 DES PRODUITS VENDUS ====\n");
    int limite = nb;
    if (limite > 5) {
        limite = 5;
    }

    /* Chercher le produit le plus vendu restant */
    for (int rang = 0; rang < limite; rang++) {
        int meilleur = -1;
        for (int p = 0; p < nb; p++) {
            if (dejaAffiche[p] == 0) {
                if (meilleur == -1 || quantites[p] > quantites[meilleur]) {
                    meilleur = p;
                    if (meilleur == -1 || quantites[meilleur] == 0) {
                        break;
                    }
                }
            }
        }
        if (meilleur != -1){
            printf("%d. %s : %d vendu(s)\n", rang + 1, b->produits[meilleur].designation, quantites[meilleur]);
            dejaAffiche[meilleur] = 1;
        }
    }
    free(quantites);
    free(dejaAffiche);
}

/* FONCTION 4 : Classer les clients par chiffre d'affaires HT */
void afficherPalmaresClients(Boutique *b) {
    printf("\n===== PALMARES CLIENTS =====\n");
    if (b->nbClients == 0) {
        printf("Aucun client enregistre.\n");
        return;
    }
    float *chiffreAffaires = (float *)calloc(b->nbClients, sizeof(float));
    int *rang = (int *)calloc(b->nbClients ,sizeof(int));
    if (chiffreAffaires == NULL || rang == NULL) {
        printf("Erreur d'allocation (statistique_afficherPalmaresClients)\n");
        free(chiffreAffaires);
        free(rang);
        return;
    }
    /* Calculer le chiffre d'affaires de chaque client */
    for (int i = 0; i < b->nbClients; i++) {
        rang[i] = i;
        for (int j = 0; j < b->clients[i].nbCommandes; j++) {
            Commande *c = &b->clients[i].commandes[j];
            if (strcmp(c->statut, ANNULEE) != 0) {
                chiffreAffaires[i] = chiffreAffaires[i] + c->montantHT;
            }
        }
    }

    /* Trier les clients du plus gros CA au plus petit */
    for (int i = 0; i < b->nbClients - 1; i++) {
        for (int j = i + 1; j < b->nbClients; j++) {
            if (chiffreAffaires[rang[j]] > chiffreAffaires[rang[i]]) {
                int temp = rang[i];
                rang[i] = rang[j];
                rang[j] = temp;
            }
        }
    }
    printf("\n==== PALMARES DES CLIENTS ====\n");
    for (int i = 0; i < b->nbClients; i++) {
        int position = rang[i];
        printf("%d %s %s : %.2f euros HT\n", i + 1, b->clients[position].prenom, b->clients[position].nom, chiffreAffaires[position]);
    }
    free(chiffreAffaires);
    free(rang);
}

/* FONCTION 5 : Afficher les produits en alerte de stock */
void afficherAlertesStock(Boutique *b) {
    int nbAlertes = 0;
    printf("\n==== ALERTES DE STOCK ====\n");
    for (int i = 0; i < b->nbProduits; i++) {
        Produit *p = &b->produits[i];
        if (p->actif == 1 && p->quantiteStock <= p->seuilAlerte) {
            printf("Attention ! %s - %s : stock restant %d (seuil %d)\n", p->reference, p->designation, p->quantiteStock, p->seuilAlerte);
            nbAlertes++;
        }
    }
    if (nbAlertes == 0) {
        printf("C'est bon : aucun produit en alerte :)\n");
    }
}
