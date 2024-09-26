#ifndef _RESPONSE_H_
#define _RESPONSE_H_

typedef struct Response
{
    char* header;
    char* retval;
    char* out0;
    char* messages;

    char* checksum;
} Response;

#endif
