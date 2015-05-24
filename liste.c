#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include "liste.h"

/* Le type géré dans ce fichier est une liste chaînée double circulaire */

/**
 \fn creer_liste
 \result liste *racine une liste vide
 */
liste* creer_liste()
{
	liste *racine=malloc(sizeof (*racine));

	if ( racine != NULL )	/* si la racine a été correctement allouée */
	{
		/* pour l'instant, la liste est vide, 
		donc 'prec' et 'suiv' pointent vers la racine elle-même */
		racine->prec = racine;
		racine->rang = 0;
		racine->nom = RACINE;
		racine->suiv = racine;
	}

	return racine;
}

/**
 \fn vider_liste
 \param liste *l liste à vider
 */
void vider_liste(liste *l)
{
	liste *it, *next;

	for(it=l->suiv;it!=l;it=next)
	{
		next = it->suiv;
		free(it->nom);	/*On supprime un par un tous les éléments de la liste excepté la racine elle-même*/
		free(it);
	}
}

/**
 \fn suppr_liste
 \param liste *l liste à supprime
 */
void suppr_liste(liste *l)
{
	vider_liste(l);	/*On commence par vider la liste*/
	free(l);	/*On supprime al racine*/
}

/**
 \fn ajouter
 \param liste *l liste à compléter
 \param int n rang de l'image à ajouter
 \param char *nom nom de l'image à ajouter
 */
void ajouter(liste *l, int n, char *nom)
{
	liste *nouveau=malloc(sizeof *nouveau);

	if ( nouveau != NULL )
	{
		nouveau->rang = n;
		nouveau->nom = malloc(strlen(nom) * sizeof(nouveau->nom));
		strcpy(nouveau->nom, nom);
		nouveau->prec = l->prec;
		nouveau->suiv = l;
		l->prec->suiv = nouveau;
		l->prec = nouveau;
	}
}

/**
 \fn ajouter_fin
 \param liste *racine racine de la liste à compléter
 \param int n rang de l'image à ajouter
 \param char *nom nom de l'image à ajouter
 \	L'interêt de cette fonction est dans le cas où le programme change et il faut pouvoir ajouter
 	quelque chose au début de la liste, il suffit de créer une fonction similaire où on modifie la ligne
 	en "ajouter(racine->suiv, n, nom);"
 */
void ajouter_fin(liste *racine, int n, char *nom)
{
	ajouter(racine, n, nom);
}

/**
 \fn get_names
 \param liste *l liste à compléter des noms d'images
 \param char *rep nom du dossier où se trouvent les images
 */
void get_names(liste *l, char *rep)
{
	int i=0;
	char buff[CHARMAX];
	DIR *dir;
	struct dirent *entry;

	dir = opendir(rep);	/*On ouvre le dossier*/
	while((entry = readdir(dir)) != NULL)	/*On le parcourt en rajoutant un élément à la liste pour chaque image*/
	{
		if(entry->d_name[strlen(entry->d_name)-1] != '.')
		{
			i++;
			strcpy(buff, rep);
			strcat(buff, entry->d_name);
			ajouter_fin(l, i, buff);
		}
	}

	closedir(dir);	/*On referme le dossier*/
}
