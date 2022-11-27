/* 
 * Operating Systems {2INCO} Practical Assignment
 * Interprocess Communication
 *
 * Contains definitions of message data structures
 *
 */

#ifndef MESSAGES_H
#define MESSAGES_H

// define the data structures for your messages here

typedef struct
{
    int a;
}MQ_REQUEST_MESSAGE;

typedef struct
{
    int x;
}MQ_RESPONSE_MESSAGE;

#endif

