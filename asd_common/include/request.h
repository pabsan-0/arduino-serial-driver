#ifndef __REQUEST_H__
#define __REQUEST_H__

#include "misc.h"

#include <stdint.h>

struct Request
{
    char    header[10]; // Header identifier string
    Command cmd;        // Command, integer defined in misc.h
    uint8_t arg0;       // Argument 0 for cmd
    uint8_t arg1;       // Argument 1 for cmd
} __attribute__((__packed__));

typedef struct Request Request;

#endif
