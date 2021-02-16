#include "simQ.h"

int main(int argc, char **argv) {

    /* validate CL arguments size */
    if (argc != 4) {
        printf("Error: command line expects three arguments...\n [InputFile][NumberOfSimulations][OutputFile]\n");
        exit(EXIT_FAILURE);
    }

    /*   load CL arguments  *
    char fileIn[]    = argv[1],
         fileOut[]   = argv[3];
    int  numSims     = argv[2];*/


    char fileIn[]    = "testInput.txt",
         fileOut[]   = "outputResult.txt";
    int  numSims     = 1000;

    /* Load parameters from the input file */
    int *inputParams = getInputFileValues(fileIn);
    int *outputParams[5];

    /*  input file parameter values */
    int maxQueueLength      = inputParams[0],
        numServicePoints    = inputParams[1],
        closingTime         = inputParams[2];

    /*  output file parameter values */
    int csmrFulfilled       = 0,
        csmrUnfulfilled     = 0,
        csmrTimedOut        = 0,
        averageWaitTime     = 0,
        timePostClosing     = 0;

    
    int i, 
        j,
        singleSim,
        clock,
        LP,
        queue[maxQueueLength],
        servicePoint[numServicePoints];

    for (singleSim = 0; singleSim < numSims; singleSim++) {

        clock = 0;
        LP = maxQueueLength;
        while(clock < closingTime) {

            

            /** service point management */
            for(i = 0; i < numServicePoints; i++) {
                
                /* random time determines service completion */
                if(genRand() == 5) {
                    servicePoint[i] = -1;
                }

                /* service point is empty */
                if(servicePoint[i] < 0) {
                    for(j = 0; j < maxQueueLength; j++) {

                        /* find customer */
                        if(queue[j] > -1) {

                            /* log wait time for customer */
                            averageWaitTime = averageWaitTime + queue[j];

                            /* move customer from queue to service point */
                            queue[j] = -1;
                            servicePoint[i] = 0;
                        }
                    }
                }
                /* inc time waiting */
                if(servicePoint[i] > -1) {
                    servicePoint[i]++;
                }
            }



            clock++;
        }
    }
    

    
    /* output simulation result */
    outputParams[0] = &csmrFulfilled;
    outputParams[1] = &csmrUnfulfilled;
    outputParams[2] = &csmrTimedOut;
    outputParams[3] = &averageWaitTime;
    outputParams[4] = &timePostClosing;
    writeOutputFile(fileOut, inputParams, *outputParams);

    
    /* outputResultsFile("outputFile.txt", simQResults); */

    exit(EXIT_SUCCESS);
}

int genRand() {

    return 5;

}