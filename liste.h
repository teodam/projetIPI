#ifndef _LISTE_CIRC_
#define _LISTE_CIRC_

#define CHARMAX 200
#define RACINE "~~racine~~"

typedef struct liste liste;

struct liste
{
	liste *prec;
	int rang;
	char *nom;
	liste *suiv;
};

liste* creer_liste();
void vider_liste(liste *l);
void suppr_liste(liste *l);
void ajouter(liste *l, int n, char *nom);
void ajouter_fin(liste *racine, int n, char *nom);
void get_names(liste *noms, char *rep);

#endif
