#include "terrain.h"
#include "robot.h"
#include <stdio.h>
#include <string.h>

erreur_terrain lire_terrain(FILE *f, Terrain *t, int *x, int *y) {
  int l, h;   // Dimensions du terrain
  int rx, ry; // Coordonnées initiales du robot
  int lt, pb, robotInex = 0;
  char *result;
  char ligne[DIM_MAX];

  if (f == NULL){return ERREUR_FICHIER;}

  // Lecture de la largeur
  /* À compléter */
  pb = fscanf(f, "%d", &l);
  if (pb == 0){return ERREUR_LECTURE_LARGEUR;} 
  else if (0>l || l>DIM_MAX){return ERREUR_LARGEUR_INCORRECTE;}
  printf("largeur: %d\n", l);
  t->largeur = l;

  // Lecture de la hauteur
  /* À compléter */
  pb = fscanf(f, "%d", &h);
  if (pb == 0){return ERREUR_LECTURE_HAUTEUR;} 
  else if (0>h || h>DIM_MAX){return ERREUR_HAUTEUR_INCORRECTE;}
  printf("hauteur: %d\n", h);
  t->hauteur = h;
  
  // Lecture du terrain
  /* À compléter */
  fgetc(f);
  for(int y = 0; y<h; y++){
    
    result = fgets(ligne, DIM_MAX, f);
    if (result == NULL){return ERREUR_LIGNES_MANQUANTES;} 
    
    lt = strlen(ligne)-1; //Longueur de la ligne lue (-1) pour enlever '\n' 
    if (lt<l){return ERREUR_LIGNE_TROP_COURTE;}
		else if (lt>l){return ERREUR_LIGNE_TROP_LONGUE;}

    for(int x = 0; x<l; x++){
      switch(ligne[x]){
        
        case '.': t->tab[x][y] = LIBRE; break;
        case '#': t->tab[x][y] = ROCHER; break;
        case '~': t->tab[x][y] = EAU; break;
        case 'C': robotInex = 1; t->tab[x][y] = LIBRE; rx = x; ry = y; break;
        
        default: return ERREUR_CARACTERE_INCORRECT;
      } 
    } 
  } 
  printf("\n");

  if (!robotInex){return ERREUR_POSITION_ROBOT_MANQUANTE;}

  // Initialisation de la position du robot
  /* À compléter */
  *x = rx;
  *y = ry;

  return OK;
}

/* À compléter */
int largeur(Terrain *t) { return t->largeur; }

int hauteur(Terrain *t) { return t->hauteur; }

int est_case_libre(Terrain *t, int x, int y) {

  if ((0<=y && y<t->hauteur) && (0<=x && x<t->largeur)) {
    return t->tab[x][y] == LIBRE;
  } else {return 0;}
}

void afficher_terrain(Terrain *t){
  printf("Terrain: \n");

  for(int y = 0; y<t->hauteur; y++){
    for(int x = 0; x<t->largeur; x++){
      switch(t->tab[x][y]){
        
        case LIBRE: printf("."); break;
        case ROCHER: printf("#"); break;
        case EAU: printf("~"); break;
        
        default: break;
      } 
    } 
    printf("\n");
  }
} 

void ecrire_terrain(FILE *f, Terrain *T, int x, int y) {

  if (!f) {perror("Erreur fichier"); return;}
  if (!T) {perror("Erreur terrain"); return;}

  fprintf(f, "%d\n", T->largeur);
  fprintf(f, "%d\n", T->hauteur);
  
  for (int i = 0; i<T->hauteur; i++) {
    for (int j = 0; j<T->largeur; j++) {

      if (i == y && j == x){fprintf(f, "C");} 
      else{
        switch (T->tab[j][i]) {
      
        case LIBRE: fprintf(f, "."); break;
        case ROCHER: fprintf(f, "#"); break;
        case EAU: fprintf(f, "~"); break;
        }
      } 
    }
    fprintf(f, "\n");
  }
}


