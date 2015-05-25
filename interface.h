#ifndef _INTERFACE
#define _INTERFACE

#include "liste.h"
#include "transformation_fork.h"
#include "dessin.h"

#define L 800
#define H 600
#define LI 600
#define MI 100
#define TIME 50
#define MAX 100
#define PASM 2
#define PASG 1.5
#define ESPACE 120

void check(SDL_Surface *s);
SDL_Surface *charger(liste *l, double ang, int N);
void mosaique(SDL_Surface *ecran, liste *l, liste *l2);
int afficher_page(SDL_Surface *ecran, liste *l, liste *l2, int page, int nbpages, int imgmax);
void choisir(SDL_Surface *ecran, SDL_Rect coo, liste *l, int page, int imgmax);
void tourner(SDL_Surface *ecran, liste *l);

#endif
