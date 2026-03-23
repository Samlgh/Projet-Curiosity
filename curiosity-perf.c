#include "environnement.h"
#include "interprete.h"
#include "programme.h"
#include "generation_terrains.h"

#include <stdio.h>
#include <stdlib.h>

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

    FILE *graine, *fichier_res;
    Terrain T;
    int N=0, L=0, H=0, nb_step_max=0;
    float d=0.0;

    int pas=0, cheminSortie=0;
    int crashes=0, sorties=0, bloque=0;
    float moyennePas=0.0;

    if (argc < 9) {
        printf("Usage: %s <programme> <nombre_terrain> <largeur> <hauteur> "
               "<densité> <graine> <nombre_de_pas> <resFile>\n", argv[0]);
        return 1;
    }



  /* Lecture des parametres avec test*/
  N = (int)strtol(argv[2], NULL, 10);
	L = (int)strtol(argv[3], NULL, 10);
	H = (int)strtol(argv[4], NULL, 10);
	d = (float)strtof(argv[5], NULL);

  if(H%2== 0 || DIM_MAX<H){printf("Hauteur incorrecte\n"); return 1;}
  if(L%2== 0 || DIM_MAX<L){printf("Largeur incorrecte\n"); return 1;}
	if(d<0.0 || 1.0<d){printf("Densité incorrecte\n"); return 1;}

  nb_step_max = (int)strtol(argv[7], NULL, 10);

	fichier_res = fopen(argv[8], "w");
  if (!fichier_res) {perror("Erreur fichier_res"); return 1;}

	fprintf(fichier_res, "Il y a %d terrains\n\n", N);
  int l_centre = (int)L / 2;
  int h_centre = (int)H / 2;


  for(int i = 1; i<=N; i++){

    /* Initialisation de l'environnement : lecture du terrain,
    initialisation de la position du robot */
    printf("%d.Terrain\n", i);

    graine = fopen(argv[6], "w");
    if (!graine) {perror("Erreur graine"); return 1;}
    generation_aleatoire(&T, L, H, d);
    ecrire_terrain(graine, &T, l_centre, h_centre);
    fclose(graine);

    fprintf(fichier_res, "%d.Terrain: ", i);

    errt = initialise_environnement(&envt, argv[6]);
    gestion_erreur_terrain(errt);

    /* Affichage du terrain et du robot */
    afficher_envt(&envt);

    /* Lecture du programme */
    errp = lire_programme(&prog, argv[1]);
    gestion_erreur_programme(errp);

    pas = 0;
    cheminSortie = existe_chemin_vers_sortie(&T);

    if (cheminSortie == 0){

			bloque++;
			printf("Bloqué\n");
			fprintf(fichier_res, "-1\n");

		} else {  
      /* Initialisation de l'état */
      init_etat(&etat);
      do {

        res = exec_pas(&prog, &envt, &etat);
        pas++;
      } while (res==OK_ROBOT && pas<=nb_step_max);

      printf("pas: %d\n", pas);
      printf("resultat: {%u}\n", res);

      /* Affichage du résultat */
      switch (res) {
        case OK_ROBOT:
          printf("Robot sur une case libre, programme non terminé (ne devrait pas "
                 "arriver)\n\n");
          bloque++;
          fprintf(fichier_res, "-1\n");
          break;

        case SORTIE_ROBOT:
          printf("Le robot est sorti :-)\n\n");
          sorties++;
          moyennePas += pas;
          fprintf(fichier_res, "%d\n", pas);
          break;

        case PLOUF_ROBOT:
          printf("Le robot est tombé dans l'eau :-(\n\n");
          crashes++;
          fprintf(fichier_res, "-2\n");
          break;

        case CRASH_ROBOT:
          printf("Le robot s'est écrasé sur un rocher X-(\n\n");
          crashes++;
          fprintf(fichier_res, "-3\n");
          break;
          
        default :
          fprintf(fichier_res, "Erreur: %u\n", res);
          break;
      }
      res = 0;
    }     
  } 
  /* Affichage des statistiques obtenus */
    printf("Nombre et pourcentage de terrains d'où le robot est sorti: %d (%f%%)\n"
            , sorties, (float)sorties/N * 100);
    printf("Nombre et pourcentage de terrains d'où le robot est resté bloqué: %d (%f%%)\n"
            , bloque, (float)bloque/N * 100);
    printf("Nombre et pourcentage de terrains d'où le robot est rentré dans un obstacle: %d (%f%%)\n"
            , crashes, (float)crashes/N * 100);
    printf("Nombre moyen de pas effectués pour les sorties: %f\n"
            , (float)moyennePas/sorties);

    fclose(fichier_res);
}


  
