#include "fileIO.h"


int processInputFile(char *ptName) {
    FILE *ptFile;
    
    char paramValues[3];
    /**
     * 0 parameter1 -> x
     * 1 parameter2 -> y
     * 2 ...
     */

    // Error if file failed to open
    if ( (ptFile = fopen(ptName, 'r')) == NULL ) {
        fprintf(stderr, "Error: Failed to open/read file... %s\n", __FILE__);
        fflush(stderr);
        exit(EXIT_FAILURE);
    }

    // Read until the end of the file
    for (int i = 0; !feof(ptFile);) {
        // scan file, checking for [PARAMETER, VALUE]
        if ( fscanf(ptFile, "%s %d", NULL, &paramValues[i, 2]) == 2)
        {

        }
    }
    fclose(ptFile);
    exit(EXIT_FAILURE);
}

//int outputFile(char *fileName) {} 