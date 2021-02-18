#include "queueSystem.h"

/***    Customer Queue Management       ***/

/** Checks if a customer has timed out **/
int checkCsmrTimeout(int csmrQueue[][2], int maxQueueLength, int clock) {
    
    int i,
        hasTimedOut = 0;
    
    for(i = 0; i < maxQueueLength; i++) {

        /*  Customer leaves     */
        if(clock == csmrQueue[i][0]) {
            
            /* shift the customer queue */
            lrsArray(csmrQueue, i);

            /*  log custmer timed out   */
            hasTimedOut = 1;
        }

        /* Suffle csmrQueue's empty spaces */
        if(csmrQueue[i][0] == -1) {
            lrsArray(csmrQueue, i);
        }
    }
    return hasTimedOut;
}

/** Checks if a new customer has arrived, then accepts of declines them */
int checkForNewCsmr(int csmrQueue[][2], int *nextCsmrArrival, int clock, gsl_rng *r, int *inputParams) {

    int closingTime         = inputParams[2],
        stdDiviation        = inputParams[3],
        csmrFrequency       = inputParams[4],
        csmrWaitTolerance   = inputParams[5],
        csmrDeclined        = 0;

    /* Check for new customer */
    if (clock == *nextCsmrArrival) {

        /* Accept  | if (space in csmrQueue & not closed) */
        if (csmrQueue[0][0] == -1 && clock <= closingTime) {

            csmrQueue[0][0] = clock + genRandom(r, csmrWaitTolerance, stdDiviation);
            /* store arrival time */
            csmrQueue[0][1] = csmrQueue[0][0];
        }
        /* Decline  */
        else {
            csmrDeclined = 1;
        }
        *nextCsmrArrival = clock + genRandom(r, csmrFrequency, stdDiviation);
    }
    return csmrDeclined;
}


/***    Service Point Management       ***/

/** checks if the service point has finished serving the customer **/
int isServiceComplete(int *servicePoint, int index, int clock) {

    int serviceFinished = 0;

    if(clock == servicePoint[index]) {

        servicePoint[index] = -1;
        serviceFinished = -1;
    }
    return serviceFinished;
}

/**  Checks if each service point is finished and/or get the next customer   **/
int checkServicePoints(int *servicePoint, int csmrQueue[][2], int *inputParams, int clock, gsl_rng *r, int *outputParams) {

    int i,
        maxQueueLength          = inputParams[0],
        numServicePoints        = inputParams[1],
        stdDiviation            = inputParams[3],
        servicePointSpeed       = inputParams[6],
        occupiedServicePoints   = 0;

    for(i = 0; i < numServicePoints; i++) {
                
        occupiedServicePoints += isServiceComplete(servicePoint, i, clock);

        /* Fill service point if empty, with waiting customer */
        if(servicePoint[i] == -1 && csmrQueue[maxQueueLength -1][0] > -1) {
    
            /* move customer to service point, generate service time */
            servicePoint[i] = clock + genRandom(r, servicePointSpeed ,stdDiviation);
            occupiedServicePoints++;
                    
            /* log: wait time for customer       */
            outputParams[3] += csmrQueue[maxQueueLength -1][1] + (clock - csmrQueue[maxQueueLength -1][0]);

            /* shift the customer queue */
            lrsArray(csmrQueue, maxQueueLength);

            /* log: customer has been fulfilled */
            outputParams[0] += + 1;           
        }
    }
    return occupiedServicePoints;
}

/***    Logical Right Shift Array   ***/
void lrsArray(int array[][2], int position) {
        
    int i;
    /* stacks all customers in the queue to the front */
    for(i = position; i > -1; i--) {

        /* wait time    */
        array[i][0] = array[i - 1][0];
        /* arrival time */
        array[i][1] = array[i - 1][1];
    }
    array[0][0] = -1;
}

/** counts each customer in the queue **/
int countCsmrQueue(int csmrQueue[][2], int maxQueueLength) {

    int i       =0,
        counter =0;
    for (i = 0; i < maxQueueLength; i++) {
        if(csmrQueue[i][0] > -1) {
            csmrQueue ++;
        }
    }
    return counter;
}