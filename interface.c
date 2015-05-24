
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <SDL.h>
#include <SDL/SDL_image.h>
#include <dirent.h>
#include <sys/types.h>
#include <math.h>
#include <time.h>
#include <sys/wait.h>
#include "interface.h"

/**
 \fn check
 \param SDL_Surface *s Surface allouée (ou pas, on veut le vérifier)
 */
void check(SDL_Surface *s)	/* sert à vérifier que les surfaces sont bien allouées*/
{
	if (!s)
	{
		fprintf(stderr, "Erreur : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
}

/**
 \fn charger
 \param liste *l liste contenant le nom de l'image à charger
 \param int *ang angle de l'image à afficher
 \param int *N taille (côté) de l'image à afficher
 \result SDL_Surface *s image chargée
 */
SDL_Surface *charger(liste *l, double ang, int N)
{
	SDL_Surface *s=NULL;

	s = SDL_CreateRGBSurface(SDL_HWSURFACE, N, N, 32, 0, 0, 0, 0);	/*On crée la surface*/
	check(s);
	if(l->rang)	/*Si la liste contient bien une image à cet endroit*/
	{
		unsigned char *chaine=magic(l->nom, ang); /*On récupère l'image sous forme de chaîne*/
		draw_sphere(s, chaine, N);	/*Et on trace l'image sur la surface*/
		free(chaine);
	}
	/*SDL_SetColorKey(s, SDL_SRCCOLORKEY, SDL_MapRGB(s->format, 0, 0, 0));*/

	return s;
}

/**
 \fn mosaique
 \param SDL_Surface *ecran pointeur sur la fenêtre
 \param liste *l liste contenant les images miniatures
 \param liste *l2 liste contenant les images taille ecran
 */
void mosaique(SDL_Surface *ecran, liste *l, liste *l2)
{
	int continuer=1, n=l->prec->rang, nbpages=1, page=1, X=floor((L-ESPACE)/(MI+ESPACE)), Y=floor((H-ESPACE)/(MI+ESPACE));
	liste *point = l->suiv, *point2 = l2->suiv;

	while ( n > X*Y )
	{
		nbpages = nbpages + 1;	/*On calcule le nombre de pages à afficher*/
		n = n - X*Y;
	}

	while(continuer)
	{
		page = afficher_page(ecran, point, l2, page, nbpages, X*Y);
		if( page > nbpages )	{	page = 1;	}
		else if( page == 0 )	{	page = nbpages;	}
		
		point = l->suiv;
		while(point->rang != (page-1)*X*Y +1)
		{	point = point->suiv;	point2 = point2->suiv;	}
	}
}

/**
 \fn afficher
 \param SDL_Surface *ecran pointeur sur la fenêtre
 \param liste *l liste contenant les images miniatures
 \param liste *l2 liste contenant les images taille ecran
 \param int page numéro de la page actuelle
 \param int nbpages nombres de pages
 \param int imgmax nombre d'images par page
 */
int afficher_page(SDL_Surface *ecran, liste *l, liste *l2, int page, int nbpages, int imgmax)
{
	int i, continuer=1, actualiser=1, temps1=0, temps2=0;
	double ang=0;
	liste *point=l;
	SDL_Event event;
	SDL_Surface **tab=calloc(imgmax, sizeof(**tab));
	SDL_Rect *coo=calloc(imgmax, sizeof(*coo)), souris;

	coo[0].x = ESPACE; coo[0].y = ESPACE;
	for(i=1; i<imgmax; i++)
	{
		if ( coo[i-1].x > (L - MI - 2*ESPACE) )
		{	coo[i].x = ESPACE; coo[i].y = coo[i-1].y + MI + ESPACE;	}
		else
		{	coo[i].x = coo[i-1].x + MI + ESPACE; coo[i].y = coo[i-1].y;	}
	}	/*On calcule les coordonées des miniatures*/

	while(continuer)
	{
		temps1 = SDL_GetTicks();
		if ( temps1 - temps2 > TIME)
		{
			if ( ang + PASM > 359 )
			{	ang = ang - 360 + PASM;	}
			else
			{	ang = ang + PASM; }
			temps2 = temps1;
			actualiser = 1;
		}/*Au bout de TIME ms on fait tourner les miniatures de PAS°*/

		if (actualiser)
		{
			SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));

			for(i=0; i<imgmax && point->rang; i++)
			{
				tab[i] = charger(point, ang, MI);
				point = point->suiv;
				SDL_BlitSurface(tab[i], NULL, ecran, &coo[i]);
			}
			point = l;

			SDL_Flip(ecran);

			for(i=0; i<imgmax && point->rang; i++)
			{	SDL_FreeSurface(tab[i]);	}
		}/*Si il y a lieu de changer l'ecran on l'actualise*/

		if (SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					exit(EXIT_SUCCESS);	/*Sortie du programme*/
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							exit(EXIT_SUCCESS);	/*Sortie du programme*/
							break;
						case SDLK_LEFT:
							if(nbpages>1)
							{
								page = page - 1;	/*Si il y a trop d'images on affiche plusieurs pages*/
								continuer = 0;
							}
							actualiser = 0;
							break;
						case SDLK_RIGHT:
							if(nbpages>1)
							{
								page = page + 1;
								continuer = 0;
							}
						default:
							actualiser = 0;
							break;
					}
					break;
				case SDL_MOUSEBUTTONUP:
					switch(event.button.button)
					{
						case SDL_BUTTON_LEFT:
							souris.x = event.button.x; souris.y = event.button.y;
							choisir(ecran, souris, l2);
							SDL_EnableKeyRepeat(0, 0);
							break; /*Si l'utilisateur clique, on regarde où*/
						default:
							actualiser = 0;
							break;
					}
					break;
				default:
					actualiser = 0;
					break;
			}
		}
		else
		{	actualiser = 0;	}
	}

	free(tab);
	free(coo);

	return page;
}

/**
 \fn choisir
 \param SDL_Surface *ecran pointeur sur la fenêtre
 \param SDL_Rect coo coordonnées où l'utilisateur a cliqué
 \param liste *l liste contenant les noms et rangs des images
 */
void choisir(SDL_Surface *ecran, SDL_Rect coo, liste *l)
{
	int continuer=1, x=1, y=1, n=l->prec->rang;
	liste *point = l;

	while(continuer)
	{
		if(coo.y > y*(ESPACE+MI))
		{	y = y + 1;	}
		else if(coo.x > x*(ESPACE + MI))
		{	x = x + 1;	}
		else
		{	continuer = 0;	}
	}

	if( 9*(y-1) + (x-1) <= n && 9*(y-1) + (x-1) >=0 )
	{
		do
		{	point = point->suiv;	}while( point->rang != 9*(y-1) + x );
		tourner(ecran, point);	/*Si l'utilisateur a cliqué sur une miniature, on lui affiche l'image en grand*/
	}
}

/**
 \fn tourner
 \param SDL_Surface *ecran pointeur sur la fenêtre
 \param liste *l liste contenant les noms des images
 */
void tourner(SDL_Surface *ecran, liste *l)
{
	int continuer=1, actualiser=1, n;
	liste *point = l;
	double ang = 0;
	SDL_Event event;
	SDL_Surface *centre;
	SDL_Rect coocentre;
	coocentre.x = (L / 2) - (LI / 2); coocentre.y = (H / 2) - (LI / 2);

	while(point->rang)	{	point = point->suiv;	}
	n = point->prec->rang;

	SDL_EnableKeyRepeat(10, 10);

	while (continuer)
	{
		if (actualiser)
		{
			centre = charger(l, ang, LI);

			SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
			SDL_BlitSurface(centre, NULL, ecran, &coocentre);

			SDL_Flip(ecran);

			SDL_FreeSurface(centre);
		}/*Si il y a lieu de changer l'ecran on l'actualise*/

		SDL_WaitEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				exit(EXIT_SUCCESS);/*Sortie du programme*/
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						exit(EXIT_SUCCESS);/*Sortie du programme*/
						break;
					case SDLK_LEFT:
						if ( ang - PASG < 0 )
						{	ang = ang + 360 - PASG;	}
						else
						{	ang = ang - PASG; }
						actualiser = 1;
						break;
					case SDLK_RIGHT:
						if ( ang + PASG > 359 )
						{	ang = ang - 360 + PASG;	}	/*On recalcule l'angle si une flèche est appuyée*/
						else
						{	ang = ang + PASG; }
						actualiser = 1;
						break;
					case SDLK_DOWN:	/*On ramène l'utilisateur aux miniatures*/
						actualiser = 0;
						if(n>1)	{	continuer = 0;	}
						break;
					default:
						actualiser = 0;
						break;
				}
				break;
			default:
				actualiser = 0;
				break;
		}
	}
}