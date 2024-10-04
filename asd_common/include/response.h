#ifndef __RESPONSE_H__
#define __RESPONSE_H__

#include "request.h"

#include <stdint.h>

// Make strings multiple of 8 to avoid garbage (empyrical)
#define RESPONSE_HEADER_SIZE (8)
#define RESPONSE_MSG_SIZE (16)

#define RESPONSE_PRINT(res)                     \
    printf("Response { '%s' , %s , %d, %d }\n", \
        res.header,                             \
        res.messages,                           \
        res.retval,                             \
        res.out0);                              \
    fflush(stdout);

struct Response
{
    char    header[RESPONSE_HEADER_SIZE]; // Header identifier string
    char    messages[RESPONSE_MSG_SIZE];  // Error messages for the user
    int16_t retval;                       // 0: OK, 1: NOT OK
    int16_t out0;                         // Command output, if any
    //    Request req;                    // Request being answered
} __attribute__((__packed__));

typedef struct Response Response;

#endif
