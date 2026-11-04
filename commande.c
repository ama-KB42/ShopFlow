#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "commande.h"

void creerCommande(Commande *commande) {
    printf("\n==============================================\n");
    printf("\n             GESTION DES COMMANDES              \n");
    printf("\n==============================================\n");
    printf("\n");
    printf("\n===== CREER UNE COMMANDE ====\n");

}

void initialiserLigneCommande(Commande *commande) {
    if (commande == NULL) return;
    commande->nbLignes = 0;
    commande->capaciteLignes = 0;
    commande->lignes = NULL;
}

int ajouterLigneCommande(Commande *commande, LigneCommande ligne) {
    if (commande == NULL) return 0;
    if (commande->nbLignes == commande->capaciteLignes) {
        int nouvelleCapacite;
        if (commande->capaciteLignes == 0) {
            nouvelleCapacite = 2;
        } else {
            nouvelleCapacite = commande->capaciteLignes * 2;
        }
        LigneCommande *nouvellesLignes = realloc(commande->lignes, nouvelleCapacite * sizeof(LigneCommande));
        if (nouvellesLignes == NULL) return 0;
        commande->lignes = nouvellesLignes;
        commande->capaciteLignes = nouvelleCapacite;
    }
    commande->lignes[commande->nbLignes] = ligne;
    commande->nbLignes++;
    return 1;
}

void supprimerLigneCommande(Commande *commande) {
    if (commande == NULL) return;
    free(commande->lignes);
    commande->lignes = NULL;
    commande->nbLignes = 0;
    commande->capaciteLignes = 0;
}

void annulerCommande(Commande *commande) {

}

void afficherCommande(Commande *commande) {

}

void calculCommande(Commande *c) {
    float brut = 0.0f;
    for (int i = 0; i < c->nbLignes; i++)
        brut += c->lignes[i].sousTotalHT;
    c->montantBrutHT = brut;
        if (brut >= seuil_remise_15) {
            c->tauxRemise = remise_15;
        } else if (brut >= seuil_remise_10) {
            c->tauxRemise = remise_10;
        } else if (brut >= seuil_remise_5) {
            c->tauxRemise = remise_5;
        } else {
            c->tauxRemise = remise_0;
        }
    c->montantRemise = brut * c->tauxRemise / 100.0f;
    c->montantHT     = brut - c->montantRemise;
    c->montantTVA    = c->montantHT * TVA / 100.0f;
    c->fraisPort     = (brut >= Frais_de_port_offert) ? 0.0f : Frais_de_port;
    c->montantTTC    = c->montantHT + c->montantTVA + c->fraisPort;
}
