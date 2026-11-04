#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "client.h"
#include "utils.h"

/*Quelques fonctions utiles */
Client *rechercherClient(Boutique *b, int idClient) {
    for (int i = 0; i < b->nbClients; i++) {
        if (b->clients[i].idClient == idClient) {
            return &b->clients[i];
        }
    }
    return NULL;
}

/* Pour chercher parmi TOUS les clients (cad y compris desactives) */
int rechercherEmail(Boutique *b, const char *email) {
    for (int i = 0; i < b->nbClients; i++) {
        if (strcmp(b->clients[i].email, email) == 0) {
            return 1;
        }
    }
    return 0;
}

/* Calcule le chiffre d'affaires HT d'un client (commandes non annulees) */
float chiffreAffairesClient(Client *c) {
    float chiffreAffaires = 0.0f;
    for (int j = 0; j < c->nbCommandes; j++) {
        if (strcmp(c->commandes[j].statut, ANNULEE) != 0) {
            chiffreAffaires += c->commandes[j].montantHT;
        }
    }
    return chiffreAffaires;
}

/* Saisie d'une adresse complete avec controles */
static void saisirAdresse(Adresse *a) {
    lireChaineNonVide("RUE : ", a->rue, sizeof(a->rue));
    lireChaineNonVide("VILLE : ", a->ville, sizeof(a->ville));
    while (lireChaine("CODE POSTAL : ", a->codePostal, sizeof(a->codePostal)) == 0 || estNumerique(a->codePostal, 5) == 0) {
        printf("Le code postal doit contenir 5 chiffres.\n");
    }
    lireChaineNonVide("PAYS : ", a->pays, sizeof(a->pays));
}

/* Saisie d'un numero de telephone de 10 chiffres */
static void saisirTelephone(char *telephone, int taille) {
    while (lireChaine("NUMERO DE TELEPHONE : ", telephone, taille) == 0 || estNumerique(telephone, 10) == 0) {
        printf("Le telephone doit contenir 10 chiffres.\n");
    }
}

void ajouterClient(Boutique *b) {
    Client nouveau;
    /* Inserer de nouveaux identifiants */
    printf("\n==============================================\n");
    printf("\n             GESTION DES CLIENTS              \n");
    printf("\n==============================================\n");
    printf("\n");
    printf("\n===== AJOUTER UN PROFIL ====\n");
    lireChaineNonVide("NOM : ", nouveau.nom, sizeof(nouveau.nom));
    lireChaineNonVide("PRENOM : ", nouveau.prenom, sizeof(nouveau.prenom));
    do {
        lireChaineNonVide("ADRESSE EMAIL : ", nouveau.email, sizeof(nouveau.email));
        if (strchr(nouveau.email, '@') == NULL) {
            printf("Cette adresse email n'en est pas une. Il manque '@'.\n");
        } else if (rechercherEmail(b, nouveau.email) == 1) {
            printf("Adresse email deja prise.\n");
        }
    } while (strchr(nouveau.email, '@') == NULL || rechercherEmail(b, nouveau.email) == 1);
    saisirTelephone(nouveau.telephone, sizeof(nouveau.telephone));
    /* Inserer une nouvelle adresse */
    saisirAdresse(&nouveau.adresse);

    /* Initialiser les nouvelles donnees du nouveau client */
    nouveau.idClient = b->idClientSuivant;
    nouveau.pointsFidelite = 0;
    nouveau.nbCommandes = 0;
    nouveau.capaciteCommandes = 0;
    nouveau.commandes = NULL;
    nouveau.actif = 1;

    if (b->nbClients == b->capaciteClients) {
        int nouvelleCapacite;
        if (b->capaciteClients == 0) {
            nouvelleCapacite = 2;
        } else {
            nouvelleCapacite = b->capaciteClients * 2;
        }
        Client *temp = realloc(b->clients, nouvelleCapacite * sizeof(Client));
        if (temp == NULL) {
            printf("Erreur d'allocation memoire.\n");
            return;
        }
        b->clients = temp;
        b->capaciteClients = nouvelleCapacite;
    }
    b->clients[b->nbClients] = nouveau;
    b->nbClients++;
    b->idClientSuivant++;
    printf("Client ajoute ! (ID : %d)\n", nouveau.idClient);
}


void modifierClient(Boutique *b) {
    int idClient;
    printf("\n==============================================\n");
    printf("\n             GESTION DES CLIENTS              \n");
    printf("\n==============================================\n");
    printf("\n");
    printf("\n===== MODIFIER UN PROFIL ====\n");
    if (lireEntier("ID Client : ", &idClient) == 0) {
        printf("Identifiant invalide.\n");
        return;
    }
    Client *c = rechercherClient(b, idClient);
    if (c == NULL || c->actif == 0) {
        printf("Client introuvable.\n");
        return;
    }
    printf("Adresse actuelle : %s, %s %s, %s\n", c->adresse.rue, c->adresse.codePostal, c->adresse.ville, c->adresse.pays);
    if (lireOuiNon("Modifier l'adresse ? (y/n) : ") == 1) {
        printf("Saisissez la nouvelle adresse : \n");
        saisirAdresse(&c->adresse);
    }
    if (lireOuiNon("Modifier les coordonnees ? (y/n) : ") == 1) {
        lireChaineNonVide("NOM : ", c->nom, sizeof(c->nom));
        lireChaineNonVide("PRENOM : ", c->prenom, sizeof(c->prenom));
        char email[80];
        do {
            lireChaineNonVide("ADRESSE EMAIL : ", email, sizeof(email));
            if (strchr(email, '@') == NULL) {
                printf("Email invalide (il faut un @).\n");
            } else if (strcmp(email, c->email) != 0 && rechercherEmail(b, email) == 1) {
                printf("Adresse email deja prise.\n");
            } else {
                break;
            }
        } while (1);
        strcpy(c->email, email);
        saisirTelephone(c->telephone, sizeof(c->telephone));
    }
    printf("C'est bon! Les donnees du client sont a jour :)\n");
}

/* Suppression logique : refusee si le client a des commandes non annulees */
void supprimerClient(Boutique *b) {
    int idClient;
    printf("\n===== SUPPRIMER UN PROFIL ====\n");
    if (lireEntier("ID Client : ", &idClient) == 0) {
        printf("Identifiant invalide.\n");
        return;
    }
    Client *c = rechercherClient(b, idClient);
    if (c == NULL || c->actif == 0) {
        printf("Client introuvable.\n");
        return;
    }
    for (int j = 0; j < c->nbCommandes; j++) {
        if (strcmp(c->commandes[j].statut, ANNULEE) != 0) {
            printf("Suppression impossible : commandes non annulees.\n");
            return;
        }
    }
    printf("Etes-vous sur de vouloir supprimer %s %s ? ", c->prenom, c->nom);
    if (lireOuiNon("(y/n) : ") == 1) {
        c->actif = 0;
        printf("Client supprime.\n");
    }
}

void afficherListe(Boutique *b) {
    printf("\n==== LISTE DES CLIENTS ====\n");
    if (b->nbClients == 0) {
        printf("Aucun client enregistre.\n");
        return;
    }
    for (int i = 0; i < b->nbClients; i++) {
        Client *c = &b->clients[i];
        if (c->actif == 1) {
            printf("\n---------------------------------\n");
            printf("ID Client : %d\n", c->idClient);
            printf("Nom : %s\n", c->nom);
            printf("Prenom : %s\n", c->prenom);
            printf("Email : %s\n", c->email);
            printf("Telephone : %s\n", c->telephone);
            printf("Nombre de commandes : %d\n", c->nbCommandes);
            printf("Chiffre d'affaires HT : %.2f euros\n", chiffreAffairesClient(c));
        }
    }
}

void afficherClient(Boutique *b) {
    int idClient;
    printf("\n==== FICHE CLIENT ====\n");
    if (lireEntier("ID Client : ", &idClient) == 0) {
        printf("Identifiant invalide.\n");
        return;
    }
    Client *c = rechercherClient(b, idClient);
    if (c == NULL) {
        printf("Client introuvable.\n");
        return;
    }
    printf("\n---- INFORMATIONS CLIENT ----\n");
    printf("ID : %d\n", c->idClient);
    printf("Nom : %s %s\n", c->prenom, c->nom);
    printf("Email : %s\n", c->email);
    printf("Telephone : %s\n", c->telephone);

    printf("\n---- ADRESSE DE LIVRAISON ----\n");
    printf("Rue : %s\n", c->adresse.rue);
    printf("Ville : %s\n", c->adresse.ville);
    printf("Code postal : %s\n", c->adresse.codePostal);
    printf("Pays : %s\n", c->adresse.pays);

    printf("\n---- FIDELITE ----\n");
    printf("Points de fidelite : %d\n", c->pointsFidelite);
    printf("Statut : %s\n", c->actif == 1 ? "Actif" : "Inactif");
    printf("Chiffre d'affaires HT : %.2f euros\n", chiffreAffairesClient(c));

    printf("\n---- HISTORIQUE DES COMMANDES ----\n");
    if (c->nbCommandes == 0) {
        printf("Aucune commande enregistree.\n");
        return;
    }
    for (int i = 0; i < c->nbCommandes; i++) {
        Commande *cmd = &c->commandes[i];
        printf("\n---------------------------------\n");
        printf("Numero commande : %d\n", cmd->numero);
        printf("Date : %02d/%02d/%d\n", cmd->dateCommande.jour, cmd->dateCommande.mois, cmd->dateCommande.annee);
        printf("Statut : %s\n", cmd->statut);
        printf("Montant HT : %.2f euros\n", cmd->montantHT);
        printf("Montant TTC : %.2f euros\n", cmd->montantTTC);
        printf("Produits commandes :\n");
        for (int j = 0; j < cmd->nbLignes; j++) {
            LigneCommande *ligne = &cmd->lignes[j];
            printf("  - %s | Quantite : %d | Sous-total : %.2f euros\n", ligne->designation, ligne->quantite, ligne->sousTotalHT);
        }
    }
}
