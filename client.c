#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "client.h"
#include "initialisation.h"

/*Quelques fonctions utiles */

Client *rechercherClient(Boutique *b, int idClient) {
    for (int i = 0; i < b->nbClients; i++) {
        if (b->clients[i].idClient == idClient) {
            return &b->clients[i];
        }
    }
    return NULL;
}
int rechercherEmail(Boutique *b, const char *email) {
    for (int i = 0; i < b->nbClients; i++) {
        if (strcmp(b->clients[i].email, email) == 0) {
            return 1;
        }
    }
    return 0;
}

void ajouterClient(Boutique *b) {
    Client nouveau;
    /* Inserer de nouveaux identifiants */
    printf("\n==============================================\n");
    printf("\n             GESTION DES CLIENTS              \n");
    printf("\n==============================================\n");
    printf("\n");
    printf("\n===== AJOUTER UN PROFIL ====\n");
    printf("NOM : ");
    fgets(nouveau.nom, sizeof(nouveau.nom), stdin);
    nouveau.nom[strcspn(nouveau.nom, "\n")] = '\0';
    printf("PRENOM : ");
    fgets(nouveau.prenom, sizeof(nouveau.prenom), stdin);
    nouveau.prenom[strcspn(nouveau.prenom, "\n")] = '\0';
    do {
        printf("ADRESSE EMAIL : ");
        fgets(nouveau.email, sizeof(nouveau.email), stdin);
        nouveau.email[strcspn(nouveau.email, "\n")] = '\0';
            if (strchr(nouveau.email, '@') == NULL) {
                printf("Cette adresse email n'en est pas une. Il manque '@'.\n");
            }
    } while (rechercherEmail(b, nouveau.email) == 1);
    printf("NUMERO DE TELEPHONE : ");
    fgets(nouveau.telephone, sizeof(nouveau.telephone), stdin);
    nouveau.telephone[strcspn(nouveau.telephone, "\n")] = '\0';
    /* Inserer une nouvelle adresse */
    printf("RUE : ");
    fgets(nouveau.adresse.rue, sizeof(nouveau.adresse.rue), stdin);
    nouveau.adresse.rue[strcspn(nouveau.adresse.rue, "\n")] = '\0';
    printf("VILLE : ");
    fgets(nouveau.adresse.ville, sizeof(nouveau.adresse.ville), stdin);
    nouveau.adresse.ville[strcspn(nouveau.adresse.ville, "\n")] = '\0';
    printf("CODE POSTAL : ");
    fgets(nouveau.adresse.codePostal, sizeof(nouveau.adresse.codePostal), stdin);
    nouveau.adresse.codePostal[strcspn(nouveau.adresse.codePostal, "\n")] = '\0';
    printf("PAYS : ");
    fgets(nouveau.adresse.pays, sizeof(nouveau.adresse.pays), stdin);
    nouveau.adresse.pays[strcspn(nouveau.adresse.pays, "\n")] = '\0';

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
            return 0;
        }
        b->clients = temp;
        b->nbClients++;
        b->capaciteClients = nouvelleCapacite;
    }
    b->clients[b->nbClients] = nouveau;
    b->idClientSuivant++;
    printf("Client ajoute !\n");
}


void modifierClient(Boutique *b) {
    Client modifier;
    printf("\n==============================================\n");
    printf("\n             GESTION DES CLIENTS              \n");
    printf("\n==============================================\n");
    printf("\n");
    printf("\n===== MODIFIER UN PROFIL ====\n");
    printf("ID Client : ");
    scanf("%d", &modifier.idClient);
    getchar();
    Client *c = rechercherClient(b, modifier.idClient);
    if (c == NULL || c->actif == 0) {
        printf("Client introuvable.\n");
        return 0;
    }
    int i;
    char y, n, rep;
    do {
        printf("Adresse de %s %s. Modifier ? (y/n) : ", c->prenom, c->nom);
        scanf(" %c", &rep);
        getchar();
    } while (rep != 'y' && rep != 'n');
    if (rep == y) {
        printf("Saisissez la nouvelle adresse : \n");
        printf("RUE : ");
        fgets(modifier.adresse.rue, sizeof(modifier.adresse.rue), stdin);
        printf("VILLE : ");
        fgets(modifier.adresse.ville, sizeof(modifier.adresse.ville), stdin);
        printf("CODE POSTAL : ");
        fgets(modifier.adresse.codePostal, sizeof(modifier.adresse.codePostal), stdin);
        printf("PAYS : ");
        fgets(modifier.adresse.pays, sizeof(modifier.adresse.pays), stdin);
    }
    do {
        printf("Coordonnees de %s %s? Modifier ? (y/n) : ", c->prenom, c->nom);
        scanf(" %c", &rep);
        getchar();
    } while (rep != 'y' && rep != 'n');
    if (rep == 'y') {
        printf("NOM : ");
        fgets(c->nom, sizeof(c->nom), stdin);
        c->nom[strcspn(c->nom, "\n")] = '\0';
        printf("PRENOM : ");
        fgets(c->prenom, sizeof(c->prenom), stdin);
        c->prenom[strcspn(c->prenom, "\n")] = '\0';
        char email[80];
        do {
            printf("ADRESSE EMAIL : ");
            fgets(email, sizeof(email), stdin);
            email[strcspn(email, "\n")] = '\0';
            if (strchr(email, '@') == NULL) {
                printf("Email invalide (il faut un @).\n");
            } else if (strcmp(email, c->email) != 0 && rechercherEmail(b, email) == 1) {
                printf("Adresse email deja prise.\n");
            } else {
                break;
            }
        } while (1);
        strcpy(c->email, email);
        printf("NUMERO DE TELEPHONE : ");
        fgets(c->telephone, sizeof(c->telephone), stdin);
        c->telephone[strcspn(c->telephone, "\n")] = '\0';
    }
    printf("C'est bon! Les donnees du client sont à jour :)");
    return 1;
}

void supprimerClient(Boutique *b) {
    Client supprimer;
    char y, n, rep;
    printf("\n===== SUPPRIMER UN PROFIL ====\n");
    printf("ID Client : ");
    printf("Chercher le client via nom et prenom ? (1)");
    scanf("%d", &supprimer.idClient);
    getchar();
    Client *c = rechercherClient(b, supprimer.idClient);
    if (c == NULL || c->actif == 0) {
        printf("Client introuvable.\n");
        return 0;
    }
    for (int j = 0; j < c->nbCommandes; j++) {
        if (strcmp(c->commandes[j].statut, ANNULEE) != 0) {
            printf("Suppression impossible : commandes non annulees.\n");
            return 0;
        }
    }
    do {
        printf("Etes-vous sur de vouloir supprimer %s %s ? (y/n) : ", c->prenom, c->nom);
        scanf(" %c", &rep);
        getchar();
    } while (rep != 'y' && rep != 'n');
    if (rep == 'y') {
        c->actif = 0;
        printf("Client supprime.\n");
        return 1;
    }
    return 0;
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
            float chiffreAffaires = 0.0f;
            for (int j = 0; j < c->nbCommandes; j++) {
                if (strcmp(c->commandes[j].statut, ANNULEE) != 0) {
                    chiffreAffaires += c->commandes[j].montantHT;
                }
            }
            printf("\n---------------------------------\n");
            printf("ID Client : %d\n", c->idClient);
            printf("Nom : %s\n", c->nom);
            printf("Prenom : %s\n", c->prenom);
            printf("Email : %s\n", c->email);
            printf("Telephone : %s\n", c->telephone);
            printf("Nombre de commandes : %d\n", c->nbCommandes);
            printf("Chiffre d'affaires HT : %.2f euros\n", chiffreAffaires);
        }
    }
}

void afficherClient(Boutique *b) {
    int idClient;
    printf("\n==== LISTE DES CLIENTS ====\n");
    printf("ID Client : ");
    scanf("%d", &idClient);
    getchar();
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

    printf("\n---- HISTORIQUE DES COMMANDES ----\n");
    if (c->nbCommandes == 0) {
        printf("Aucune commande enregistree.\n");
        return;
    }
    for (int i = 0; i < c->nbCommandes; i++) {
        Commande *cmd = &c->commandes[i];
        printf("\n---------------------------------\n");
        printf("Numero commande : %d\n", cmd->numero);
        printf("Date : %0d/%0d/%d\n", cmd->dateCommande.jour, cmd->dateCommande.mois, cmd->dateCommande.annee);
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
