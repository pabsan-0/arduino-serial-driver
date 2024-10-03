#ifndef __REQUEST_H__
#define __REQUEST_H__

#include "misc.h"

#include <stdint.h>

struct Request
{
    char    header[16]; // Header identifier string
    int16_t cmd;        // Command, integer defined in misc.h
    int16_t arg0;       // Argument 0 for cmd
    int16_t arg1;       // Argument 1 for cmd
} __attribute__((__packed__));

typedef struct Request Request;

#endif
