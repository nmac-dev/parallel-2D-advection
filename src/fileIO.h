#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*      Function Prototypes      */
extern int* getInputFileValues(char *pName);
void writeOutputFile(char *fileOut, int *inputParams, int *outputParams);