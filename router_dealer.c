/* 
 * Operating Systems  (2INCO)  Practical Assignment
 * Interprocess Communication
 *
 * Alexandru Rosca (1453890)
 * Dragos Galeteanu (1542672)
 * Matei Mihu (1561391)
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


message_queue_child (void)
{
    mqd_t               mq_fd_request;
    mqd_t               mq_fd_response;
    MQ_REQUEST_MESSAGE  req;
    MQ_RESPONSE_MESSAGE rsp;

    mq_fd_request = mq_open (mq_name1, O_RDONLY);
    mq_fd_response = mq_open (mq_name2, O_WRONLY);

    // read the message queue and store it in the request message
    printf ("                                   child: receiving...\n");
    mq_receive (mq_fd_request, (char *) &req, sizeof (req), NULL);

    printf ("                                   child: received: %d, %d, '%c'\n",
            req.a, req.b, req.c);
    
    // fill response message
    // e   contains the length of f[]
    // f[] contains characters abcd
    // g[] contains characters ABCDEFGHIJ + \0-terminator (so it is a C-string)
    rsp.e = 4;	// set 'e' to the number of characters in f[]
    for (int i = 0; i < rsp.e; i++)
    {
        rsp.f[i] = 'a' + i;
    }
    for (int i = 0; i < 10; i++)
    {
        rsp.g[i] = 'A' + i;
    }
    rsp.g[10] = '\0';

    sleep (3);
    // send the response
    printf ("                                   child: sending...\n");
    mq_send (mq_fd_response, (char *) &rsp, sizeof (rsp), 0);

    mq_close (mq_fd_response);
    mq_close (mq_fd_request);
}
//function that sends message to a queue
message_queue (MQ_REQUEST_MESSAGE rsq, mqd_t mq_name)
{
    pid_t               processID;      /* Process ID from fork() */
    MQ_RESPONSE_MESSAGE rsp;
    struct mq_attr      attr;
    sprintf (mq_name2, "/mq_response_%s_%d", STUDENT_NAME, getpid());
    attr.mq_maxmsg  = 10;
    attr.mq_msgsize = sizeof (MQ_RESPONSE_MESSAGE);
    mq_fd_response = mq_open (mq_name, O_RDONLY | O_CREAT | O_EXCL, 0600, &attr);
    getattr(mq_fd_response);
    processID = fork();
    if (processID < 0)
    {
        perror("fork() failed");
        exit (1);
    }
    else
    {
        if (processID == 0)
        {
            // child-stuff
            message_queue_child ();
            exit (0);
        }
        else
        {
            mq_send (mq_fd_response, (char *) &rsp, sizeof (rsp), 0);

    
            waitpid (processID, NULL, 0);   // wait for the child
            
            mq_close (mq_fd_response);
            mq_close (mq_fd_request);
            mq_unlink (mq_name1);
            mq_unlink (mq_name2);   
        }
    }
}


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

    mq_request = mq_open (mq_fd_request53, O_RDONLY);

    S1_queue = mq_open (mq_name1_worker_s1, O_WRONLY);
    S2_queue = mq_open (mq_name1_worker_s2, O_WRONLY);
    Rsp_queue = mq_open (mq_fd_response53, O_RDONLY);


    while (mq_request != NO_REQ && !Rsp_queue.empty){
        //reading requests from request queue
        mq_receive (mq_fd_request53, (char *) &req, sizeof (rsp), NULL);
        //Check the service ID 
        if(req.serviceID = 1){
            //sends job to worker 1 
            message_queue(req, S1_queue);
        }else{
            //sends job to worker 2
            message_queue(req, S2_queue);
        }
        attr.mq_maxmsg  = 10;
        attr.mq_msgsize = sizeof (MQ_REQUEST_MESSAGE);
        //reading responses from response queue
        mq_receive (Rsp_queue, (char *) &rsp, sizeof (rsp), NULL);
        //printing the responses from the workers
        printf(rsp);
        
    }


    
    //closes S1/S2 and response queue after there is no more work to do
    mq_close (S1_queue);
    mq_close (S2_queue);
    mq_close (Rsp_queue);

    return (0);
}
