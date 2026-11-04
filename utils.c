/* utils.c - saisie securisee et validation des donnees */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

/* Lit une ligne complete au clavier et enleve le '\n'.
 * Retourne 0 si la ligne est trop longue (le reste est vide du clavier). */
static int lireLigne(const char *message, char *ligne, int taille) {
    printf("%s", message);
    if (fgets(ligne, taille, stdin) == NULL) {
        /* entree fermee (Ctrl+D) : on quitte proprement */
        printf("\nFin de la saisie.\n");
        exit(EXIT_FAILURE);
    }
    char *fin = strchr(ligne, '\n');
    if (fin != NULL) {
        *fin = '\0';
        return 1;
    }
    int caractere = getchar();
    if (caractere == '\n' || caractere == EOF) {
        return 1;
    }
    while (caractere != '\n' && caractere != EOF) {
        caractere = getchar();
    }
    printf("Erreur : saisie trop longue.\n");
    return 0;
}

int lireEntier(const char *message, int *valeur) {
    char ligne[100];
    char reste;
    if (valeur == NULL || lireLigne(message, ligne, 100) == 0) {
        return 0;
    }
    return sscanf(ligne, "%d %c", valeur, &reste) == 1;
}

int lireReel(const char *message, float *valeur) {
    char ligne[100];
    char reste;
    if (valeur == NULL || lireLigne(message, ligne, 100) == 0) {
        return 0;
    }
    return sscanf(ligne, "%f %c", valeur, &reste) == 1;
}

int lireChaine(const char *message, char *chaine, int taille) {
    if (chaine == NULL || taille <= 0) {
        return 0;
    }
    return lireLigne(message, chaine, taille);
}

/* Redemande la saisie tant que la chaine est vide ou trop longue */
void lireChaineNonVide(const char *message, char *chaine, int taille) {
    while (lireChaine(message, chaine, taille) == 0 || chaine[0] == '\0') {
        printf("Erreur : ce champ ne peut pas etre vide.\n");
    }
}

/* Pose une question (y/n) jusqu'a obtenir une reponse valide.
 * Retourne 1 pour 'y' et 0 pour 'n'. */
int lireOuiNon(const char *message) {
    char rep[10];
    do {
        if (lireChaine(message, rep, 10) == 0) {
            rep[0] = '\0';
        }
    } while (strcmp(rep, "y") != 0 && strcmp(rep, "n") != 0);
    return rep[0] == 'y';
}

/* Verifie que la chaine contient exactement 'longueur' chiffres */
int estNumerique(const char *chaine, int longueur) {
    int i = 0;
    while (chaine[i] != '\0') {
        if (chaine[i] < '0' || chaine[i] > '9') {
            return 0;
        }
        i++;
    }
    return i == longueur;
}

/* Annee bissextile : divisible par 4 mais pas par 100, ou divisible par 400 */
int estBissextile(int annee) {
    return (annee % 4 == 0 && annee % 100 != 0) || annee % 400 == 0;
}

/* Verifie une date (jour valide selon le mois, annee entre 2000 et 2100) */
int dateValide(Date d) {
    int joursParMois[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (d.annee < 2000 || d.annee > 2100) return 0;
    if (d.mois < 1 || d.mois > 12) return 0;
    if (estBissextile(d.annee) == 1) {
        joursParMois[1] = 29;
    }
    if (d.jour < 1 || d.jour > joursParMois[d.mois - 1]) return 0;
    return 1;
}

/* Saisie d'une date valide (redemande tant qu'elle est incorrecte) */
Date lireDate(void) {
    Date d;
    do {
        if (lireEntier("JOUR : ", &d.jour) == 0) d.jour = 0;
        if (lireEntier("MOIS : ", &d.mois) == 0) d.mois = 0;
        if (lireEntier("ANNEE : ", &d.annee) == 0) d.annee = 0;
        if (dateValide(d) == 0) {
            printf("Erreur : date invalide (annee entre 2000 et 2100).\n");
        }
    } while (dateValide(d) == 0);
    return d;
}
