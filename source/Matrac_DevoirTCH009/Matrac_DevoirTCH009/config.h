#ifndef CONFIG_H
#define CONFIG_H

// constantes
// choix du menu
#define CHARGER     1
#define AFFICHER    2
#define ANALYSER    3
#define PENTEMAX    4
#define QUITTER     5

// quelques valeurs maximale pour les tableaux statiques
#define MAXCARNOMFICHIER    100      //longueur max pour le nom de fichier
#define MAXPOINT            10000    //nombre max de points dans une trace (pour créer les tableaux statiques)
#define MAXCARLIGNE         1000     //longueur max d'une ligne dans le fichier de données CSV

#endif