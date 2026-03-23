#include "environnement.h"
#include "interprete.h"
#include "programme.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void gestion_erreur_terrain(erreur_terrain e) {
  switch (e) {
  case OK:
    break;
  case ERREUR_FICHIER:
    printf("Erreur lecture du terrain : erreur d'ouverture du fichier\n");
    exit(1);
  case ERREUR_LECTURE_LARGEUR:
    printf("Erreur lecture du terrain : erreur de lecture de la largeur\n");
    exit(1);
  case ERREUR_LECTURE_HAUTEUR:
    printf("Erreur lecture du terrain : erreur de lecture de la hauteur\n");
    exit(1);
  case ERREUR_LARGEUR_INCORRECTE:
    printf("Erreur lecture du terrain : largeur incorrecte\n");
    exit(1);
  case ERREUR_HAUTEUR_INCORRECTE:
    printf("Erreur lecture du terrain : hauteur incorrecte\n");
    exit(1);
  case ERREUR_CARACTERE_INCORRECT:
    printf("Erreur lecture du terrain : caractère incorrect\n");
    exit(1);
  case ERREUR_LIGNE_TROP_LONGUE:
    printf("Erreur lecture du terrain : ligne trop longue\n");
    exit(1);
  case ERREUR_LIGNE_TROP_COURTE:
    printf("Erreur lecture du terrain : ligne trop courte\n");
    exit(1);
  case ERREUR_LIGNES_MANQUANTES:
    printf("Erreur lecture du terrain : lignes manquantes\n");
    exit(1);
  case ERREUR_POSITION_ROBOT_MANQUANTE:
    printf(
        "Erreur lecture du terrain : position initiale du robot manquante\n");
    exit(1);
  }
}

void affichage_position_programme(erreur_programme e) {
  int i;
  printf("Ligne %d, colonne %d :\n", e.num_ligne, e.num_colonne);
  printf("%s\n", e.ligne);
  /* Impression de e.num_colonne-1 espaces */
  for (i = 1; i < e.num_colonne; i++) {
    printf(" ");
  }
  /* Impression d'un curseur de position */
  printf("^\n");
}

void gestion_erreur_programme(erreur_programme e) {
  switch (e.type_err) {
  case OK_PROGRAMME:
    break;
  case ERREUR_FICHIER_PROGRAMME:
    printf("Erreur lecture du programme : erreur d'ouverture du fichier\n");
    exit(2);
  case ERREUR_BLOC_NON_FERME:
    printf("Erreur lecture du programme : bloc non fermé\n");
    exit(2);
  case ERREUR_FERMETURE_BLOC_EXCEDENTAIRE:
    printf("Erreur lecture du programme : fermeture de bloc excédentaire\n");
    affichage_position_programme(e);
    exit(2);
  case ERREUR_COMMANDE_INCORRECTE:
    printf("Erreur lecture du programme : commande incorrecte\n");
    affichage_position_programme(e);
    exit(2);
  }
}

int main(int argc, char **argv) {
    Environnement envt;
    Programme prog;
    erreur_terrain errt;
    erreur_programme errp;
    etat_inter etat;
    resultat_inter res;

    FILE *f;
    char ligne[100]; 
    int maxPas, attX, attY, robX, robY;
    char attO, robO, event;

    if (argc < 2) {
        printf("Usage: %s <fichier>\n", argv[0]);
        return 1;
    }

    f = fopen(argv[1], "r");
    
    /* Initialisation de l'environnement : lecture du terrain,
        initialisation de la position du robot */
    fgets(ligne, 100, f);
    ligne[strlen(ligne) - 1] = '\0';
    errt = initialise_environnement(&envt, ligne);
    gestion_erreur_terrain(errt);

    /* Lecture du programme */
    fgets(ligne, 100, f);
    ligne[strlen(ligne) - 1] = '\0';
    errp = lire_programme(&prog, ligne);
    gestion_erreur_programme(errp);

    /* Recuperation des composants du fichier*/
    fscanf(f, "%d", &maxPas);
    fscanf(f, "\n%c", &event);

    fscanf(f, "%d", &attX); 
    fscanf(f, "%d", &attY);
    fscanf(f, "\n%c", &attO);

    /* Initialisation de l'état */
    init_etat(&etat);
    do {
        res = exec_pas(&prog, &envt, &etat);
        /* Affichage du terrain et du robot */
        afficher_envt(&envt);
        
        maxPas--;
    } while (res == OK_ROBOT && maxPas!=0);

    /* Affichage du résultat par rapport au resultat obtenu*/
    switch (res) {
      case SORTIE_ROBOT:
          if(event=='S'){printf("----- Ok -----\n");} 
          else{printf("----- Pb -----\n");} 
          printf("Le robot est sorti :-)\n");
          break;

      case ARRET_ROBOT: case OK_ROBOT:
          if(event=='N' ||  event=='F'){
              position(&envt.r, &robX, &robY);
              switch (orient(&envt.r)){
                  
                  case Nord: robO = 'N'; break;
                  case Sud: robO = 'S'; break;
                  case Est: robO = 'E'; break;
                  case Ouest: robO = 'O'; break;
              }
              if (robO == attO && robX == attX && robY == attY){
                  printf("----- Ok -----\n");
                  printf("attendue: %d, %d, %c\n", attX, attY, attO);
              } 
              else{
                  printf("----- Pb -----\n");
                  printf("attendue: %d, %d, %c\n", attX, attY, attO);
              }   
          } 
          else{printf("Robot sur une case libre, programme terminé :-/\n");}      
          break;

      case PLOUF_ROBOT:
          if(event=='P'){printf("----- Ok -----\n");} 
          else{printf("----- Pb -----\n");} 
          printf("Le robot est tombé dans l'eau :-(\n");
          break;

      case CRASH_ROBOT:
          if(event=='O'){printf("----- Ok -----\n");} 
          else{printf("----- Pb -----\n");} 
          printf("Le robot s'est écrasé sur un rocher X-(\n");
          break;

      case ERREUR_PILE_VIDE: printf("ERREUR : pile vide\n"); break;
      case ERREUR_ADRESSAGE: printf("ERREUR : erreur d'adressage\n"); break;
      case ERREUR_DIVISION_PAR_ZERO: printf("ERREUR : division par 0\n"); break;
    }
  fclose(f);
}