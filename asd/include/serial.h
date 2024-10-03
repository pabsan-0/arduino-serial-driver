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

#define SERIAL_DELIMITER (10) // ascii char
#define SERIAL_TIMEOUT (5)    // seconds

enum SerialError
{
    SERIAL_ERROR_TIMEOUT,  // No serial response in time
    SERIAL_ERROR_OVERFLOW, // Too many incoming bytes
    SERIAL_ERROR_REMOTE,   // Misc error, info in Arduino's msg
    SERIAL_ERROR_NONE      // OK
};
typedef enum SerialError SerialError;

int serialBegin(char* port);
int serialClose();
int serialRequestResponse(Request request, Response response);

#endif
