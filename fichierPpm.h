/*
 *  fichierPpm.h
 *  
 *
 *  Created by Pierre Tellier on 21/10/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

extern unsigned char * lireFichierPpm(char *nf, int *NBL, int *NBC);
extern void ecrireFichierPpm(char *nf, unsigned char *img, int NBL, int NBC);
