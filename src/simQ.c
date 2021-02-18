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
                    ********************/


/************* sim below here ****************/

    /*  input file parameter values */
    int *inpMaxQueueLength      = inputParams + 0,
        *inpNumServicePoints    = inputParams + 1,
        *inpClosingTime         = inputParams + 2,

        *inpStdDiviation        = inputParams + 3,
        *inpCsmrFrequency       = inputParams + 4,
        *inpCsmrWaitTolerance   = inputParams + 5,
        *inpServicePointSpeed   = inputParams + 6;

    /*  output file parameter values */
    int *outpCsmrFulfilled      = outputParams + 0,
        *outpCsmrUnfulfilled    = outputParams + 1,
        *outpCsmrTimedOut       = outputParams + 2,
        *outpAverageWaitTime    = outputParams + 3,
        *outpTimePostClosing    = outputParams + 4;
    
    int singleSim, i, j;

    /*  1 iteration == 1 simulation */
    for (singleSim = 0; singleSim < numSims; singleSim++) {

        /* 0:[customer wait time] 1:[customer arrival time] */
        int queue[*inpMaxQueueLength][2],
            servicePoint[*inpNumServicePoints],
            clock                   = 0,
            occupiedServicePoints   = 0,
            nextCustomerArrival     = genRandom(r, *inpCsmrFrequency, *inpStdDiviation);

        for(i = 0; i < *inpMaxQueueLength; i++) {
            queue[i][0] = -1;
        }
        for(i = 0; i < *inpNumServicePoints; i++) {
            servicePoint[i] = -1;
        }

        /* Loop until the single simulation is complete */
        while(clock < *inpClosingTime || occupiedServicePoints > 0) {

            /********************* service point management */
            for(i = 0; i < *inpNumServicePoints; i++) {
                

               /* printf("SP%d: %d\n", i + 1, servicePoint[i]);

                /*FUNC UPDATE SERVICE 
                    empty service point if complete */
                if(clock == servicePoint[i]) {

                    servicePoint[i] = -1;
                    occupiedServicePoints--;

                }

                /* Fill service point if empty, with waiting customer */
                if(servicePoint[i] == -1 && queue[*inpMaxQueueLength -1][0] > -1) {

                
                    /* move customer to service point, generate service time */
                    servicePoint[i] = clock + genRandom(r, *inpServicePointSpeed ,*inpStdDiviation);
                    occupiedServicePoints++;
                    
                    /* log wait time for customer       */
                    *outpAverageWaitTime += queue[*inpMaxQueueLength -1][1] + (clock - queue[*inpMaxQueueLength -1][0]);

                    /* shift queue */
                    lrsArray(queue, *inpMaxQueueLength);


                    /* log customer has been fulfilled */
                    *outpCsmrFulfilled += + 1;
                    
                }

            }
            /********************* csmr queue managment     */
            for(i = 0; i < *inpMaxQueueLength; i++) {

                /*  FUNC 
                    Customer leaves */
                if(clock == queue[i][0]) {

                    /* shift queue */
                    lrsArray(queue, i);

                    /*  log custmer timed out   */
                    *outpCsmrTimedOut+= + 1;
                }


                /* suffle queue empty spaces */
                if(queue[i][0] == -1) {
                    lrsArray(queue, i);
                }

            }

            /* Accept or Decline New customer */
            if (clock == nextCustomerArrival) {

                /* Accept  | if (space in queue & not closed) */
                if (queue[0][0] == -1 && clock <= *inpClosingTime) {

                    queue[0][0] = clock + genRandom(r, *inpCsmrWaitTolerance, *inpStdDiviation);
                    /* store arrival time */
                    queue[0][1] = queue[0][0];
                }
                /* Decline  */
                else {
                    *outpCsmrUnfulfilled+= + 1;;
                }
                nextCustomerArrival = clock + genRandom(r, *inpCsmrFrequency, *inpStdDiviation);
            }

            clock++;
        }
        /* log completed simulation outputs */
        *outpTimePostClosing += clock - *inpClosingTime;
    }

    /************* sim end ****************/


        printf("OUTPUT:\n%d\n%d\n%d\n%d\n%d\n", 
                        *outpCsmrFulfilled,
                        *outpCsmrUnfulfilled,
                        *outpCsmrTimedOut, 
                        *outpAverageWaitTime, 
                        *outpTimePostClosing);

    /*  turn each putput parameter into an average */

    *outpAverageWaitTime =  (*outpAverageWaitTime * numSims) / *outpCsmrFulfilled;
    for(i = 0; i < 5; i++) {
        outputParams[i] = outputParams[i] / numSims;
    }
        printf("OUTPUT:\n%d\n%d\n", 
                        outputParams[3],
                        outputParams[4]);

    writeOutputFile(fileOut, inputParams, outputParams);

    /*  free gsl memory     */
    gsl_rng_free(r);

    /*  exit program with success */
    exit(EXIT_SUCCESS);
}

int genRandom(gsl_rng *r, int lowerLimit, int diviation) {
    return (int)gsl_ran_gaussian_tail(r, lowerLimit ,diviation);
}

void lrsArray(int array[][2], int position) {
        
        int i;
        for(i = position; i > -1; i--) {
            /* wait time    */
            array[i][0] = array[i - 1][0];
            /* arrival time */
            array[i][1] = array[i - 1][1];
        }
        array[0][0] = -1;
}