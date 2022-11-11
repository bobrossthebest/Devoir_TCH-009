#include "menu.h"
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

/* procédure: afficher_menu
	auteur: David Marche
	description: afficher le menu principal
	modifiée par : Abdelmoumène Toudeft
	description des modifications : la fonction lit et retourne le choix du menu
	Retour: le choix de menu
*/
int afficher_menu() {
	int choix;
	printf("\n");
	printf("\n--- MENU PRINCIPAL ---\n");
	printf("%d. Charger la trace d'un fichier de donnees GPS\n", CHARGER);
	printf("%d. Afficher les donnees de la trace en memoire\n", AFFICHER);
	printf("%d. Analyse de base la trace\n", ANALYSER);
	printf("%d. Calculer la pente maximale\n", PENTEMAX);
	printf("%d. Quitter \n", QUITTER);
	fflush(stdin);      // par sécurité, vider le buffer avant la lecture du choix
	scanf("%d", &choix); // lecture du choix au clavier
	return choix;
}
