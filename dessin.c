#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <SDL.h>
#include <SDL/SDL_image.h>
#include <dirent.h>
#include <sys/types.h>
#include <math.h>
#include <time.h>
#include <sys/wait.h>
#include "dessin.h"

/* ********************************************************************* */
/*def_pixel : permet de modifier la couleur d'un pixel
Paramètres d'entrée/sortie :
SDL_Surface *s : la surface sur laquelle on va modifier la couleur d'un pixel
int x : la coordonnée en x du pixel à modifier
int y : la coordonnée en y du pixel à modifier
Uint32 pixel : le pixel à insérer
*/
void def_pixel(SDL_Surface *s, int x, int y, Uint32 pixel)
{
    /*octets_pixel représente le nombre d'octets utilisés pour stocker un pixel.
    En multipliant ce nombre d'octets par 8 (un octet = 8 bits), on obtient la profondeur de couleur
    de l'image : 8, 16, 24 ou 32 bits.*/
    int octets_pixel = s->format->BytesPerPixel;
    /*Ici p est l'adresse du pixel que l'on veut modifier*/
    /*surface->pixels contient l'adresse du premier pixel de l'image*/
    Uint8 *p = (Uint8 *)s->pixels + y * s->pitch + x * octets_pixel;

    /*Gestion différente suivant le nombre d'octets par pixel de l'image*/
    switch(octets_pixel)
    {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16 *)p = pixel;
            break;

        case 3:
            /*Suivant l'architecture de la machine*/
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            }
            else
            {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}

/**
 \fn draw_sphere
 \param SDL_Surface *s surface à remplir
 \param unsigned char *img image sous forme de chaine de caractères
 \param int N taille (côté) de l'image
 */
void draw_sphere(SDL_Surface *s, unsigned char *img, int N)
{
	int x, y;
	Uint32 pixel;
	Uint8 r=0, g=0, b=0, a=255;
	unsigned char *pix_val;

    SDL_LockSurface(s); /*On bloque la surface*/
    for (y=0;y<N;y++)
    {
        for (x=0;x<N;x++)
        {
        	pix_val = getPixel(img, x, y, N); /*On récupère les couleurs du pixel de coordonnées (x,y)*/
        	r = (Uint8)pix_val[0];
        	g = (Uint8)pix_val[1];
        	b = (Uint8)pix_val[2];
        	pixel = SDL_MapRGBA(s->format, r, g, b, a); /*On traduit les couleurs dans le type géré par SDL*/
        	def_pixel(s, x, y, pixel); /*On remplit le pixel en question des bonnes couleurs sur notre surface*/
            free(pix_val);
        }
    }

    SDL_UnlockSurface(s); /*On libère la surface, elle peut être utilisée*/
}