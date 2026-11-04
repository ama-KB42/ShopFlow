#ifndef CLIENT_H
#define CLIENT_H
#include "types.h"

Client *rechercherClient(Boutique *b, int idClient);
int rechercherEmail(Boutique *b, const char *email);
float chiffreAffairesClient(Client *c);

void ajouterClient(Boutique *b);
void modifierClient(Boutique *b);
void supprimerClient(Boutique *b);
void afficherListe(Boutique *b);
void afficherClient(Boutique *b);

#endif
