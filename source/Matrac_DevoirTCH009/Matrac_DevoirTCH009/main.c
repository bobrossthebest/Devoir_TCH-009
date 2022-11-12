/*  programme d'analyse (simple) de trace GPS.
        Auteur: David Marche
        Date : Septembre 2013

        Modification : Abdelmoum�ne Toudeft
        Date : Septembre 2022

        R�f�rences:
                format gpx : http://www.topografix.com/gpx.asp
                GPSBabel: http://www.gpsbabel.org/
                analyseur de trace en ligne: http://www.ibpindex.com/index.asp

        Les donn�es initiales sont celles de fichiers GPX (GPS Exchange) obtenu
   d'un GPS. Pour �viter les difficult�s de d�codage (parsing) de ce type de
   fichier, le fichier GPX est transform� en fichier CSV (Comma seperated
   values) avec l'utilitaire gratuit GPSBabel Pour faire cette transformation
   avec GPSBabel: GPX XML --> Universal csv with field structure in first line
        Le fichier CSV obtenu et trait� par ce programme contient une ligne d'en
   tete et les colonnes de donn�es: No,Latitude,Longitude,Altitude,Date,Time
                1,49.192273,-123.178443,-44.6,2013/06/21,11:02:00
                2,49.192272,-123.178318,-44.6,2013/06/21,11:02:03
                3,49.192184,-123.178061,-41.7,2013/06/21,11:02:10
                ...

        Les coordonn�es des points de la traces sont charg�es en m�moire.
        Un certain nombre de calculs peuvent ensuite �tre r�alis�s sur ces
   points: X Distance parcourue X Mont�e/Descente totale X Altitude max/min
   atteinte
                - Vitesse Maximale
                X Pente Maximale
                - Temps de parcours
                - ..

        En X ceux qui sont r�alis�s par le programme actuel de base

        Supposition de simplification du probl�me:
                -un seul segement de trace (ignorer perte de reception
   sattelite, ...) -trace se passe dans une seule journ�e. Calcul de temps
   seulement sur Heure,minute,seconde. -Le fichier de points doit se trouver
   dans le r�pertoire du projet Difficult� particuli�re: -Pour le calcul de la
   pente max, il faut d�finir une fenetre de moyenne sinon les valeurs sont
   ab�rantes. -Les valeurs d'ascension totale sont parfois �galement farfelues
   sans filtrer ou moyenner -Pour tous les calculs impliquant le temps, il faut
   tenir compte de la date qui peut changer au cours d'une trace.

*/

/*
  Modification: Julien Groulx, Zachary Fillion, Karl Aidans, Justin Landry.
  Date: Novembre 2022
*/

#define _CRT_SECURE_NO_WARNINGS

// librairies standard
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "matrac_lib.h"
#include "menu.h"

// programme principal
int main() {
  // variables
  int choix;                               // choix du menu principal
  char nom_fichier[MAXCARNOMFICHIER] = ""; // nom du fichier � traiter
  int nb_point = 0;                // nombre de points de la trace en m�moire
  double tab_lat[MAXPOINT] = {0};  // tableau des valeurs de lattitude
  double tab_long[MAXPOINT] = {0}; // tableau des valeurs de longitudes
  double tab_alti[MAXPOINT] = {0}; // tableau des valeurs d'altidude
  int taillefenetre = 100; // nombre de segments a consid�rer autour d'un point
                           // pour le calcul de sa pente
  double altitude_min, altitude_max; // altitudes minimale et maximale
  // Ajoutez toute variable que vous jugerez utile (n'oubliez pas de commenter)
  // :

  // Ins�rer ici la boucle du menu principal se terminant lorsque l'utilisateur
  // choisit l'option Quitter :

  do {
    choix = afficher_menu();

    switch (choix) {
    case CHARGER:
      printf("Entrez le nom du fichier a charger: ");
      fflush(stdin);
      scanf("%s", nom_fichier);
      printf("\n\nChargement du fichier...");
      nb_point =
          lire_fichier_gpx(nom_fichier, tab_lat, tab_long, tab_alti, MAXPOINT);
      if (nb_point == -1) {
        printf("\nErreur de chargement de fichier");
      } else {
        printf("\n...%d points lus", nb_point);
      }
      break;

    case AFFICHER:
      printf("Donnees en memoire: \n\n");
      afficher_trace(tab_lat, tab_long, tab_alti, nb_point);
      break;

    case ANALYSER:
      break;

    case PENTEMAX:
      break;
    }

  } while (choix != QUITTER);

  // fin du programme
  return EXIT_SUCCESS;
}