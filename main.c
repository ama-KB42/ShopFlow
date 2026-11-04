/* ==========================================================================
 * main.c - Point d'entree de ShopFlow (XTI101)
 * --------------------------------------------------------------------------
 * - cree la structure Boutique (structure racine) ;
 * - charge le jeu de donnees de demonstration (initialisation.c) ;
 * - fait tourner le menu principal et ses 4 sous-menus ;
 * - libere toute la memoire avant de quitter.
 * ========================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "utils.h"
#include "produit.h"
#include "client.h"
#include "commande.h"
#include "statistique.h"
#include "initialisation.h"

/* --------------------------------------------------------------------------
 * Lecture d'un choix au clavier avec la saisie securisee du module utils.c
 * Retourne -1 si la saisie n'est pas un entier (=> "Choix invalide").
 * -------------------------------------------------------------------------- */
static int lireChoix(void)
{
    int choix;
    if (lireEntier("  Votre choix : ", &choix) == 0)
    {
        return -1;
    }
    return choix;
}

/* ==========================================================================
 * SOUS-MENUS : un seul motif a retenir, applique 4 fois.
 * ========================================================================== */

static void menuProduits(Boutique *b)
{
    int choix;
    do
    {
        printf("\n===== GESTION DES PRODUITS =====\n");
        printf("  1. Ajouter un produit\n");
        printf("  2. Modifier un produit\n");
        printf("  3. Supprimer un produit\n");
        printf("  4. Afficher le catalogue\n");
        printf("  5. Rechercher un produit\n");
        printf("  6. Recherche multi-criteres\n");
        printf("  0. Retour au menu principal\n");
        choix = lireChoix();

        switch (choix)
        {
            case 1: ajouterProduit(b);         break;
            case 2: modifierProduit(b);        break;
            case 3: supprimerProduit(b);       break;
            case 4: afficherCatalogue(b);      break;
            case 5: rechercherProduit(b);      break;
            case 6: rechercheMultiCriteres(b); break;
            case 0:                            break; /* retour automatique */
            default: printf("  [!] Choix invalide.\n"); break;
        }
    } while (choix != 0);
}

static void menuClients(Boutique *b)
{
    int choix;
    do
    {
        printf("\n===== GESTION DES CLIENTS =====\n");
        printf("  1. Ajouter un client\n");
        printf("  2. Modifier un client\n");
        printf("  3. Supprimer un client\n");
        printf("  4. Afficher la liste des clients\n");
        printf("  5. Afficher la fiche d'un client\n");
        printf("  0. Retour au menu principal\n");
        choix = lireChoix();

        switch (choix)
        {
            case 1: ajouterClient(b);   break;
            case 2: modifierClient(b);  break;
            case 3: supprimerClient(b); break;
            case 4: afficherListe(b);   break;
            case 5: afficherClient(b);  break;
            case 0:                     break;
            default: printf("  [!] Choix invalide.\n"); break;
        }
    } while (choix != 0);
}

static void menuCommandes(Boutique *b)
{
    int choix;
    do
    {
        printf("\n===== GESTION DES COMMANDES =====\n");
        printf("  1. Creer une commande\n");
        printf("  2. Ajouter une ligne a une commande\n");
        printf("  3. Supprimer une ligne d'une commande\n");
        printf("  4. Changer le statut d'une commande\n");
        printf("  5. Annuler une commande\n");
        printf("  6. Afficher une commande\n");
        printf("  0. Retour au menu principal\n");
        choix = lireChoix();

        switch (choix)
        {
            case 1: creerCommande(b);    break;
            case 2: ajouterLigne(b);     break;
            case 3: supprimerLigne(b);   break;
            case 4: changerStatut(b);    break;
            case 5: annulerCommande(b);  break;
            case 6: afficherCommande(b); break;
            case 0:                      break;
            default: printf("  [!] Choix invalide.\n"); break;
        }
    } while (choix != 0);
}

/* Saisie de la periode puis affichage du chiffre d'affaires correspondant */
static void menuChiffreAffairesPeriode(Boutique *b)
{
    int mois, annee;
    while (lireEntier("  Mois (1-12, 0 = toute l'annee) : ", &mois) == 0 || mois < 0 || mois > 12)
    {
        printf("  [!] Mois invalide.\n");
    }
    while (lireEntier("  Annee : ", &annee) == 0 || annee < 2000 || annee > 2100)
    {
        printf("  [!] Annee invalide (2000 a 2100).\n");
    }
    afficherChiffreAffairesPeriode(b, mois, annee);
}

static void menuStatistiques(Boutique *b)
{
    int choix;
    do
    {
        printf("\n===== STATISTIQUES =====\n");
        printf("  1. Chiffre d'affaires global\n");
        printf("  2. Chiffre d'affaires par periode\n");
        printf("  3. Palmares produits (top 5)\n");
        printf("  4. Palmares clients\n");
        printf("  5. Alertes de stock\n");
        printf("  6. Statistiques par categorie\n");
        printf("  0. Retour au menu principal\n");
        choix = lireChoix();

        switch (choix)
        {
            case 1: afficherChiffreAffaires(b);        break;
            case 2: menuChiffreAffairesPeriode(b);     break;
            case 3: afficherPalmaresProduits(b);       break;
            case 4: afficherPalmaresClients(b);        break;
            case 5: afficherAlertesStock(b);           break;
            case 6: afficherStatistiquesCategories(b); break;
            case 0:                                    break;
            default: printf("  [!] Choix invalide.\n"); break;
        }
    } while (choix != 0);
}

/* ==========================================================================
 * PROGRAMME PRINCIPAL
 * ========================================================================== */
int main(void)
{
    int choix;
    Boutique boutique;

    initialiserBoutique(&boutique);
    initialiserDonneesTest(&boutique);

    printf("==============================================\n");
    printf("  %s - Boutique en ligne\n", boutique.nom);
    printf("==============================================\n");
    printf("  %d produits, %d clients charges.\n", boutique.nbProduits, boutique.nbClients);

    do
    {
        printf("\n===== MENU PRINCIPAL =====\n");
        printf("  1. Gestion des produits\n");
        printf("  2. Gestion des clients\n");
        printf("  3. Gestion des commandes\n");
        printf("  4. Statistiques\n");
        printf("  0. Quitter\n");
        choix = lireChoix();

        switch (choix)
        {
            case 1: menuProduits(&boutique);     break;
            case 2: menuClients(&boutique);      break;
            case 3: menuCommandes(&boutique);    break;
            case 4: menuStatistiques(&boutique); break;
            case 0: printf("\n  Au revoir !\n"); break;
            default: printf("  [!] Choix invalide.\n"); break;
        }
    } while (choix != 0);

    /* Liberation complete de la memoire */
    detruireBoutique(&boutique);
    return EXIT_SUCCESS;
}
