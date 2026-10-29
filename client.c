#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "client.h"
#include "initialisation.h"

void ajouterClient(Client *c) {
    Client nouveau;
    /* Inserer de nouveaux identifiants */
    printf("\n==============================================\n");
    printf("\n             GESTION DES CLIENTS              \n");
    printf("\n==============================================\n");
    printf("\n");
    printf("\n===== AJOUTER UN PROFIL ====\n");
    printf("NOM : ");
    fgets(nouveau.nom, sizeof(nouveau.nom), stdin);
    printf("PRENOM : ");
    fgets(nouveau.prenom, sizeof(nouveau.prenom), stdin);
    printf("ADRESSE EMAIL : ");
    fgets(nouveau.email, sizeof(nouveau.email), stdin);
        if (strcmp(nouveau.email, c->email) == 0) {
            printf("Adresse email deja prise. Veuillez en saisir un nouveau.");
            return ;
        }
    printf("NUMERO DE TELEPHONE : ");
    fgets(nouveau.telephone, sizeof(nouveau.telephone), stdin);
        if (strcmp(nouveau.telephone, c->telephone) == 0) {
            printf("Numero deja pris. Veuillez en saisir un nouveau.");
            return ;
        }
    /* Inserer une nouvelle adresse */
    printf("ADRESSE : ");
    fgets(nouveau.adresse.rue, sizeof(nouveau.adresse), stdin);
    printf("VILLE : ");
    fgets(nouveau.adresse.ville, sizeof(nouveau.adresse.ville), stdin);
    printf("CODE POSTAL : ");
    fgets(nouveau.adresse.codePostal, sizeof(nouveau.adresse.codePostal), stdin);
    printf("PAYS : ");
    fgets(nouveau.adresse.pays, sizeof(nouveau.adresse.pays), stdin);

    /* Initialiser les nouvelles donnes du nouveau client */
    int ajouterClient(Boutique *b);
    nouveau.idClient = b->idClientSuivant;
    nouveau.pointsFidelite = 0;
    nouveau.nbCommandes = 0;
    nouveau.capaciteCommandes = 0;
    nouveau.commandes = 0;
    nouveau.actif = 1;
}

void modifierClient(Client *c) {
    Client modifier;
    printf("\n==============================================\n");
    printf("\n             GESTION DES CLIENTS              \n");
    printf("\n==============================================\n");
    printf("\n");
    printf("\n===== MODIFIER UN PROFIL ====\n");
    printf("ID Client : ");
    scanf("%d", &modifier.idClient);

    char x, y, n;
    if (strcmp(modifier.idClient, c->idClient) == 0) {
        printf("%s\n", modifier.client.idClient);
        printf("Adresse livraison : %s. Modifier ? (y/n) \n", modifier.client.idClient);
        scanf("%c", &x);
        if (char x = y) {
            printf("Saisissez la nouvelle adresse : \n");
            printf("ADRESSE : ");
            fgets(modifier.adresse.rue, sizeof(modifier.adresse), stdin);
            printf("VILLE : ");
            fgets(modifier.adresse.ville, sizeof(modifier.adresse.ville), stdin);
            printf("CODE POSTAL : ");
            fgets(modifier.adresse.codePostal, sizeof(modifier.adresse.codePostal), stdin);
            printf("PAYS : ");
            fgets(modifier.adresse.pays, sizeof(modifier.adresse.pays), stdin);
        } else if {char x = n) {

        } else {
            return 0;
        }
    }
}
    /* A suivre */

void supprimerClient(Client *c) {
    Client supprimer;
    printf("\n==============================================\n");
    printf("\n             GESTION DES CLIENTS              \n");
    printf("\n==============================================\n");
    printf("\n");
    printf("\n===== SUPPRIMER UN PROFIL ====\n");
    printf("ID Client : ");
    scanf("%d", &supprimer.idClient);
/* A suivre */
}

void afficherListe(Client *c) {

}

void afficherClient(Client *c) {

}
