#include "fileIO.h"


int* getInputFileValues(char *pFileName) {

    FILE *pInputFile;
    char        paramName[20];
    static int  paramValues[4];
    /**
     * 0 parameter1 -> x
     * 1 parameter2 -> y
     * 2 ...
     */
        printf("filename: %s\n", pFileName);


    /* Error if file failed to open */
    if ( (pInputFile = fopen(pFileName, "r")) == NULL ) {
        fprintf(stderr, "Error: Failed to open/read file... %s\n", __FILE__);
        fflush(stderr);
        exit(EXIT_FAILURE);
    }

    /* Read until the end of the file */
    int i = 0;
    while (!feof(pInputFile)) {
        /* scan file, checking for [PARAMETER, VALUE] */
        if ( fscanf(pInputFile, "%s %d", &paramName, &paramValues[i]) == 2) {
            i++;
        }
    }
    fclose(pInputFile);

    return paramValues;
}

/* int outputFile(char *fileName) {} */