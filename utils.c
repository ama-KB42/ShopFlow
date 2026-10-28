#include <stdio.h>
#include <string.h>
#include "utils.h"

static int lireLigne(const char *message, char *ligne, int taille) {
    printf("%s", message);
    if (fgets(ligne, taille, stdin) == NULL) {
        return 0;
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
