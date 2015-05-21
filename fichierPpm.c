/** \file fichierPpm.c
	\author {Pierre Tellier}
	\brief module de fonctions de lecture et écriture au format PPM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fichierPpm.h"

/**
 \fn lireFichierPpm
 \param char * nf nom de fichier
 \param int *NBL nombre de lignes de l'image
 \param int *NBC nombre de colonnes de l'image
 \result pointeur sur le bitmap
 */

unsigned char * lireFichierPpm(char *nf, int *NBL, int *NBC) {
	FILE *f;
	unsigned char *res=NULL;
	char buffer[128];
	int maxval;
	
	f = fopen(nf,"rb");
	if (f==NULL) {
		fprintf(stderr,"erreur ouverture %s en lecture\n",nf);
		exit(-1);
	}
	
	fgets(buffer, 128, f);
	while (buffer[0]=='#') 	fgets(buffer, 128, f);
	/*if (strncmp(buffer,"P6",2)!=0) {
		fprintf(stderr,"entête : %s non reconnue\n",buffer);
		exit(-1);
	}
	else */
	fgets(buffer, 128, f);
	while (buffer[0]=='#') 	fgets(buffer, 128, f);
	sscanf(buffer, "%d %d", NBC, NBL);
	
	fgets(buffer, 128, f);
	while (buffer[0]=='#') 	fgets(buffer, 128, f);
	sscanf(buffer, "%d", &maxval);	
	res=(unsigned char *)malloc(3*(*NBL)*(*NBC));
//	fgets(buffer, 128, f);
	fread(res, 3, (*NBL)*(*NBC), f);
	fclose(f);
	return res;
}
	
void ecrireFichierPpm(char *nf, unsigned char *img, int NBL, int NBC) {
	FILE *f;
	
	f = fopen(nf,"wb");
	if (f==NULL) {
		fprintf(stderr,"erreur ouverture %s en écriture\n",nf);
		exit(-1);
	}
	
	fprintf(f,"P6\n#C EST MON IMAGE A MOI !!!\n%d %d\n255\n",NBC, NBL);
	fwrite(img, 3, NBC*NBL, f);
	fclose(f);
}



