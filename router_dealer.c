/* 
 * Operating Systems  (2INCO)  Practical Assignment
 * Interprocess Communication
 *
 * Alexandru Rosca (1453890)
 * Dragos Galeteanu (1542672)
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
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>    
#include <unistd.h>    // for execlp
#include <mqueue.h>    // for mq


#include "settings.h"  
#include "messages.h"


int main (int argc, char * argv[])
{
    if (argc != 1)
    {
        fprintf (stderr, "%s: invalid arguments\n", argv[0]);
    }
        
    // TODO:
    //  * create the message queues (see message_queue_test() in
    //    interprocess_basic.c)
    //  * create the child processes (see process_test() and
    //    message_queue_test())
    //  * keep a dictionarry mapping clients' PIDs to a clientID
    //  * read requests from the Req queue and transfer them to services
    //  * read answers from services in the Rep queue and print them
    //  * wait until the clients have been stopped (see process_test())
    //  * clean up the message queues (see message_queue_test())

    // Important notice: make sure that the names of the message queues
    // contain your goup number (to ensure uniqueness during testing)

    mqd_t               mq_fd_request53;
    mqd_t               mq_fd_response53;
    MQ_REQUEST_MESSAGE  req;
    MQ_RESPONSE_MESSAGE rsp;

    pid_t           processID;
    printf ("parent pid:%d\n", getpid());
    processID = fork();

    mq_fd_request53 = mq_open ()

    return (0);
}
