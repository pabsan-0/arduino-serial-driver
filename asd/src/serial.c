#include "serial.h"

int __serial_port = -1;

int serialBegin()
{
    __serial_port = open("/dev/ttyUSB0", O_RDWR);
    if (__serial_port < 0)
    {
        printf("Error %i from open: %s\n", errno, strerror(errno));
        return 1;
    }

    // Configure the serial port
    struct termios tty;
    memset(&tty, 0, sizeof tty);

    if (tcgetattr(__serial_port, &tty) != 0)
    {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        return 1;
    }

    tty.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    tty.c_iflag = IGNPAR;
    tty.c_oflag = 0;
    tty.c_lflag = 0;

    tcflush(__serial_port, TCIFLUSH);
    tcsetattr(__serial_port, TCSANOW, &tty);
};

int serialClose()
{
    close(__serial_port);
};

int serialRequestResponse(Request request, Response response)
{
    write(__serial_port, &request, sizeof(Request));

    char read_buf[sizeof(Response)];
    memset(read_buf, '\0', sizeof(read_buf));

    usleep(100);

    int num_bytes = read(__serial_port, &read_buf, sizeof(Response) / 2);
    if (num_bytes < 0)
    {
        printf("Error reading: %s\n", strerror(errno));
        return 1;
    }

    // printf("%s", read_buf);
    memcpy(&response, read_buf, sizeof(Response));
};
