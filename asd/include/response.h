#ifndef _RESPONSE_H_
#define _RESPONSE_H_

typedef struct Response
{
    char header[10];
    char retval[10];
    char out0[10];
    char messages[50];

    char checksum[10];
} Response;

#endif
