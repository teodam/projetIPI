#ifndef _INTERFACE
#define _INTERFACE

#include "liste.h"
#include "transformation_fork.h"
#include "dessin.h"

#define L 1000
#define H 670
#define LI 600
#define MI 100
#define TIME 50
#define MAX 100
#define PASM 2
#define PASG 1.5

void check(SDL_Surface *s);
SDL_Surface *charger(liste *l, double ang, int N);
void mosaique(SDL_Surface *ecran, liste *l, liste *l2);
void choisir(SDL_Surface *ecran, SDL_Rect coo, liste *l);
void tourner(SDL_Surface *ecran, liste *l);

#endif
