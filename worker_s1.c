/* 
 * Operating Systems  (2INCO)  Practical Assignment
 * Interprocess Communication
 *
 * Alexandru Rosca (1453890)
 * Dragos Găleteanu  (1542672)
 * Matei Mihu (1561391)
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
#include "service1.h"

static char                 mq_name1_worker_s2[80];
static char                 mq_name2_worker_s2[80];



int main (int argc, char * argv[])
{
    // TODO:
    // (see message_queue_test() in interprocess_basic.c)
    //  * open the two message queues (whose names are provided in the
    //    arguments)
    //  * repeatingly:
    //      - read from the SX message queue the new job to do
    //      - wait a random amount of time (e.g. rsleep(10000);)
    //      - do the job 
    //      - write the results to the Rep message queue
    //    until there are no more tasks to do
    //  * close the message queues
    
    
    mqd_t               S2_queue;
    mqd_t               Rsp_queue;
    MQ_REQUEST_MESSAGE  req;
    MQ_RESPONSE_MESSAGE rsp;

    S2_queue = mq_open (mq_name1_worker_s2, O_RDONLY);
    Rsp_queue = mq_open (mq_fd_response53, O_WRONLY);

    
    
    while (S2_queue != NO_REQ)
    {
    int	i;
    // command-line arguments are available in argv[0] .. argv[argc-1]
    // argv[0] always contains the name of the program
    
    // check if the user has started this program with valid arguments
    if (argc != 1)
    {
        fprintf (stderr, "%s: %d arguments:\n", argv[0], argc);
        for (i = 1; i < argc; i++)
        {
            fprintf (stderr, "     '%s'\n", argv[i]);
        }
        exit (1);
    }

    
    process_test();
    //performes rsleep(10000) as asked for in the requirements 
    rsleep(10000);

    char buffer[MAX_SIZE + 1];
    //reades the message intednded for worker 2
    int bytes_read = mq_receive(S2_queue, buffer, MAX_SIZE, NULL);
    //performs service2 on the given data
    rsp = service2(bytes_read);
    char buffer[MAX_SIZE];
    //sends the response back on the response queue
    mq_send(Rsp_queue, buffer, MAX_SIZE, 0);
    }

    return (0);
}

/*
 * rsleep(int t)
 *
 * The calling thread will be suspended for a random amount of time
 * between 0 and t microseconds
 * At the first call, the random generator is seeded with the current time
 */
static void rsleep (int t)
{
    static bool first_call = true;
    
    if (first_call == true)
    {
        srandom (time (NULL) % getpid ());
        first_call = false;
    }
    usleep (random() % t);
}
