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

// pour utiliser DBL_MAX dans pentemax
#include <float.h>

#include "config.h"
#include "matrac_lib.h"

void afficher_tab1D_double(double tab[], int nb_valeur) {
  int i;
  for (i = 0; i < nb_valeur; i++) {
    printf("\n%lf", tab[i]);
  }
}

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

void afficher_trace(double tab_lat[], double tab_long[], double tab_alti[],
                    int taille) {
  // L'en-tete est affichee avant la boucle for pour remplir la consigne
  printf("index \t\tlattitude\tlongitude\taltitude\n");
  int i;
  for (i = 0; i < taille; i++) {
    printf("\t%d\t%lf\t%lf\t%lf\n", i, tab_lat[i], tab_long[i], tab_alti[i]);
  }
}

double deg2rad(double d) { return PI / 180 * d; }

double distance_entre_2_points(double lat1, double lon1, double lat2,
                               double lon2, double alt1, double alt2) {
  // Distance sans prendre en compte l'altitude
  double distance =
      RAYONTERRE * 2 *
      asin(sqrt(pow(sin((lat1 - lat2) / 2), 2) +
                cos(lat1) * cos(lat2) * pow(sin((lon1 - lon2) / 2), 2)));
  // Pythagore pour prendre en compte l'altitude
  distance = sqrt(pow(distance, 2) + pow(alt2 - alt1, 2));
  return distance;
}

double distance_totale(double tab_lat[], double tab_long[], double tab_alti[],
                       int taille, int debut, int fin) {
  double dist_tot = 0;
  int i;
  // Si debut ou fin est invalide, la boucle est sautee et dist_tot reste 0
  if (!(debut < 0 || debut >= taille || fin < 0 || fin >= taille)) {
    /* Pour prendre en compte jusqu'au point b, il faut arreter le calcul a b-1
     puisque la distance est celle entre le point et le point ulterieur.*/

    for (i = debut; i < fin; i++) {
      dist_tot += distance_entre_2_points(tab_lat[i], tab_long[i],
                                          tab_lat[i + 1], tab_long[i + 1],
                                          tab_alti[i], tab_alti[i + 1]);
    }
  }

  return dist_tot;
}

double ascension(double tab_alti[], int taille) {
  double ascension_tot = 0;
  int i;
  // Si taille est egale ou infieure a 0, la condition du for n'est pas remplie.
  for (i = 0; i < taille; i++) {
    if (tab_alti[i + 1] >= tab_alti[i]) {
      ascension_tot += tab_alti[i + 1] - tab_alti[i];
    }
  }
  return ascension_tot;
}

double pentemax(double tab_lat[], double tab_long[], double tab_alti[],
                int taille, int taille_fenetre) {
  double pentemax = 0, pente_i;
  int i;
  /* Avant que (taille_fenetre > 0) soit confirmee, pentemax == 0
  La condition de taille est redondante avec la boucle for, mais je n'ai pas
  trouve de meilleur moyen de retourner 0 si taille est invalide ET de prendre
  en compte des boucles toutes negatives.
*/
  if ((taille_fenetre > 0) && (taille > 2 * taille_fenetre)) {
    // pour eviter que des pentes toutes negatives retournent 0
    pentemax = -DBL_MAX;
    // i < taille-taille_fenetre car taille commence a 1, les indices commencent
    // a 0.
    for (i = taille_fenetre; i < taille - taille_fenetre; i++) {
      // pour eviter des divisions par 0
      if (distance_totale(tab_lat, tab_long, tab_alti, taille,
                          i - taille_fenetre, i + taille_fenetre) > 0) {
        pente_i =
            (tab_alti[i + taille_fenetre] - tab_alti[i - taille_fenetre]) /
            distance_totale(tab_lat, tab_long, tab_alti, taille,
                            i - taille_fenetre, i + taille_fenetre);
        // Cette verification n'a pas de sens si pente_i n'a pas ete redefini
        if (pente_i > pentemax) {
          pentemax = pente_i;
        }
      }
    }
  }

  return pentemax;
}

int valeurmin_tab(double tab[], int taille, double *valmin) {
  /*La variable validite est 0 tant que la condition (taille > 0) n'est pas
    remplie.*/
  int i, validite = 0;
  if (taille > 0) {
    validite = 1;
    // pour comparer a une valeur existante, tab[0] est assignee a *valmin.
    *valmin = tab[0];
    // compare avec toutes les valeurs du tableau, remplace si valeur<*valmin
    for (i = 1; i < taille; i++) {
      if (tab[i] < *valmin) {
        *valmin = tab[i];
      }
    }
  }
  // pour donner une valeur nulle a *valmin si la taille est invalide
  else {
    *valmin = 0;
  }
  return validite;
}

// Meme demarche que pour valeurmin_tab
int valeurmax_tab(double tab[], int taille, double *valmax) {
  int i, validite = 0;
  if (taille > 0) {
    validite = 1;
    *valmax = tab[0];
    for (i = 1; i < taille; i++) {
      if (tab[i] > *valmax) {
        *valmax = tab[i];
      }
    }
  } else {
    *valmax = 0;
  }
  return validite;
}

// impl�mentations des fonctions de matrac_lib (c.f. prototype et doc dans
// matrac_lib.h)
