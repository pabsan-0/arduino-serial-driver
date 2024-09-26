#ifndef _REQUEST_H_
#define _REQUEST_H_

typedef struct Request
{
    char header[10];
    char command[20];
    char arg0[10];
    char arg1[10];
    char arg2[10];

    char checksum[10];
} Request;

#endif
