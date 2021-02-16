#include "simQ.h"

int main(int argc, char *argv) {
    
    char fileName[] = "testInput.txt";

    /* Load parameters from the input file */
    int *pInputFileValues = getInputFileValues(fileName);

    int i;
    for (i = 0; i<3; i++) {
        printf("%d\n", pInputFileValues[i]);
    }
    /** inputFileValues
     * 0 parameter1 -> x
     * 1 parameter2 -> y
     * 2 ...
     */


    /* store each simulation result */

    /*int i;
    for (i = 0; i < (int)argv[2]; i++)
    {
        
    }*/
    
    /* outputResultsFile("outputFile.txt", simQResults); */

    exit(EXIT_SUCCESS);
}