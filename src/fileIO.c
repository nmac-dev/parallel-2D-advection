#include "fileIO.h"


int *getInputFileValues(char *fileIn) {

    FILE        *fileP;
    char        paramName[20];
    static int  paramValues[7];

    /* Error if file failed to open */
    if ( (fileP = fopen(fileIn, "r")) == NULL ) {
        fprintf(stderr, "Error: Failed to open/read file... %s\n", __FILE__);
        fflush(stderr);
        exit(EXIT_FAILURE);
    }

    /* Read stream until the end of the file */
    int i = 0;
    while (!feof(fileP)) {
        /* scan stream from file, checking for [PARAMETER VALUE] */
        if ( fscanf(fileP, "%s %d", &paramName, &paramValues[i]) == 2) {
            i++;
        }
    }
    fclose(fileP);

    return paramValues;
}

void writeOutputFile(char *fileOut, int *inputParams, int *outputParams) {

    FILE *fileP;

    /* Error if file failed to write */
    if ( (fileP = fopen(fileOut, "w")) == NULL ) {
        fprintf(stderr, "Error: Failed to open/write file... %s\n", __FILE__);
        fflush(stderr);
        exit(EXIT_FAILURE);
    }

    /* Write Input parameters to file */
    fprintf(fileP, "%s\n\n%d\t\t| %s\n%d\t\t| %s\n%d\t\t| %s\n%d\t\t| %s\n%d\t\t| %s\n%d\t\t| %s\n%d\t\t| %s\n\n",
                    "### The simulation input parameters",
                    inputParams[0],     "maxQueueLength",   
                    inputParams[1],     "numServicePoints",
                    inputParams[2],     "closingTime",
                    inputParams[3],     "stdDiviation",
                    inputParams[4],     "csmrFrequency",
                    inputParams[5],     "csmrWaitTolerance",
                    inputParams[6],     "servicePointSpeed");

    /* Write Output parameters to file */
    fprintf(fileP, "%s\n\n%d\t\t| %s\n%d\t\t| %s\n%d\t\t| %s\n%d\t\t| %s\n%d\t\t| %s\n",
                    "### The  simulation output parameters",
                    outputParams[0],    "Number of fulfilled customers",
                    outputParams[1],    "Number of unfulfilled customers",
                    outputParams[2],    "Number of timed-out customers",
                    outputParams[3],    "Average time for fulfilled customers",
                    outputParams[4],    "Time it to serve all customers from closing");
    /* close stream */
    fclose(fileP);
}