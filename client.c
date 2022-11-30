/* 
 * Operating Systems  (2INCO)  Practical Assignment
 * Interprocess Communication
 *
 * Alexandru Rosca (1453890)
 * STUDENT_NAME_2 (STUDENT_NR_2)
 *
 * Grading:
 * Your work will be evaluated based on the following criteria:
 * - Satisfaction of all the specifications
 * - Correctness of the program
 * - Coding style
 * - Report quality
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>      // for perror()
#include <unistd.h>     // for getpid()
#include <mqueue.h>     // for mq-stuff
#include <time.h>       // for time()

#include "messages.h"
#include "request.h"

#include "request.c"

#define GROUP_NR "group 53"

static char                 mq_client[80];

static void rsleep (int t);


int main (int argc, char * argv[])
{

    // TODO:
    // (see message_queue_test() in interprocess_basic.c)
    //  * open the message queue (whose name is provided in the
    //    arguments)    
    //  * repeatingly:
    //      - get the next job request 
    //      - send the request to the Req message queue
    //    until there are no more requests to send
    //  * close the message queue


    mqd_t               Req_queue;
    MQ_REQUEST_MESSAGE  req;

    sprintf(mq_client, "/Req_queue_%s_%d", GROUP_NR, getpid());

    Req_queue= mq_open (mq_client, O_RDONLY);

    
    int jobID;
    int data;
    int serviceID;

    while (getNextRequest(&jobID, &data, &serviceID) != NO_REQ)
    {
        req.jobID = jobID;
        req.data = data;
        req.serviceID = serviceID;

        printf("client sending job with ID %d...\n" , req.jobID);
        mq_send (Req_queue, (char *) &req, sizeof (req), 0);
    }
    
    mq_close (Req_queue);
    
    return (0);
}