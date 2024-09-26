#ifndef ___SERIAL_H_
#define ___SERIAL_H_

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

#define SERIAL_BEGIN                                                     \
    int __serial_port = open("/dev/ttyUSB0", O_RDWR);                    \
    if (__serial_port < 0)                                               \
    {                                                                    \
        printf("Error %i from open: %s\n", errno, strerror(errno));      \
        return 1;                                                        \
    }                                                                    \
    struct termios tty;                                                  \
    memset(&tty, 0, sizeof tty);                                         \
    if (tcgetattr(__serial_port, &tty) != 0)                             \
    {                                                                    \
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno)); \
        return 1;                                                        \
    }                                                                    \
    tty.c_cflag = B9600 | CS8 | CLOCAL | CREAD;                          \
    tty.c_iflag = IGNPAR;                                                \
    tty.c_oflag = 0;                                                     \
    tty.c_lflag = 0;                                                     \
    tcflush(__serial_port, TCIFLUSH);                                    \
    tcsetattr(__serial_port, TCSANOW, &tty);

#endif
