#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

/**      Function Prototypes      **/
int checkCsmrTimeout(int (*)[2], int, int);
int checkForNewCsmr(int (*)[2], int *, int, gsl_rng *, int *);
int isServiceComplete(int *, int, int);
int checkServicePoints(int *, int (*)[2], int *, int, gsl_rng *, int *);
void lrsArray(int (*)[2], int);
int countCsmrQueue(int (*)[2], int);