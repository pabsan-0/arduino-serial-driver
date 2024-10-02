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

int serialBegin();
int serialClose();
int serialRequestResponse(Request request, Response response);

#endif
