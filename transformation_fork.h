#ifndef _FORK
#define _FORK

#include "fichierPpm.h"

//Déclaration des fonctions
unsigned char * getPixel(unsigned char * img, int x, int y, int t_x);
void setPixel(unsigned char * img, int x, int y, int t_x, unsigned char r, unsigned char g, unsigned char b);
void setPixelT(unsigned char * img, int x, int y, int t_x, unsigned char * pixel);
void sphere(unsigned char * base, unsigned char* final,int l_x, int l_y);
void rotation(unsigned char* img, unsigned char* img2, int L, int H, double angle);
void traitement(double i, int L, int H, unsigned char* img, unsigned char* rot, unsigned char* final);
void magic_gen(char* name, double pas);
unsigned char* magic(char* name, double angle);

#endif