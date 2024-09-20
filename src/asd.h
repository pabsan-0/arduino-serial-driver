#include "request.h"
#include "response.h"

int serialBegin(int& serial_port)
{
    serial_port = open("/dev/ttyUSB0", O_RDWR);
    if (serial_port < 0)
    {
        printf("Error %i from open: %s\n", errno, strerror(errno));
        return 1;
    }

    // Configure the serial port
    struct termios tty;
    memset(&tty, 0, sizeof tty);

    if (tcgetattr(serial_port, &tty) != 0)
    {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        return 1;
    }

    tty.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    tty.c_iflag = IGNPAR;
    tty.c_oflag = 0;
    tty.c_lflag = 0;

    tcflush(serial_port, TCIFLUSH);
    tcsetattr(serial_port, TCSANOW, &tty);
};

int serialClose(int serial_port)
{
    close(serial_port);
};

int serialRequestResponse(int& request, int& response, int sleeptime)
{
    write(serial_port, &request, sizeof(Request));

    char read_buf[sizeof(Response)];
    memset(read_buf, '\0', sizeof(read_buf));

    usleep(100);
    if (sleeptime < 0)
    {
        usleep(sleeptime);
    }

    int num_bytes = read(serial_port, &read_buf, sizeof(Response) / 2);
    if (num_bytes < 0)
    {
        printf("Error reading: %s\n", strerror(errno));
        return 1;
    }

    // printf("%s", read_buf);
    memcpy(&response, read_buf, sizeof(Response));
};

int digitalWrite(int pin, int value)
{
    Request  req;
    Response res;

    req.cmd  = pin;
    req.arg0 = value;

    serialRequestResponse(req, res);

    return res.retval;
};

int digitalRead();
int pinMode();
