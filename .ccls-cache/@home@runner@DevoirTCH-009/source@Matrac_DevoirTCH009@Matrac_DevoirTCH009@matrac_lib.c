/*
Auteur(s):
Date:

Ce module contient les implémentations des fonctions
d'analyse de traces GPS

*/

#define _CRT_SECURE_NO_WARNINGS

// librairies standard
#include <stdio.h>
#include <math.h>

#include "config.h"
#include "matrac_lib.h"

/* fonction: lire_fichier_gpx
	auteur: David Marche
	description: lit un fichier de données GPS au format suivant
		No,Latitude,Longitude,Altitude,Date,Time
		1,49.192273,-123.178443,-44.6,2013/06/21,11:02:00
		2,49.192272,-123.178318,-44.6,2013/06/21,11:02:03
		3,49.192184,-123.178061,-41.7,2013/06/21,11:02:10
		...

		place toutes les données de lattitude, longitude, altitude et temps dans des tableau statiques fournit en paramètre

	Retour: nombre de points lus, 0 siaucun, -1 si problème d'ouverture/lecture de fichier,
	Note: les tableaux données en paramètre sont modifiés
*/
int lire_fichier_gpx(char nom_fichier[], double tab_latt[], double tab_long[], double tab_alti[], int taillemax) 
{

	int nb_point = 0;                 // nombre de point trouvés
	char ligne[MAXCARLIGNE];        // chaine de caractère contenant la balise
	FILE* fp;                       // pointeur au fichier à lire
	int no, an, mois, jour, h, m, s;      // info lues du fichier
	double latt, lon, alti;

	// ouverture du fichier et vérification du succès de l'ouverture
	fp = fopen(nom_fichier, "r"); // en mode lecture seulement
	if (fp == NULL) nb_point = -1;
	else {

		// sauter l'en-tête
		if (fgets(ligne, MAXCARLIGNE - 1, fp) == NULL) nb_point = -1;

		// continuer la lecture si fichier n'est pas vide
		else {

			// boucle de lecture principale de chaque point (un par ligne)
			while ((fgets(ligne, MAXCARLIGNE - 1, fp) != NULL) && (nb_point < taillemax)) {
				// lecture d'un point et identification des 10 champs attendus:
				//  No,Latitude,Longitude,Altitude,Date(a/m/j),Time(h:m:s)
				if (sscanf(ligne, "%d,%lf,%lf,%lf,%d/%d/%d,%d:%d:%d", &no, &latt, &lon, &alti, &an, &mois, &jour, &h, &m, &s) == 10) {

					//si decodage correct,mise en mémoire de la lattitude, longitude,altitude
					tab_latt[nb_point] = latt;
					tab_long[nb_point] = lon;
					tab_alti[nb_point] = alti;

					//printf("\nindex:%d latt:%10f long:%10f alti:%10f ",nb_point,tab_latt[nb_point],tab_long[nb_point],tab_alti[nb_point],);
					// incrementer le nombre de points correctement lus
					nb_point++;
				}
			}
		}
		// fermeture du fichier
		fclose(fp);
	}

	//renvoi du nb de points trouvés
	return nb_point;

}

// implémentations des fonctions de matrac_lib (c.f. prototype et doc dans matrac_lib.h)
