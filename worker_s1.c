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

static char                 mq_name1_worker_s1[80];
static char                 mq_name2_worker_s1[80];

static void rsleep (int t);

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

message_queue_test (mqd_t S1_queue,mqd_t Rsp_queue)
{
    pid_t               processID;      /* Process ID from fork() */
    mqd_t               mq_fd_request;
    mqd_t               mq_fd_response;
    mq_fd_request = S1_queue;
    mq_fd_response = Rsp_queue;
    MQ_REQUEST_MESSAGE  req;
    MQ_RESPONSE_MESSAGE rsp;
    struct mq_attr      attr;

    sprintf (mq_name1, "/mq_request_%s_%d", STUDENT_NAME, getpid());
    sprintf (mq_name2, "/mq_response_%s_%d", STUDENT_NAME, getpid());

    attr.mq_maxmsg  = 10;
    attr.mq_msgsize = sizeof (MQ_REQUEST_MESSAGE);
    mq_fd_request = mq_open (mq_name1, O_WRONLY | O_CREAT | O_EXCL, 0600, &attr);

    attr.mq_maxmsg  = 10;
    attr.mq_msgsize = sizeof (MQ_RESPONSE_MESSAGE);
    mq_fd_response = mq_open (mq_name2, O_RDONLY | O_CREAT | O_EXCL, 0600, &attr);

    getattr(mq_fd_request);
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
            // remaining of the parent stuff
            
            // fill request message
            req.a = 73;
            req.b = 42;
            req.c = 'z';

            sleep (3);
            // send the request
            printf ("parent: sending...\n");
            mq_send (mq_fd_request, (char *) &req, sizeof (req), 0);

            sleep (3);
            // read the result and store it in the response message
            printf ("parent: receiving...\n");
            mq_receive (mq_fd_response, (char *) &rsp, sizeof (rsp), NULL);

            printf ("parent: received: %d, '", rsp.e);
            // printing characters of f[] separately:
            for (int i = 0; i < rsp.e; i++)
            {
                printf ("%c", rsp.f[i]);
            }
            // printing g[] in one step (because it has the \0-terminator):
            printf ("', '%s'\n", rsp.g);
    
            sleep (1);
    
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
    
    
    mqd_t               S1_queue;
    mqd_t               Rsp_queue;
    MQ_REQUEST_MESSAGE  req;
    MQ_RESPONSE_MESSAGE rsp;

    S1_queue = mq_open (mq_name1_worker_s1, O_RDONLY);
    Rsp_queue = mq_open (mq_fd_response53, O_WRONLY);

    
    while (S1_queue != NO_REQ)
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
    rsleep(10000);
    message_queue_test(S1_queue, Rsp_queue);

    char buffer[MAX_SIZE + 1];
    //reades the message intednded for worker 2
    int bytes_read = mq_receive(S1_queue, buffer, MAX_SIZE, NULL);
    //performs service2 on the given data
    rsp = service1(bytes_read);
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
