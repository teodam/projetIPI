#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <SDL.h>
#include <SDL/SDL_image.h>
#include <dirent.h>
#include <sys/types.h>
#include "interface.h"
#include "liste.h"
#include "transformation_fork.h"

void menu(SDL_Surface *ecran);

int main()
{
	SDL_Surface *ecran=NULL;

	if (SDL_Init(SDL_INIT_VIDEO) == -1)	/*Initialisation de la SDL*/
	{
		fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	ecran = SDL_SetVideoMode(L, H, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
	check(ecran);	/*Ouverture de la fenêtre*/

	SDL_EnableKeyRepeat(10, 10);

	system("Images/./init.sh");
	menu(ecran);
	SDL_Quit();	/*On quitte la SDL proprement*/

	return EXIT_SUCCESS;
}

/**
 \fn menu
 \param SDL_Surface *ecran pointeur sur la fenêtre
 */
void menu(SDL_Surface *ecran)
{
	int n;
	liste *minies=creer_liste(), *micros=creer_liste();

	get_names(minies, "Images/.minies/");	/*Création des listes d'images*/
	get_names(micros, "Images/.micros/");

	n = minies->prec->rang;	/*On récupère le nombre d'images*/

	if (!n)
	{
		printf("\nPas d'image!\n\n");
		exit(EXIT_SUCCESS);
	}
	else if ( n == 1)
	{	tourner(ecran, minies->suiv);	}	/*Si il n'y en a qu'une on l'affiche en grand*/
	else
	{	mosaique(ecran, micros, minies);	}	/*Sinon on présente des miniatures*/

	suppr_liste(minies);
	suppr_liste(micros);	/*Suppression des listes*/
}

