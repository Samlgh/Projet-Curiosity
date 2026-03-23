#ifndef _OBSERVATEUR_H_
#define _OBSERVATEUR_H_

typedef enum{ M, A, G, D} Signal;

typedef enum{ Init, Erreur, Mes } Etat;

Etat init();

Etat transi(Etat etat, Signal cmd);

int final(Etat etat);

#endif