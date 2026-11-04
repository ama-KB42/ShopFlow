#ifndef UTILS_H
#define UTILS_H
#include "types.h"

int lireEntier(const char *message, int *valeur);
int lireReel(const char *message, float *valeur);
int lireChaine(const char *message, char *chaine, int taille);
void lireChaineNonVide(const char *message, char *chaine, int taille);
int lireOuiNon(const char *message);

int estNumerique(const char *chaine, int longueur);
int estBissextile(int annee);
int dateValide(Date d);
Date lireDate(void);

#endif
