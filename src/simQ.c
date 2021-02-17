#include "simQ.h"

int main(int argc, char **argv) {

    /* validate CL arguments size */
    if (argc != 4) {
        printf("%s\n%s\n", 
                "Error: command line expects three arguments...",
                "[InputFile][NumberOfSimulations][OutputFile]");
        exit(EXIT_FAILURE);
    }

    /*   load CL arguments  */
    char *fileIn    = argv[1],
         *fileOut   = argv[3];
    int  numSims    = atoi(argv[2]);

    /*  GSL true random number setup   */
    const gsl_rng_type*     T;
	gsl_rng*                rndNum;
	gsl_rng_env_setup();
	T       = gsl_rng_default;
	rndNum  = gsl_rng_alloc(T);
	gsl_rng_set(rndNum, time(0));

    /* Load parameters from the input file */
    int *inputParams = getInputFileValues(fileIn);

    /************** end of main after working   
    /*  Run the simulation(s), 
        loading the results into the array *
    

    int *outputParams = runSim(inputParams, gsl_rng *rndNum, numSims);

    writeOutputFile(fileOut, inputParams, outputParams);
                    ********************/

    /*  free gsl memory     */
    gsl_rng_free(rndNum);

/************* sim below here ****************/

    /*  input file parameter values */
    int maxQueueLength      = inputParams[0],
        numServicePoints    = inputParams[1],
        closingTime         = inputParams[2];

    /*  output file parameter values */
    int outputParams[]      = {0, 0, 0, 0, 0,};
    int csmrFulfilled       = 0,
        csmrUnfulfilled     = 0,
        csmrTimedOut        = 0,
        averageWaitTime     = 0,
        timePostClosing     = 0;

    
    int i, j,
        singleSim,
        clock,
        LP,
        queue[maxQueueLength],
        servicePoint[numServicePoints];

    /*  1 iteration == 1 simulation */
    for (singleSim = 0; singleSim < numSims; singleSim++) {

        clock = 0;
        LP = maxQueueLength;
        /* Loop until the single simulation is complete */
        while(clock < closingTime && servicePoint[numServicePoints] < 0) {

            /********************* service point management */
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
    outputParams[0] = csmrFulfilled;
    outputParams[1] = csmrUnfulfilled;
    outputParams[2] = csmrTimedOut;
    outputParams[3] = averageWaitTime;
    outputParams[4] = timePostClosing;
    if (numSims > 1) {
        for(i = 0; i < 5; i++){
            /*outputParams[i] = (outputParams[i] / numSims);*/
        }
    }
    writeOutputFile(fileOut, inputParams, outputParams);

    
    /* outputResultsFile("outputFile.txt", simQResults); */

    exit(EXIT_SUCCESS);
}

int *runSim(int *inputParams, gsl_rng *rndNum, int* numSims) {

}

int genRand() {

    return 5;

}