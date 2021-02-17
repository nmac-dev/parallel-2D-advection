#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include "fileIO.h"

/*      Function Prototypes      */
int *runSim(int *, gsl_rng *, int *);
int genRand();