#include "fileIO.h"
#include "queueSystem.h"

/*      Function Prototypes      */
void runSims(int *, int *, gsl_rng *, int);
int  genRandom(gsl_rng *, int, int);
void setArrays(int (*)[2], int *, int, int);
void sumOutputParams(int *outputParams, int numSims);