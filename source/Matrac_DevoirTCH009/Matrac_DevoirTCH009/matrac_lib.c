/*
Auteur(s): Julien Groulx, Zachary Fillion, Karl Aidans, Justin Landry
Date: 11 novembre 2022

Ce module contient les impl�mentations des fonctions
d'analyse de traces GPS

*/

#define _CRT_SECURE_NO_WARNINGS

// librairies standard
#include <math.h>
#include <stdio.h>

#include "config.h"
#include "matrac_lib.h"

/* --------------------------------------------------------------
        fonction: afficher_tab1D_double
        Auteur:
        description: affiche le contenu d'un tableau 1D de double (1 valeur par
   ligne) Param�tres: -tab: le tableau de valeurs (type double) -nb_valeur: le
   nombre de valuer � afficher Retour: rien Note: le tableau n'est pas modifi�.
                Aucune v�rification sur la validit� de nb_valeur n'est
   effectu�e(ex: nb_valeur>taille du tableau)
*/
void afficher_tab1D_double(double tab[], int nb_valeur) {
  int i;
  for (i = 0; i < nb_valeur; i++) {
    printf("\n%lf", tab[i]);
  }
}

/* fonction: lire_fichier_gpx
        auteur: David Marche
        description: lit un fichier de donn�es GPS au format suivant
                No,Latitude,Longitude,Altitude,Date,Time
                1,49.192273,-123.178443,-44.6,2013/06/21,11:02:00
                2,49.192272,-123.178318,-44.6,2013/06/21,11:02:03
                3,49.192184,-123.178061,-41.7,2013/06/21,11:02:10
                ...

                place toutes les donn�es de lattitude, longitude, altitude et
   temps dans des tableau statiques fournit en param�tre

        Retour: nombre de points lus, 0 siaucun, -1 si probl�me
   d'ouverture/lecture de fichier, Note: les tableaux donn�es en param�tre sont
   modifi�s
*/
int lire_fichier_gpx(char nom_fichier[], double tab_latt[], double tab_long[],
                     double tab_alti[], int taillemax) {

  int nb_point = 0;                // nombre de point trouv�s
  char ligne[MAXCARLIGNE];         // chaine de caract�re contenant la balise
  FILE *fp;                        // pointeur au fichier � lire
  int no, an, mois, jour, h, m, s; // info lues du fichier
  double latt, lon, alti;

  // ouverture du fichier et v�rification du succ�s de l'ouverture
  fp = fopen(nom_fichier, "r"); // en mode lecture seulement
  if (fp == NULL)
    nb_point = -1;
  else {

    // sauter l'en-t�te
    if (fgets(ligne, MAXCARLIGNE - 1, fp) == NULL)
      nb_point = -1;

    // continuer la lecture si fichier n'est pas vide
    else {

      // boucle de lecture principale de chaque point (un par ligne)
      while ((fgets(ligne, MAXCARLIGNE - 1, fp) != NULL) &&
             (nb_point < taillemax)) {
        // lecture d'un point et identification des 10 champs attendus:
        //  No,Latitude,Longitude,Altitude,Date(a/m/j),Time(h:m:s)
        if (sscanf(ligne, "%d,%lf,%lf,%lf,%d/%d/%d,%d:%d:%d", &no, &latt, &lon,
                   &alti, &an, &mois, &jour, &h, &m, &s) == 10) {

          // si decodage correct,mise en m�moire de la lattitude,
          // longitude,altitude
          tab_latt[nb_point] = latt;
          tab_long[nb_point] = lon;
          tab_alti[nb_point] = alti;

          // printf("\nindex:%d latt:%10f long:%10f alti:%10f
          // ",nb_point,tab_latt[nb_point],tab_long[nb_point],tab_alti[nb_point],);
          // incrementer le nombre de points correctement lus
          nb_point++;
        }
      }
    }
    // fermeture du fichier
    fclose(fp);
  }

  // renvoi du nb de points trouv�s
  return nb_point;
}

/* --------------------------------------------------------------
        fonction: afficher_trace
        auteur: Julien Groulx
        description:
                afficher toutes les donn�es de la trace pr�sente en m�moire avec
   la premi�re ligne d'en-t�te suivante: "index      lattitude     longitude
   altitude" Param�tres: tableaux d'infos (tab_lat,tab_long,tab_alti) et le
   nombre de point (taille) tab_lat:        tableau des valeurs de latitude
                tab_long:       tableau des valeurs de longitude
                tab_alti:       tableau des valeurs d'altitude
                taille:         nombre de valeurs valides pr�sentes dans les
   tableaux Retour: rien Cas particuliers: Quand la taille est nulle ou
   n�gative, seule la ligne d'en-t�te est affich�e.
*/
void afficher_trace(double tab_lat[], double tab_long[], double tab_alti[],
                    int taille) {
  printf("index \t\tlattitude\tlongitude\taltitude\n");
  int i;
  for (i = 0; i < taille; i++) {
    printf("\t%d\t%lf\t%lf\t%lf\n", i, tab_lat[i], tab_long[i], tab_alti[i]);
  }
}

/* --------------------------------------------------------------
        fonction: deg2rad
        auteur: Julien Groulx
        description: convertir en radians une valeur d'angle donn�e initialement
   en degr�s Param�tres: -d: la valeur en degr�s Retour: la valeur
   correspondante en radians Cas particuliers: aucun
*/
double deg2rad(double d) {
  double rad;
  rad = PI / 180 * d;
  return rad;
}

/* --------------------------------------------------------------
        fonction: distance_entre_2_points
        auteur: Julien Groulx
        description:
                Calculer la distance entre 2 points d�crits par leurs lattitude,
                longitue et altitude
        Param�tres:
                lat1,lon1,atl1: lattitude, longitude et altitude du premier
   point lat2,lon2,atl2: lattitude, longitude et altitude du second point

        Note:
                Utilise une constante d�finissant le rayon de la terre
   (RAYONTERRE) en km Utilise des fonctions de la librairie math.h (asin, sin,
   cos, pow, sqrt) V�rifier que cette constante est bien d�finie dans
   matrac_lib.h et que la librairie math.h est bien inclue dans matrac_lib.c
        Retour: la distance calcul�e en km
        Source:
                Calcul inspir� d'une formule de www.partir_en_vtt.com.
                Cette formule est d�crite en annexe de l'�nonc� du devoir.
*/
double distance_entre_2_points(double lat1, double lon1, double lat2,
                               double lon2, double alt1, double alt2) {
  double distance =
      RAYONTERRE * 2 *
      asin(sqrt(pow(sin((lat1 - lat2) / 2), 2) +
                cos(lat1) * cos(lat2) * pow(sin((lon1 - lon2) / 2), 2)));
  distance = sqrt(pow(distance, 2) + pow(alt2 - alt1, 2));
  return distance;
}

/* --------------------------------------------------------------
        fonction: distance_totale
        auteur:
        description:
                Calculer la distance parcourue par une trace d�finie par des
   tableaux avec les information GPS de tous les points de la trace. debut et
   fin permettent de pr�ciser les index des points entre lesquels la distance
   doit �tre calcul�e. si d�but=0 et fin=taille-1, c'est la distance totale qui
   est calcul�e Param�tres: tab_lat:        tableau des valeurs de latitude
                tab_long:       tableau des valeurs de longitude
                tab_alti:       tableau des valeurs d'altitude
                taille:         nombre de valeurs valides pr�sentes dans les
   tableaux debut:          index du premier point gps � prendre en compte dans
   le calcul fin:            index du dernier point gps � prendre en compte dans
   le calcul Retour: la distance calcul�e en km Cas particuliers: si un des deux
   index est invalide (plus petit que 0 ou plus grand que taille) la distance
   renvoy�e est 0.

*/
double distance_totale(double tab_lat[], double tab_long[], double tab_alti[],
                       int taille, int debut, int fin) {
  double dist_tot = 0;
  int i;

  if (!(debut < 0 || debut >= taille || fin < 0 || fin >= taille)) {
    for (i = debut; i <= fin; i++) {
      dist_tot += distance_entre_2_points(tab_lat[i], tab_long[i],
                                          tab_lat[i + 1], tab_long[i + 1],
                                          tab_alti[i], tab_alti[i + 1]);
    }
  }

  return dist_tot;
}

/* --------------------------------------------------------------
        fonction: ascension
        auteur: Julien Groulx
        description:
                calculer le total de mont�e le long d'une trace
                les information d'altitude des points de la trace sont fournies
                dans un tableau en param�tre (tab_alti)
                Le calcul se fait en ajoutant les contribution de gain
   d'altitude de chacun des segments de la trace. Les segments qui sont en
   descente (perte d'altitude) sont ignor�s param�tres: tab_alti:   tableau des
   valeurs d'altitudes taille:     nombre de points valides pr�sent dans le
   tableau d'altitude Retour: l'ascension totale calcul�e (en m�tres) Cas
   particulier: si la valeur de taille nulle ou n�gative, la valeur renvoy�e est
   0 Aucune v�rification sur le fait que taille puisse d�passer le nombre de
   donn�es du tableau
*/
double ascension(double tab_alti[], int taille) {
  double ascension_tot = 0;
  int i;
  for (i = 0; i < taille; i++) {
    if (tab_alti[i + 1] >= tab_alti[i]) {
      ascension_tot += tab_alti[i + 1] - tab_alti[i];
    }
  }
  return ascension_tot;
}

/* --------------------------------------------------------------
        fonction: pentemax
        auteur:
        description:
                fonction de calcul de pente maximale de la trace.
                Pour diminuer l'effet des variations locale
                (ex: une personne se met debout avec le GPS dans la main)
                la pente est obtenu par une moyenne glissante
                prenant toujours en compte taillefenetre segments de chaque c�t�
   du point de calcul

        ex. de points utilis�s pour le calcul de variation d'altitude avec
   taillefenetre=5: -pente aux points 0-4: pas d�fini -pente au point 5 est la
   pente moyenne de 5 segments avant le point 5 et de 5 segments apr�s le point
   5 (points 0-10) -pente au point 6 est la pente moyenne de 5 segments avant le
   point 6 et de 5 segments apr�s le point 6 (points 0-5)
                -...
                -pente au point x est la pente moyenne de 5 segments avant le
   point x (point (x-5) � x) et de 5 segments apr�s le point x (point x � (x+5)
                -pente des 4 derniers points pas d�finis

        Param�tres:
                tab_lat:        tableau des valeurs de latitude
                tab_long:       tableau des valeurs de longitude
                tab_alti:       tableau des valeurs d'altitude
                taille:         nombre de valeurs valides pr�sentes dans les
   tableaux taille_fenetre: nombre de segments de trace a consid�rer avant et
   apr�s chaque point de calcul de pente

        Note:
                Attention la distance parcourue doit �tre calcul�e en suivant la
   trace avec la fonction distance_totale Si la distance entre 2 points est
   nulle, on suppose que la pente est nulle pour �viter une division par 0.

        Retour:
                la pente maximale calcul�e
        Cas particuliers:
                Si taille_fenetre est invalide (valeur nulle ou n�gative) la
   fonction renvoie 0 Si taille_fenetre est trop grand et qu'il n'y a pas assez
   de points disponibles dans les tableaux de donn�es, la fonction renvoi 0
*/
double pentemax(double tab_lat[], double tab_long[], double tab_alti[],
                int taille, int taille_fenetre) {
  double pentemax = 0, pente_i;
  int i;
  if (taille_fenetre > 0) {
    for (i = taille_fenetre; i < taille - taille_fenetre; i++) {
      pente_i = (tab_alti[i + taille_fenetre] - tab_alti[i - taille_fenetre]) /
                distance_totale(tab_lat, tab_long, tab_alti, taille,
                                i - taille_fenetre, i + taille_fenetre);
      if (pente_i > pentemax) {
        pentemax = pente_i;
      }
    }
  }
  return pentemax;
}

/*      fonction: valeurmin_tab
      auteur: Julien Groulx
      description: rechercher la valeur minimale d'un tableau de
 r�elsvaleurmin_tab param�tres: taille: nombre de points valides pr�sent dans
 le tableau tab:    tableau des valeurs dans lequel la recherche du minimum
 doit se faire valmax: valeur minimale trouv�e dans le tableau (re�u par
 adresse) Retour: 1 en cas de r�sultat valide, 0 sinon (taille<=0) Note: la
 valeur min est stock�e dans valmin qui est transmis par adresse Cas
 particulier: si taille est nulle ou n�gative, la valeur renvoy�e est 0 et
 valmin est fix� � 0
*/
int valeurmin_tab(double tab[], int taille, double *valmin) {
  int i, validite = 0;
  if (taille > 0) {
    validite = 1;
    *valmin = tab[0];
    for (i = 1; i < taille; i++) {
      if (tab[i]<*valmin)
      {*valmin=tab[i];}
    }
  } else {
    *valmin = 0;
  }
  return validite;
}

/* --------------------------------------------------------------
        fonction: valeurmax_tab
        auteur:
        description: rechercher la valeur maximale d'un tableau de r�els
                param�tres:
                taille: nombre de points valides pr�sent dans le tableau
                tab:    tableau des valeurs dans lequel la recherche du maximum
   doit se faire valmax: valeur maximale trouv�e dans le tableau (re�u par
   adresse)

        Retour:
                1 en cas de r�sultat valide, 0 sinon (taille<=0)
        Note:
                la valeur max est stock�e dans valmax qui est transmis par
   adresse Cas particulier: si taille est nulle ou n�gative, la valeur renvoy�e
   est 0 et valmax est fix� � 0
*/

int valeurmax_tab(double tab[], int taille, double *valmax);

// impl�mentations des fonctions de matrac_lib (c.f. prototype et doc dans
// matrac_lib.h)
