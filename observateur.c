#include <stdio.h>
#include "observateur.h"

/* Etat initial (debut de l automate)*/
Etat init(){return Init;}

Etat transi(Etat etat, Signal cmd){
    switch (etat){
        
        case Init:
            switch (cmd){
                case A:
                    return Erreur; //(Init, A, Erreur)

                case M:
                    return Mes; //(Init, M, Mes)

                case G: case D:
                    return Init; //(Init, G, Init) & (Init, D, Init)
            }


        case Mes:
            switch (cmd){
                case M: case G: case D: //(Mes, M, Mes) & (Mes, G, Mes) & (Mes, D, Mes)
                    return Mes;
                
                case A:
                    return Init; //(Mes, A, Init)
            }
            
        case Erreur:
            return Erreur; //(Erreur, *, Erreur)
    }
}

int final(Etat etat){return etat!=Erreur;}