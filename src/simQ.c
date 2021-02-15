#include "simQ.h"

int main(int argc, char **argv) {
    
    // Load parameters from the input file
    char parameters[] = processInputFile(argv[1]);

    // store each simulation result
    char simQResults[1000];

    for (int i = 0; i < argv[2]; i++)
    {
        
    }
    
    outputResultsFile("outputFile.txt", simQResults);

    exit(EXIT_SUCCESS);
}