/*
Auteur: David Marche
Date: Automne 2016

Ce module contient les constantes et prototypes des fonctions
d'analyse de traces GPS

*/


#ifndef MATRAC_LIB_H    // pour éviter d'inclure plusieur fois le même module
#define MATRAC_LIB_H


//constantes physiques
#define PI  3.14159
#define RAYONTERRE 6366     // rayon de la terre (en km) pour les calculs de distances

// prototypes

/* --------------------------------------------------------------
	fonction: afficher_tab1D_double
	Auteur:
	description: affiche le contenu d'un tableau 1D de double (1 valeur par ligne)
	Paramètres:
		-tab: le tableau de valeurs (type double)
		-nb_valeur: le nombre de valuer à afficher
	Retour: rien
	Note:
		le tableau n'est pas modifié.
		Aucune vérification sur la validité de nb_valeur n'est effectuée(ex: nb_valeur>taille du tableau)
*/
void afficher_tab1D_double(double tab[], int nb_valeur);

/* --------------------------------------------------------------
	fonction: lire_fichier_gpx
	auteur:Charger la trace test1.txt  (fonction charger_trace)
Afficher les données de la trace (afficher_trace)
 David Marche
	description: lit un fichier de données GPS au format suivant
		No,Latitude,Longitude,Altitude,Date,Time
		1,49.192273,-123.178443,-44.6,2013/06/21,11:02:00
		2,49.192272,-123.178318,-44.6,2013/06/21,11:02:03
		3,49.192184,-123.178061,-41.7,2013/06/21,11:02:10
		...

		place toutes les données de lattitude, longitude, altitude et temps
		dans des tableaux statiques fournis en paramètres
	Paramètres:
		nom_fichier:    chaîne de caractères contenant le nom du fihier à lire
		tab_lat:        tableau à remplir avec les valeurs de latitude
		tab_long:       tableau à remplir avec les valeurs de longitude
		tab_alti:       tableau à remplir avec les valeurs d'altitude
		taille_max:     nombre maximum de valeurs pouvant être chargées dans les tableaux

	Retour:
		nombre de points lus, 0 si aucun, -1 si problème d'ouverture/lecture de fichier,
	Note:
		les tableaux données en paramètre sont modifiés.
		Les valeurs de temps présentes dans le fichier sont ignorés.
	Cas particuliers:
		Si le fichier ne peut pas être ouvert la fonction renvoie -1
		Si la ligne d'en tête n'est pas présente en première ligne du fichier,
		la fonction renvoie -1
		Si le nombre de valeurs du fichier dépasse la capacité du tableau,
		les valeur en trop sont ignorées
*/
int lire_fichier_gpx(char nom_fichier[], double tab_lat[], double tab_long[], double tab_alti[], int taillemax);


/* --------------------------------------------------------------
	fonction: afficher_trace
	auteur:
	description:
		afficher toutes les données de la trace présente en mémoire avec la première
		ligne d'en-tête suivante: "index      lattitude     longitude   altitude"
	Paramètres: tableaux d'infos (tab_lat,tab_long,tab_alti) et le nombre de point (taille)
		tab_lat:        tableau des valeurs de latitude
		tab_long:       tableau des valeurs de longitude
		tab_alti:       tableau des valeurs d'altitude
		taille:         nombre de valeurs valides présentes dans les tableaux
	Retour: rien
	Cas particuliers:
		Quand la taille est nulle ou négative, seule la ligne d'en-tête est affichée.
*/
void afficher_trace(double tab_lat[], double tab_long[], double tab_alti[], int taille);

/* --------------------------------------------------------------
	fonction: deg2rad
	auteur:
	description: convertir en radians une valeur d'angle donnée initialement en degrés
	Paramètres:
		-d: la valeur en degrés
	Retour: la valeur correspondante en radians
	Cas particuliers: aucun
*/
double deg2rad(double d);

/* --------------------------------------------------------------
	fonction: distance_entre_2_points
	auteur:
	description:
		Calculer la distance entre 2 points décrits par leurs lattitude,
		longitue et altitude
	Paramètres:
		lat1,lon1,atl1: lattitude, longitude et altidute du premier point
		lat2,lon2,atl2: lattitude, longitude et altidute du second point

	Note:
		Utilise un constante définissant le rayon de la terre (RAYONTERRE) en km
		Utilise des fonctions de la librairie math.h (asin, sin, cos, pow, sqrt)
		Vérifier que cette constante est bien définie dans matrc_lib.h
		et que la librairie math.h est bien inclue dans matrac_lib.c
	Retour: la distance calculée en km
	Source:
		Calcul inspiré d'une formule de www.partir_en_vtt.com.
		Cette formule est décrite en annexe de l'énoncé du devoir.
*/
double distance_entre_2_points(double lat1, double lon1, double lat2, double lon2, double alt1, double alt2);

/* --------------------------------------------------------------
	fonction: distance_totale
	auteur:
	description:
		Calculer la distance parcourue par une trace définie par des tableaux avec
		les information GPS de tous les points de la trace. debut et fin permettent de
		préciser les index des points entre lesquels la distance doit être calculée.
		si début=0 et fin=taille-1, c'est la distance totale qui est calculée
	Paramètres:
		tab_lat:        tableau des valeurs de latitude
		tab_long:       tableau des valeurs de longitude
		tab_alti:       tableau des valeurs d'altitude
		taille:         nombre de valeurs valides présentes dans les tableaux
		debut:          index du premier point gps à prendre en compte dans le calcul
		fin:            index du dernier point gps à prendre en compte dans le calcul
	Retour: la distance calculée en km
	Cas particuliers:
		si un des deux index est invalide (plus petit que 0 ou plus grand que taille)
		la distance renvoyée est 0.

*/
double distance_totale(double tab_lat[], double tab_long[], double tab_alti[], int taille, int debut, int fin);

/* --------------------------------------------------------------
	fonction: ascension
	auteur:
	description:
		calculer le total de montée le long d'une trace
		les information d'altitude des points de la trace sont fournies
		dans un tableau en paramètre (tab_alti)
		Le calcul se fait en ajoutant les contribution de gain d'altitude
		de chacun des segments de la trace.
		Les segments qui sont en descente (perte d'altitude) sont ignorés
	paramètres:
		tab_alti:   tableau des valeurs d'altitudes
		taille:     nombre de points valides présent dans le tableau d'altitude
	Retour: l'ascension totale calculée (en mètres)
	Cas particulier:
		si la valeur de taille nulle ou négative, la valeur renvoyée est 0
		Aucune vérification sur le fait que taille puisse dépasser le nombre de données du tableau
*/
double ascension(double tab_alti[], int taille);


/* --------------------------------------------------------------
	fonction: pentemax
	auteur:
	description:
		fonction de calcul de pente maximale de la trace.
		Pour diminuer l'effet des variations locale
		(ex: une personne se met debout avec le GPS dans la main)
		la pente est obtenu par une moyenne glissante
		prenant toujours en compte taillefenetre segments de chaque côté du point de calcul

	ex. de points utilisés pour le calcul de variation d'altitude avec taillefenetre=5:
		-pente aux points 0-4: pas défini
		-pente au point 5 est la pente moyenne de 5 segments avant le point 5 et de 5 segments après le point 5 (points 0-10)
		-pente au point 6 est la pente moyenne de 5 segments avant le point 6 et de 5 segments après le point 6 (points 0-5)
		-...
		-pente au point x est la pente moyenne de 5 segments avant le point x (point (x-5) à x) et de 5 segments après le point x (point x à (x+5)
		-pente des 4 derniers points pas définis

	Paramètres:
		tab_lat:        tableau des valeurs de latitude
		tab_long:       tableau des valeurs de longitude
		tab_alti:       tableau des valeurs d'altitude
		taille:         nombre de valeurs valides présentes dans les tableaux
		taille_fenetre: nombre de segments de trace a considérer avant et après chaque point de calcul de pente

	Note:
		Attention la distance parcourue doit être calculée en suivant la trace avec
		la fonction distance_totale
		Si la distance entre 2 points est nulle, on suppose que la pente est nulle pour éviter une division par 0.

	Retour:
		la pente maximale calculée
	Cas particuliers:
		Si taille_fenetre est invalide (valeur nulle ou négative) la fonction renvoie 0
		Si taille_fenetre est trop grand et qu'il n'y a pas assez de points disponibles dans les
		tableaux de données, la fonction renvoi 0


*/
double pentemax(double tab_lat[], double tab_long[], double tab_alti[], int taille, int taille_fenetre);

/* --------------------------------------------------------------
	fonction: valeurmin_tab
	auteur:
	description: rechercher la valeur minimale d'un tableau de réelsvaleurmin_tab
	paramètres:
		taille: nombre de points valides présent dans le tableau
		tab:    tableau des valeurs dans lequel la recherche du minimum doit se faire
		valmax: valeur minimale trouvée dans le tableau (reçu par adresse)
	Retour:
		1 en cas de résultat valide, 0 sinon (taille<=0)
	Note:
		la valeur min est stockée dans valmin qui est transmis par adresse
	Cas particulier:
		si taille est nulle ou négative, la valeur renvoyée est 0 et valmin est fixé à 0
*/
int valeurmin_tab(double tab[], int taille, double* valmin);

/* --------------------------------------------------------------
	fonction: valeurmax_tab
	auteur:
	description: rechercher la valeur maximale d'un tableau de réels
		paramètres:
		taille: nombre de points valides présent dans le tableau
		tab:    tableau des valeurs dans lequel la recherche du maximum doit se faire
		valmax: valeur maximale trouvée dans le tableau (reçu par adresse)

	Retour:
		1 en cas de résultat valide, 0 sinon (taille<=0)
	Note:
		la valeur max est stockée dans valmax qui est transmis par adresse
	Cas particulier:
		si taille est nulle ou négative, la valeur renvoyée est 0 et valmax est fixé à 0
*/

int valeurmax_tab(double tab[], int taille, double* valmax);

#endif
