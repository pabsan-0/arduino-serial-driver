#ifndef __RESPONSE_H__
#define __RESPONSE_H__

#include "request.h"

#include <stdint.h>

#define RESPONSE_MSG_SIZE (50)

struct Response
{
    char    header[10];                  // Header identifier string
    char    messages[RESPONSE_MSG_SIZE]; // Error messages for the user
    int16_t retval;                      // 0: OK, 1: NOT OK
    int16_t out0;                        // Command output, if any
    //    Request req;                   // Request being answered
} __attribute__((__packed__));

typedef struct Response Response;

#endif
