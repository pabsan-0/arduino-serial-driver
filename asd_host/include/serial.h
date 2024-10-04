#ifndef __SERIAL_H__
#define __SERIAL_H__

#include "request.h"
#include "response.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <time.h>

#define SERIAL_TIMEOUT (5)     // seconds
#define SERIAL_RETRY_LIMIT (3) // serial attempts

enum SerialError
{
    SERIAL_ERROR_TIMEOUT,  // No serial response in time
    SERIAL_ERROR_OVERFLOW, // Too many incoming bytes
    SERIAL_ERROR_NONE      // OK
};
typedef enum SerialError SerialError;

int serialBegin(char* port);
int serialClose();
int serialRequestResponse(Request request, Response response, int recursion_lvl);

#endif
