#ifndef __RESPONSE_H__
#define __RESPONSE_H__

#include "request.h"

#include <stdint.h>

struct Response
{
    char    header[10];   // Header identifier string
    char    messages[50]; // Error messages for the user
    uint8_t retval;       // 0: OK, 1: NOT OK
    uint8_t out0;         // Command output, if any
    Request req;          // Request being answered
} __attribute__((__packed__));

typedef struct Response Response;

#endif
