#ifndef __REQUEST_H__
#define __REQUEST_H__

#include "misc.h"

#include <stdint.h>

// Make strings multiple of 8 to avoid garbage (empyrical)
#define REQUEST_HEADER_SIZE (8)

#define REQUEST_PRINT(req)                     \
    printf("Request { '%s' , %d , %d, %d }\n", \
        req.header,                            \
        req.cmd,                               \
        req.arg0,                              \
        req.arg1);                             \
    fflush(stdout);

struct Request
{
    char    header[REQUEST_HEADER_SIZE]; // Header identifier string
    int16_t cmd;                         // Command, integer defined in misc.h
    int16_t arg0;                        // Argument 0 for cmd
    int16_t arg1;                        // Argument 1 for cmd
} __attribute__((__packed__));

typedef struct Request Request;

#endif
