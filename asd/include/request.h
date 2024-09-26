#ifndef _REQUEST_H_
#define _REQUEST_H_

typedef struct Request
{
    char* header;
    char* command;
    char* arg0;
    char* arg1;
    char* arg2;

    char* checksum;
} Request;

#endif
