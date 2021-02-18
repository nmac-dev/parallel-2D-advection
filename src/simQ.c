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
    static int outputParams[]   = {0, 0, 0, 0, 0};

    /************** end of main after working   
    /*  Run the simulation(s), 
        loading the results into the array *
    

    int *outputParams = runSim(inputParams, gsl_rng *r, numSims);

    writeOutputFile(fileOut, inputParams, outputParams);
                    ********************/

    /*  free gsl memory     *
    gsl_rng_free(r);*/

/************* sim below here ****************/

    /*  input file parameter values */
    int *inPMaxQueueLength      = inputParams + 0,
        *inPNumServicePoints    = inputParams + 1,
        *inPClosingTime         = inputParams + 2,

        *inPStdDiviation        = inputParams + 3,
        *inPCsmrFrequency       = inputParams + 4,
        *inPCsmrWaitTolerance   = inputParams + 5,
        *inPServicePointSpeed   = inputParams + 6;

    /*  output file parameter values */
    int *outPCsmrFulfilled      = outputParams + 0,
        *outPCsmrUnfulfilled    = outputParams + 1,
        *outPCsmrTimedOut       = outputParams + 2,
        *outPAverageWaitTime    = outputParams + 3,
        *outPTimePostClosing    = outputParams + 4;
    
    int singleSim, i, j;

    /*  1 iteration == 1 simulation */
    for (singleSim = 0; singleSim < numSims; singleSim++) {

        int queue[*inPMaxQueueLength],
            servicePoint[*inPNumServicePoints],
            clock                   = 0,
            occupiedServicePoints   = 0,
            nextCustomerArrival     = genRandom(r, *inPCsmrFrequency, *inPStdDiviation);

                printf("NCA: %d\n", nextCustomerArrival);

        for(i = 0; i < *inPMaxQueueLength; i++) {
            queue[i] = -1;
        }
        for(i = 0; i < *inPNumServicePoints; i++) {
            servicePoint[i] = -1;
        }

        /* Loop until the single simulation is complete */
        while(clock < *inPClosingTime) {

            printf("Clock: %d\n",  clock + 1);

            /********************* service point management */
            for(i = 0; i < *inPNumServicePoints; i++) {
                

               /* printf("SP%d: %d\n", i + 1, servicePoint[i]);

                /*FUNC UPDATE SERVICE 
                    empty service point if complete */
                if(clock == servicePoint[i]) {

                    printf("NEW_CSMR-SP: %d\n", i +1);
                    servicePoint[i] = -1;
                    occupiedServicePoints--;

                }

                /* Fill service point if empty, with waiting customer */
                if(servicePoint[i] == -1 && queue[*inPMaxQueueLength -1] > -1) {

                    printf("SP-COMPLETED: %d\n", queue[*inPMaxQueueLength -1]);

                    /* log wait time for customer                   TODO LOG arival time*/
                    *outPAverageWaitTime += clock - queue[*inPMaxQueueLength -1];
                
                    /* move customer to service point, generate service time */
                    servicePoint[i] = clock + genRandom(r, *inPServicePointSpeed ,*inPStdDiviation);
                    occupiedServicePoints++;

                    /* shift queue */
                    lrsArray(queue, *inPMaxQueueLength);


                    /* log customer has been fulfilled */
                    *outPCsmrFulfilled += + 1;
                    
                }

            }
            /********************* csmr queue managment     */
            for(i = 0; i < *inPMaxQueueLength; i++) {

                /*  FUNC 
                    Customer leaves */
                if(clock == queue[i]) {
                    
                    printf("CSMR-TO: %d\n", queue[i]);

                    /* shift queue */
                    lrsArray(queue, i);

                    /*  log custmer timed out   */
                    *outPCsmrTimedOut+= + 1;
                }


                /* suffle queue empty spaces */
                if(queue[i] == -1) {
                    lrsArray(queue, i);
                }

            }

            /* Accept or Decline New customer */
            if (clock == nextCustomerArrival) {

                /* Accept   */
                if (queue[0] == -1) {
                    printf("CSMR-Accepted++\n");
                    queue[0] = clock + genRandom(r, *inPCsmrWaitTolerance, *inPStdDiviation);
                }
                /* Decline  */
                else {
                    printf("CSMR-Declined--\n");
                    *outPCsmrUnfulfilled+= + 1;;
                }
                nextCustomerArrival = clock + genRandom(r, *inPCsmrFrequency, *inPStdDiviation);
                printf("NCA-@: %d\n", nextCustomerArrival);
            }

            clock++;
        }
        *outPTimePostClosing = clock - *inPClosingTime;
    }
    
    /* output simulation result *
    if (numSims > 1) {
        for(i = 0; i < 5; i++){
            outputParams[i] = (outputParams[i] / numSims);
        }
    } */

        printf("OUTPUT:\n%d\n%d\n%d\n%d\n%d\n", 
                        *outPCsmrFulfilled,
                        *outPCsmrUnfulfilled,
                        *outPCsmrTimedOut, 
                        *outPAverageWaitTime, 
                        *outPTimePostClosing);

    writeOutputFile(fileOut, inputParams, outputParams);

    exit(EXIT_SUCCESS);
}

int genRandom(gsl_rng *r, int lowerLimit, int diviation) {
    return (int)gsl_ran_gaussian_tail(r, lowerLimit ,diviation);
}

void lrsArray(int *array, int position) {
        
        int i;
        for(i = position; i > -1; i--) {
            array[i] = array[i - 1];
        }
        array[0] = -1;
}