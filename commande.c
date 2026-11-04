/* commande.c - creation des commandes, gestion des lignes, calcul des montants,
 * changement de statut et annulation */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "commande.h"
#include "client.h"
#include "produit.h"
#include "utils.h"

/* ---------- Fonctions de base sur les lignes ---------- */

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

/* Libere toutes les lignes d'une commande */
void libererLignesCommande(Commande *commande) {
    if (commande == NULL) return;
    free(commande->lignes);
    commande->lignes = NULL;
    commande->nbLignes = 0;
    commande->capaciteLignes = 0;
}

/* Calcul en cascade : lignes -> montant brut -> remise -> HT -> TVA -> port -> TTC */
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

/* ---------- Fonctions utilitaires internes ---------- */

/* Recherche une commande par son numero dans toute la boutique */
Commande *rechercherCommande(Boutique *b, int numero) {
    for (int i = 0; i < b->nbClients; i++) {
        for (int j = 0; j < b->clients[i].nbCommandes; j++) {
            if (b->clients[i].commandes[j].numero == numero) {
                return &b->clients[i].commandes[j];
            }
        }
    }
    return NULL;
}

/* Retourne l'indice de la ligne qui porte cette reference, ou -1 */
static int chercherLigne(Commande *c, const char *reference) {
    for (int i = 0; i < c->nbLignes; i++) {
        if (strcmp(c->lignes[i].refProduit, reference) == 0) {
            return i;
        }
    }
    return -1;
}

/* Remet en stock les quantites de toutes les lignes d'une commande */
static void reintegrerStock(Boutique *b, Commande *c) {
    for (int i = 0; i < c->nbLignes; i++) {
        Produit *p = trouverProduit(b, c->lignes[i].refProduit);
        if (p != NULL) {
            p->quantiteStock += c->lignes[i].quantite;
        }
    }
}

/* Demande un numero de commande et retourne la commande (ou NULL) */
static Commande *demanderCommande(Boutique *b) {
    int numero;
    if (lireEntier("NUMERO DE COMMANDE : ", &numero) == 0) {
        printf("Numero invalide.\n");
        return NULL;
    }
    Commande *c = rechercherCommande(b, numero);
    if (c == NULL) {
        printf("Commande introuvable.\n");
    }
    return c;
}

/* Saisie d'une ligne (reference + quantite) et ajout dans la commande.
 * Le stock est retire tout de suite. Si la reference est deja dans la
 * commande, la quantite est cumulee sur la meme ligne (pas de doublon).
 * Comme le stock des lignes deja saisies a deja ete retire, verifier
 * "quantite <= stock restant" revient a controler la quantite cumulee.
 * Retourne 1 si la ligne a ete ajoutee, 0 sinon. */
static int saisirLigne(Boutique *b, Commande *c) {
    char reference[12];
    int quantite;
    lireChaineNonVide("REFERENCE PRODUIT : ", reference, sizeof(reference));
    Produit *p = trouverProduit(b, reference);
    if (p == NULL || p->actif == 0) {
        printf("Produit introuvable.\n");
        return 0;
    }
    if (p->quantiteStock == 0) {
        printf("Le produit est en rupture de stock.\n");
        return 0;
    }
    printf("%s - %.2f euros HT - stock disponible : %d\n", p->designation, p->prixUnitaireHT, p->quantiteStock);
    if (lireEntier("QUANTITE : ", &quantite) == 0 || quantite < 1 || quantite > p->quantiteStock) {
        printf("Quantite invalide (entre 1 et %d).\n", p->quantiteStock);
        return 0;
    }

    int indice = chercherLigne(c, reference);
    if (indice != -1) {
        /* La reference existe deja : on cumule la quantite */
        c->lignes[indice].quantite += quantite;
        c->lignes[indice].sousTotalHT = c->lignes[indice].quantite * c->lignes[indice].prixUnitaireHT;
        printf("Quantite cumulee sur la ligne existante (total : %d).\n", c->lignes[indice].quantite);
    } else {
        LigneCommande ligne;
        strcpy(ligne.refProduit, p->reference);
        strcpy(ligne.designation, p->designation);     /* libelle fige */
        ligne.quantite = quantite;
        ligne.prixUnitaireHT = p->prixUnitaireHT;      /* prix fige */
        ligne.sousTotalHT = ligne.quantite * ligne.prixUnitaireHT;
        if (ajouterLigneCommande(c, ligne) == 0) {
            printf("Erreur d'allocation memoire (ligne de commande).\n");
            return 0;
        }
    }
    p->quantiteStock -= quantite;
    printf("Ligne ajoutee.\n");
    return 1;
}

/* Recalcule une commande et met a jour les points de fidelite du client
 * (on retire les anciens points puis on ajoute les nouveaux) */
static void recalculerAvecFidelite(Boutique *b, Commande *c) {
    Client *cl = rechercherClient(b, c->idClient);
    int anciensPoints = POINTS_FIDELITE(c->montantTTC);
    calculCommande(c);
    if (cl != NULL) {
        cl->pointsFidelite = cl->pointsFidelite - anciensPoints + POINTS_FIDELITE(c->montantTTC);
        if (cl->pointsFidelite < 0) {
            cl->pointsFidelite = 0;
        }
    }
}

/* ---------- Fonctionnalites du menu ---------- */
void creerCommande(Boutique *b) {
    int idClient;
    printf("\n==============================================\n");
    printf("\n             GESTION DES COMMANDES              \n");
    printf("\n==============================================\n");
    printf("\n");
    printf("\n===== CREER UNE COMMANDE ====\n");

    /* 1. Selection du client */
    if (lireEntier("ID CLIENT : ", &idClient) == 0) {
        printf("Identifiant invalide.\n");
        return;
    }
    Client *cl = rechercherClient(b, idClient);
    if (cl == NULL || cl->actif == 0) {
        printf("Client introuvable.\n");
        return;
    }
    printf("Commande pour %s %s\n", cl->prenom, cl->nom);

    /* 2. Date de la commande */
    Commande c;
    initialiserLigneCommande(&c);
    printf("Date de la commande :\n");
    c.dateCommande = lireDate();

    /* 3. Saisie des N lignes */
    do {
        saisirLigne(b, &c);
    } while (lireOuiNon("Ajouter une autre ligne ? (y/n) : ") == 1);

    if (c.nbLignes == 0) {
        printf("Commande vide : creation annulee.\n");
        libererLignesCommande(&c);
        return;
    }

    /* 4. Place dans l'historique du client (realloc sur NULL = malloc) */
    if (cl->nbCommandes == cl->capaciteCommandes) {
        int nouvelleCapacite;
        if (cl->capaciteCommandes == 0) {
            nouvelleCapacite = 2;
        } else {
            nouvelleCapacite = cl->capaciteCommandes * 2;
        }
        Commande *temp = realloc(cl->commandes, nouvelleCapacite * sizeof(Commande));
        if (temp == NULL) {
            printf("Erreur d'allocation memoire (commandes du client).\n");
            reintegrerStock(b, &c);
            libererLignesCommande(&c);
            return;
        }
        cl->commandes = temp;
        cl->capaciteCommandes = nouvelleCapacite;
    }

    /* 5. Finalisation : numero automatique, statut, adresse figee, montants, fidelite */
    c.numero = b->numeroSuivant++;
    c.idClient = cl->idClient;
    strcpy(c.statut, EN_COURS);
    c.adresseLivraison = cl->adresse;
    calculCommande(&c);
    cl->pointsFidelite += POINTS_FIDELITE(c.montantTTC);
    cl->commandes[cl->nbCommandes++] = c;           /* le client possede desormais c.lignes */

    printf("Commande %d creee.\n", c.numero);
    afficherDetailCommande(&c);
}

/* Ajout d'une ligne : uniquement sur une commande EN_COURS */
void ajouterLigne(Boutique *b) {
    printf("\n===== AJOUTER UNE LIGNE ====\n");
    Commande *c = demanderCommande(b);
    if (c == NULL) return;
    if (strcmp(c->statut, EN_COURS) != 0) {
        printf("Impossible : la commande n'est pas EN_COURS (statut %s).\n", c->statut);
        return;
    }
    if (saisirLigne(b, c) == 1) {
        recalculerAvecFidelite(b, c);
        afficherDetailCommande(c);
    }
}

/* Suppression d'une ligne : uniquement sur une commande EN_COURS, stock reintegre */
void supprimerLigne(Boutique *b) {
    char reference[12];
    printf("\n===== SUPPRIMER UNE LIGNE ====\n");
    Commande *c = demanderCommande(b);
    if (c == NULL) return;
    if (strcmp(c->statut, EN_COURS) != 0) {
        printf("Impossible : la commande n'est pas EN_COURS (statut %s).\n", c->statut);
        return;
    }
    if (c->nbLignes == 1) {
        printf("Impossible : une commande doit garder au moins une ligne (utilisez l'annulation).\n");
        return;
    }
    for (int i = 0; i < c->nbLignes; i++) {
        printf("  %s - %s x %d\n", c->lignes[i].refProduit, c->lignes[i].designation, c->lignes[i].quantite);
    }
    lireChaineNonVide("REFERENCE A SUPPRIMER : ", reference, sizeof(reference));
    int indice = chercherLigne(c, reference);
    if (indice == -1) {
        printf("Cette reference n'est pas dans la commande.\n");
        return;
    }
    /* Reintegration du stock */
    Produit *p = trouverProduit(b, reference);
    if (p != NULL) {
        p->quantiteStock += c->lignes[indice].quantite;
    }
    /* Decalage des lignes suivantes */
    for (int i = indice; i < c->nbLignes - 1; i++) {
        c->lignes[i] = c->lignes[i + 1];
    }
    c->nbLignes--;
    recalculerAvecFidelite(b, c);
    printf("Ligne supprimee.\n");
    afficherDetailCommande(c);
}

/* Progression autorisee : EN_COURS -> EXPEDIEE -> LIVREE */
void changerStatut(Boutique *b) {
    printf("\n===== CHANGER LE STATUT ====\n");
    Commande *c = demanderCommande(b);
    if (c == NULL) return;
    if (strcmp(c->statut, EN_COURS) == 0) {
        if (lireOuiNon("Passer la commande au statut EXPEDIEE ? (y/n) : ") == 1) {
            strcpy(c->statut, EXPEDIEE);
            printf("Commande %d : EXPEDIEE.\n", c->numero);
        }
    } else if (strcmp(c->statut, EXPEDIEE) == 0) {
        if (lireOuiNon("Passer la commande au statut LIVREE ? (y/n) : ") == 1) {
            strcpy(c->statut, LIVREE);
            printf("Commande %d : LIVREE.\n", c->numero);
        }
    } else {
        printf("Impossible : une commande %s ne peut plus changer de statut.\n", c->statut);
    }
}

/* Annulation : seulement au statut EN_COURS, stock reintegre et points retires */
void annulerCommande(Boutique *b) {
    printf("\n===== ANNULER UNE COMMANDE ====\n");
    Commande *c = demanderCommande(b);
    if (c == NULL) return;
    if (strcmp(c->statut, EN_COURS) != 0) {
        printf("Impossible : seule une commande EN_COURS peut etre annulee (statut %s).\n", c->statut);
        return;
    }
    if (lireOuiNon("Confirmer l'annulation ? (y/n) : ") == 0) {
        return;
    }
    reintegrerStock(b, c);
    Client *cl = rechercherClient(b, c->idClient);
    if (cl != NULL) {
        cl->pointsFidelite -= POINTS_FIDELITE(c->montantTTC);
        if (cl->pointsFidelite < 0) {
            cl->pointsFidelite = 0;
        }
    }
    strcpy(c->statut, ANNULEE);
    printf("Commande %d annulee, stock reintegre.\n", c->numero);
}

/* Affiche le detail complet d'une commande */
void afficherDetailCommande(Commande *c) {
    printf("\n---------------------------------------------------------------\n");
    printf("COMMANDE N.%d   -   Client %d   -   %02d/%02d/%d\n", c->numero, c->idClient,
           c->dateCommande.jour, c->dateCommande.mois, c->dateCommande.annee);
    printf("Statut : %s\n", c->statut);
    printf("Livraison : %s, %s %s, %s\n", c->adresseLivraison.rue, c->adresseLivraison.codePostal,
           c->adresseLivraison.ville, c->adresseLivraison.pays);
    printf("---------------------------------------------------------------\n");
    printf("%-10s %-20s %5s %10s %12s\n", "REF", "DESIGNATION", "QTE", "PRIX HT", "SOUS-TOTAL");
    for (int i = 0; i < c->nbLignes; i++) {
        LigneCommande *l = &c->lignes[i];
        printf("%-10s %-20s %5d %10.2f %12.2f\n", l->refProduit, l->designation, l->quantite, l->prixUnitaireHT, l->sousTotalHT);
    }
    printf("---------------------------------------------------------------\n");
    printf("Montant brut HT        : %10.2f euros\n", c->montantBrutHT);
    printf("Remise (%4.1f %%)      : %10.2f euros\n", c->tauxRemise, c->montantRemise);
    printf("Montant HT             : %10.2f euros\n", c->montantHT);
    printf("TVA (%.0f %%)          : %10.2f euros\n", TVA, c->montantTVA);
    printf("Frais de port          : %10.2f euros\n", c->fraisPort);
    printf("TOTAL TTC              : %10.2f euros\n", c->montantTTC);
    printf("---------------------------------------------------------------\n");
}

void afficherCommande(Boutique *b) {
    printf("\n===== AFFICHER UNE COMMANDE ====\n");
    Commande *c = demanderCommande(b);
    if (c != NULL) {
        afficherDetailCommande(c);
    }
}
