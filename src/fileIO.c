#include "fileIO.h"


int *getInputFileValues(char *fileIn) {

    FILE        *fileP = validateFile(fileIn, "r");
    char        paramName[20];
    static int  paramValues[7];

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

void setupSingleOutput(char *fileOut, int *inputParams) {

    FILE *fileP = validateFile(fileOut, "w");

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
    /* close stream */
    fclose(fileP);
}


void writeSingleOutput(char *fileOut, int *singleOutputs) {

    FILE *fileP = validateFile(fileOut, "a");

    /* Write Output parameters to file */
    fprintf(fileP, "%s\n\n%d\t\t| %s\n%d\t\t| %s\n%d\t\t| %s\n%d\t\t| %s\n%d\t\t| %s\n%d\t\t| %s\n\n",
                    "# # # # # # # # # # # # # # # # # | Single interval of simulation outputs | # # #",
                    singleOutputs[0],    "Time interval number",
                    singleOutputs[1],    "Customers being served",
                    singleOutputs[2],    "Customers in the queue",
                    singleOutputs[3],    "Fulfilled customers",
                    singleOutputs[4],    "Unfulfilled customers",
                    singleOutputs[5],    "Timed-Out customers");
    /* close stream */
    fclose(fileP);
}

void writeOutputFile(char *fileOut, int *inputParams, int *outputParams) {

    FILE *fileP = validateFile(fileOut, "w");

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

FILE *validateFile(char *file, char *mode) {

    FILE *fileP;

    /* Error if file failed to write */
    if ( (fileP = fopen(file, mode)) == NULL ) {
        fprintf(stderr, "Error: Failed to open/write file... %s\n", __FILE__);
        fflush(stderr);
        exit(EXIT_FAILURE);
    }

    return fileP;
}