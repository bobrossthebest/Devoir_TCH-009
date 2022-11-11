/*  programme d'analyse (simple) de trace GPS.
	Auteur: David Marche
	Date : Septembre 2013

	Modification : Abdelmoumène Toudeft
	Date : Septembre 2022

	Références:
		format gpx : http://www.topografix.com/gpx.asp
		GPSBabel: http://www.gpsbabel.org/
		analyseur de trace en ligne: http://www.ibpindex.com/index.asp

	Les données initiales sont celles de fichiers GPX (GPS Exchange) obtenu d'un GPS.
	Pour éviter les difficultés de décodage (parsing) de ce type de fichier,
	le fichier GPX est transformé en fichier CSV (Comma seperated values) avec l'utilitaire gratuit GPSBabel
	Pour faire cette transformation avec GPSBabel: GPX XML --> Universal csv with field structure in first line
	Le fichier CSV obtenu et traité par ce programme contient une ligne d'en tete et les colonnes de données:
		No,Latitude,Longitude,Altitude,Date,Time
		1,49.192273,-123.178443,-44.6,2013/06/21,11:02:00
		2,49.192272,-123.178318,-44.6,2013/06/21,11:02:03
		3,49.192184,-123.178061,-41.7,2013/06/21,11:02:10
		...

	Les coordonnées des points de la traces sont chargées en mémoire.
	Un certain nombre de calculs peuvent ensuite être réalisés sur ces points:
		X Distance parcourue
		X Montée/Descente totale
		X Altitude max/min atteinte
		- Vitesse Maximale
		X Pente Maximale
		- Temps de parcours
		- ..

	En X ceux qui sont réalisés par le programme actuel de base

	Supposition de simplification du problème:
		-un seul segement de trace (ignorer perte de reception sattelite, ...)
		-trace se passe dans une seule journée. Calcul de temps seulement sur Heure,minute,seconde.
		-Le fichier de points doit se trouver dans le répertoire du projet
	Difficulté particulière:
		-Pour le calcul de la pente max, il faut définir une fenetre de moyenne sinon les valeurs sont abérantes.
		-Les valeurs d'ascension totale sont parfois également farfelues sans filtrer ou moyenner
		-Pour tous les calculs impliquant le temps, il faut tenir compte de la date qui peut changer au cours d'une trace.

*/

#define _CRT_SECURE_NO_WARNINGS

// librairies standard
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "config.h"
#include "menu.h"
#include "matrac_lib.h"

// programme principal
int main()
{
	// variables
	int choix;                                 // choix du menu principal
	char nom_fichier[MAXCARNOMFICHIER] = "";   // nom du fichier à traiter
	int nb_point = 0;                          // nombre de points de la trace en mémoire
	double tab_latt[MAXPOINT] = { 0 };         // tableau des valeurs de lattitude
	double tab_long[MAXPOINT] = { 0 };         // tableau des valeurs de longitudes
	double tab_alti[MAXPOINT] = { 0 };         // tableau des valeurs d'altidude
	int taillefenetre = 100;                   // nombre de segments a considérer autour d'un point pour le calcul de sa pente
	double altitude_min, altitude_max;		   // altitudes minimale et maximale
	//Ajoutez toute variable que vous jugerez utile (n'oubliez pas de commenter) :

	// Insérer ici la boucle du menu principal se terminant lorsque l'utilisateur
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
				lire_fichier_gpx(nom_fichier, tab_latt, tab_long, tab_alti, MAXPOINT);
			if (nb_point == -1) {
				printf("\nErreur de chargement de fichier");
			}
			else {
				printf("\n...%d points lus", nb_point);
			}
			break;

		case AFFICHER:
			printf("Donnees en memoire: \n\n");
			printf("index \t\tlattitude\tlongitude\tlaltitude");
			int i;
			for (i = 0; i < nb_point; i++) {
				printf("\t%d\t%lf\t%lf\t%lf", i, tab_latt[i], tab_long[i], tab_alti[i]);
			}
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