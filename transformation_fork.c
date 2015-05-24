#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/wait.h>
#include "transformation_fork.h"

//Fonctions

//Récupère un pixel et le renvoie sous la forme de tableau
unsigned char * getPixel(unsigned char * img, int x, int y, int t_x){
	unsigned char * pixel = (unsigned char *) malloc(3 * sizeof(unsigned char));
	long offset = 3*(y*t_x + x);
	pixel[0] = img[offset];
	pixel[1] = img[offset + 1];
	pixel[2] = img[offset + 2];

	return pixel;
}

//Définit la nouvelle couleur du pixel
void setPixel(unsigned char * img, int x, int y, int t_x, unsigned char r, unsigned char g, unsigned char b){
	long offset     = 3*(y*t_x + x);
	img[offset]     = r;
	img[offset + 1] = g;
	img[offset + 2] = b;
}

//Définit la nouvelle couleur de pixel à partir d'un tableau de pixel
void setPixelT(unsigned char * img, int x, int y, int t_x, unsigned char * pixel){
	setPixel(img, x,y,t_x, pixel[0], pixel[1], pixel[2]);
}

//Donne un effet sphérique à la photo carrée
void sphere(unsigned char* base, unsigned char* final,int l_x, int l_y){
	int x,y;
	double n_x, n_y;
	double r,t;
	unsigned char* pixel;

	for(x = 0; x< l_x; x++){
		for(y=0; y<l_y; y++){
			n_x = (double)(x-l_x/2)/(double)(l_x/2);
			n_y = (double)((l_y/2) - y)/(double)(l_y/2);

			r   = sqrt(pow(n_x,2) + pow(n_y,2));
			t   = atan2(n_y,n_x	);
			if(r >= 0 && r <= 1){
				r = (r + (double)(1.0 - sqrt(1.0 -r*r)))/ 2.0;
				if(r <= 1){
					n_x = ((r * cos(t)+1) * l_x)/2;
					n_y = ((r * sin(t)+1) * l_y)/2;
					pixel=getPixel(base,n_x,l_y-n_y,l_x);
					setPixelT(final, x,y,l_x ,pixel);
					free(pixel);
				}
			}
		}
	}
}

//Sélectionne un carré de la photo
void rotation(unsigned char* img, unsigned char* img2, int L, int H, double angle){
	double anglepix=(angle*L)/360.0;
	unsigned char* pixel;
	int n_x, x, y;
	for(x=0; x<H; x++)
	{
		n_x=(x+(int)anglepix)%L;
		for (y=0; y<H; y++)
		{
			pixel=getPixel(img, n_x, y, L);
			setPixelT(img2, x, y, H, pixel);
			free(pixel);
		}
	}
}

void traitement(double i, int L, int H, unsigned char* img, unsigned char* rot, unsigned char* final){
	char buff[10];

	printf("angle : %.2f \n", i);
	sprintf(buff, "%.2f", i);
	rotation(img, rot, L, H, i);
	sphere(rot, final, H, H);
	strcat(buff,".ppm");
	ecrireFichierPpm(buff,final,H,H);
}

void magic_gen(char* name, double pas){
	clock_t t;
	double i;
	int pid1,pid2,pid3;
	int H, L;
	unsigned char* img = NULL;
	unsigned char* rot = NULL;
	unsigned char* final = NULL;
	t=clock();

	//	Lecture de la photo et enregistrement en chaine de caractères (on récupère au passage H et L)
	printf("Lecture de l'image de base ... ");
	img = lireFichierPpm(name, &H, &L);
	printf("Fait\n");

	//	Allocation de la mémoire
	rot = (unsigned char *)calloc(H*H*3, sizeof(unsigned char));
	final = (unsigned char *)calloc(H*H*3, sizeof(unsigned char));
	printf("Traitement :\n");

	pid1=fork();
	if (pid1==-1)
	{
		perror("fork");
		exit(1);
	}

	if (pid1==0){
		//fils1
		for(i=pas;i<360;i=i+4*pas)
		{
			traitement(i,L,H,img,rot,final);
		}
		wait(NULL);
	}
	else{
		//pere

		pid2=fork();
		if (pid2==-1)
		{
			perror("fork");
			exit(1);
		}

		if (pid2==0){
			//fils2
			for(i=2*pas;i<360;i=i+4*pas)
			{
				traitement(i,L,H,img,rot,final);
			}
			wait(NULL);
		}
		else
		{
			//pere
			pid3=fork();
			if (pid3==-1)
			{
				perror("fork");
				exit(1);
			}

			if (pid3==0){
				//fils3
				for(i=3*pas;i<360;i=i+4*pas)
				{
					traitement(i,L,H,img,rot,final);
				}
				wait(NULL);
			}
			else
			{
				//pere
				for(i=0;i<360;i=i+4*pas)
				{
					traitement(i,L,H,img,rot,final);
				}
				wait(NULL);
				t=clock()-t;
				printf("temps : %f\n", ((double)t)/CLOCKS_PER_SEC);
	//				Liberation de la mémoire
				printf("Libération de la mémoire ... ");
				free(rot);
				free(final);
				free(img);
				printf("Fait\n");
				printf("Conversion des photos ppm en jpg ... ");
				execlp("./convert.sh","",NULL);
				printf("Fait\n");
			}
		}
	}
}

unsigned char* magic(char* name, double angle){
	//clock_t t;
	int H, L;
	unsigned char* img = NULL;
	unsigned char* rot = NULL;
	unsigned char* final = NULL;
	//t=clock();

	//	Lecture de la photo et enregistrement en chaine de caractères (on récupère au passage H et L)
	//printf("Lecture de l'image de base ... ");
	img = lireFichierPpm(name, &H, &L);
	//printf("Fait\n");

	//	Allocation de la mémoire
	rot = (unsigned char *)calloc(H*H*3, sizeof(unsigned char));
	final = (unsigned char *)calloc(H*H*3, sizeof(unsigned char));
	//printf("Traitement :\n");

	rotation(img, rot, L, H, angle);
	sphere(rot, final, H, H);

	//t=clock()-t;
	//printf("temps : %f\n", ((double)t)/CLOCKS_PER_SEC);
	//	Liberation de la mémoire
	//printf("Libération de la mémoire ... ");
	free(rot);
	free(img);

	//ecrireFichierPpm("coucou.ppm",final,H,H);

	return final;
}