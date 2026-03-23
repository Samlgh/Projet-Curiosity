#include "terrain.h"
#include <stdio.h>

int main(int argc, char **argv) {
  FILE *f;
  Terrain t;
  erreur_terrain erreur;
  int x, y;

  if (argc < 2) {
    printf("Usage : %s <fichier>\n", argv[0]);
    return 1;
  }

  f = fopen(argv[1], "r");
  erreur = lire_terrain(f, &t, &x, &y);
  while(erreur != OK){

    switch(erreur){

      case 1: printf("ERREUR_FICHIER, donnez-en un autre:"); break;
      case 2: printf("ERREUR_LECTURE_LARGEUR, donnez-en un autre:"); break;
      case 3: printf("ERREUR_LECTURE_HAUTEUR, donnez-en un autre:"); break;
      case 4: printf("ERREUR_LARGEUR_INCORRECTE, donnez-en un autre:"); break;
      case 5: printf("ERREUR_HAUTEUR_INCORRECTE, donnez-en un autre:"); break;
      case 6: printf("ERREUR_CARACTERE_INCORRECT, donnez-en un autre:"); break;
      case 7: printf("ERREUR_LIGNE_TROP_LONGUE, donnez-en un autre:"); break;
      case 8: printf("ERREUR_LIGNE_TROP_COURTE, donnez-en un autre:"); break;
      case 9: printf("ERREUR_LIGNES_MANQUANTES, donnez-en un autre:"); break;
      case 10: printf("ERREUR_POSITION_ROBOT_MANQUANTE, donnez-en un autre:"); break;
      default: break;
    } 

    scanf("%s", argv[1]);
    f = fopen(argv[1], "r");
    erreur = lire_terrain(f, &t, &x, &y);
  }

  fclose(f);
  afficher_terrain(&t);
  printf("Position initiale du robot : (%d, %d)\n", x, y);
}
