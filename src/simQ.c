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
	const gsl_rng_type  *T;
	gsl_rng             *r;

	gsl_rng_env_setup();
	T = gsl_rng_default;
	r = gsl_rng_alloc(T);
    
	gsl_rng_set(r, time(0));

    /* Load parameters from the input file */
    int *inputParams            = getInputFileValues(fileIn);
    static int outputParams[]   = {0, 0, 0, 0, 0},
               singleOutputs[]   = {0, 0, 0, 0, 0, 0};
    
    if (numSims == 1) {
        setupSingleOutput(fileOut, inputParams);
    }

    /***    Run Simulations     ***/

    runSims(inputParams, outputParams, singleOutputs, r, numSims, fileOut);


    /*  summarise output parameters, then output to file*/
    if (numSims > 1) {
        sumOutputParams(outputParams, numSims);
        writeOutputFile(fileOut, inputParams, outputParams);
    }

    /*  free gsl memory     */
    gsl_rng_free(r);

    /*  exit program with success */
    exit(EXIT_SUCCESS);
}


/** Runs all simulations using the CL and input file parameters  **/
void runSims(int *inputParams, int *outputParams, int *singleOutputs, gsl_rng *r, int numSims, char *fileOut) {

    /** Parameter Values For... **/

    /*  ...Input    */
    int *inpMaxQueueLength      = inputParams + 0,
        *inpNumServicePoints    = inputParams + 1,
        *inpClosingTime         = inputParams + 2,

        *inpStdDiviation        = inputParams + 3,
        *inpCsmrFrequency       = inputParams + 4,
        *inpCsmrWaitTolerance   = inputParams + 5,
        *inpServicePointSpeed   = inputParams + 6;

    /*  ...Output   */
    int *outpCsmrFulfilled      = outputParams + 0,
        *outpCsmrUnfulfilled    = outputParams + 1,
        *outpCsmrTimedOut       = outputParams + 2,
        *outpAverageWaitTime    = outputParams + 3,
        *outpTimePostClosing    = outputParams + 4;

    int i,
        singleSim;

    /* represents a single simulation per loop*/
    for (singleSim = 0; singleSim < numSims; singleSim++) {

        /* Simulation Variables */
        int csmrQueue[*inpMaxQueueLength][2],
            servicePoint[*inpNumServicePoints],
            clock                   = 0,
            occupiedServicePoints   = 0,
            nextCustomerArrival     = genRandom(r, *inpCsmrFrequency, *inpStdDiviation);

            setArrays(csmrQueue, servicePoint, *inpMaxQueueLength, *inpNumServicePoints);

        /* when true a single simulation is complete */
        while(clock < *inpClosingTime || occupiedServicePoints > 0) {

            /**     Service Point Processing        **/

            /* Update occupied service points */
            occupiedServicePoints += checkServicePoints(servicePoint, 
                                                        csmrQueue, 
                                                        inputParams, 
                                                        clock, 
                                                        r, 
                                                        outputParams);

            /**     Customer Queue Processing       **/

            /* update parameter if customer has timed out   */
            *outpCsmrTimedOut += checkCsmrTimeout(csmrQueue, *inpMaxQueueLength, clock);

            /* update parameter if a new customer was declined */
            *outpCsmrUnfulfilled+= checkForNewCsmr( csmrQueue, 
                                                    &nextCustomerArrival,
                                                    clock, 
                                                    r,
                                                    inputParams);

            /* increment clock for next iteration */
            clock++;

            /* Output current interval data */
            if(numSims == 1) {

                singleOutputs[0] = clock;
                singleOutputs[1] = occupiedServicePoints;
                singleOutputs[2] = countCsmrQueue(csmrQueue, *inpMaxQueueLength);
                singleOutputs[3] = outputParams[0];
                singleOutputs[4] = outputParams[1];
                singleOutputs[5] = outputParams[2];

                writeSingleOutput(fileOut, singleOutputs);
            }
        }
        /* log completed simulation outputs */
        *outpTimePostClosing += clock - *inpClosingTime;
    }
    /* Summarise average wait time*/
    *outpAverageWaitTime =  (*outpAverageWaitTime * numSims) / *outpCsmrFulfilled;
}


/** Generates a random number for each function call **/
int genRandom(gsl_rng *r, int lowerLimit, int diviation) {
    return (int)gsl_ran_gaussian_tail(r, lowerLimit ,diviation);
    
}

/** Set the arrays for the simulation to the inputParam sizes and instantiate each index **/
void setArrays(int csmrQueue[][2], int *servicePoint, int maxQueueLength, int NumServicePoints) {

    int i;

    for(i = 0; i < maxQueueLength; i++) {
        csmrQueue[i][0] = -1;
    }
    for(i = 0; i < NumServicePoints; i++) {
        servicePoint[i] = -1;
    }
}

/** Summarises the output parameters against the numbers of simulations **/
void sumOutputParams(int *outputParams, int numSims) {

    int i;

    /* Divide each param by the number of simulations */
    for(i = 0; i < 5; i++) {
        outputParams[i] = outputParams[i] / numSims;
    }
}