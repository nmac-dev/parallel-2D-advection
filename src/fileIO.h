#include <stdio.h>
#include <stdlib.h>

/**      Function Prototypes      **/
extern int *getInputFileValues(char *);
void setupSingleOutput(char *, int *);
void writeSingleOutput(char *, int *);
void writeOutputFile(char *, int *, int *);
FILE *validateFile(char *, char *);