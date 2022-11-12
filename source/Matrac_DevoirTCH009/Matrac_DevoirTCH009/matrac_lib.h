/*
Auteur: David Marche
Date: Automne 2016

Ce module contient les constantes et prototypes des fonctions
d'analyse de traces GPS

*/

#ifndef MATRAC_LIB_H // pour �viter d'inclure plusieur fois le m�me module
#define MATRAC_LIB_H

// constantes physiques
#define PI 3.14159
#define RAYONTERRE                                                             \
  6371 // rayon de la terre (en km) pour les calculs de distances
// source: H. Moritz, « Geodetic Reference System 1980 », Journal of Geodesy,
// vol. 74, no 1, mars 2000, p. 128–133

// prototypes

/* --------------------------------------------------------------
        fonction: afficher_tab1D_double
        Auteur: Julien Groulx
        description: affiche le contenu d'un tableau 1D de double (1 valeur par
   ligne) Param�tres: -tab: le tableau de valeurs (type double) -nb_valeur: le
   nombre de valuer � afficher Retour: rien Note: le tableau n'est pas modifi�.
                Aucune v�rification sur la validit� de nb_valeur n'est
   effectu�e(ex: nb_valeur>taille du tableau)
*/
void afficher_tab1D_double(double tab[], int nb_valeur);

/* --------------------------------------------------------------
        fonction: lire_fichier_gpx
        auteur:Charger la trace test1.txt  (fonction charger_trace)
Afficher les donn�es de la trace (afficher_trace)
 David Marche
        description: lit un fichier de donn�es GPS au format suivant
                No,Latitude,Longitude,Altitude,Date,Time
                1,49.192273,-123.178443,-44.6,2013/06/21,11:02:00
                2,49.192272,-123.178318,-44.6,2013/06/21,11:02:03
                3,49.192184,-123.178061,-41.7,2013/06/21,11:02:10
                ...

                place toutes les donn�es de lattitude, longitude, altitude et
temps dans des tableaux statiques fournis en param�tres Param�tres: nom_fichier:
cha�ne de caract�res contenant le nom du fihier � lire tab_lat:        tableau �
remplir avec les valeurs de latitude tab_long:       tableau � remplir avec les
valeurs de longitude tab_alti:       tableau � remplir avec les valeurs
d'altitude taille_max:     nombre maximum de valeurs pouvant �tre charg�es dans
les tableaux

        Retour:
                nombre de points lus, 0 si aucun, -1 si probl�me
d'ouverture/lecture de fichier, Note: les tableaux donn�es en param�tre sont
modifi�s. Les valeurs de temps pr�sentes dans le fichier sont ignor�s. Cas
particuliers: Si le fichier ne peut pas �tre ouvert la fonction renvoie -1 Si la
ligne d'en t�te n'est pas pr�sente en premi�re ligne du fichier, la fonction
renvoie -1 Si le nombre de valeurs du fichier d�passe la capacit� du tableau,
                les valeur en trop sont ignor�es
*/
int lire_fichier_gpx(char nom_fichier[], double tab_lat[], double tab_long[],
                     double tab_alti[], int taillemax);

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
                    int taille);

/* --------------------------------------------------------------
        fonction: deg2rad
        auteur: Julien Groulx
        description: convertir en radians une valeur d'angle donn�e initialement
   en degr�s Param�tres: -d: la valeur en degr�s Retour: la valeur
   correspondante en radians Cas particuliers: aucun
*/
double deg2rad(double d);

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
                               double lon2, double alt1, double alt2);

/* --------------------------------------------------------------
        fonction: distance_totale
        auteur: Julien Groulx
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
                       int taille, int debut, int fin);

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
double ascension(double tab_alti[], int taille);

/* --------------------------------------------------------------
        fonction: pentemax
        auteur: Julien Groulx
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
                int taille, int taille_fenetre);

/* --------------------------------------------------------------
        fonction: valeurmin_tab
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
int valeurmin_tab(double tab[], int taille, double *valmin);

/* --------------------------------------------------------------
        fonction: valeurmax_tab
        auteur: Julien Groulx
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

#endif
