#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include "fileIO.h"

/*      Function Prototypes      */
int genRandom(gsl_rng *, int, int);
void lrsArray(int (*)[2], int);