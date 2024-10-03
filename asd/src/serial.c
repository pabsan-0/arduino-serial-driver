#include "serial.h"

static int serial_port = -1;

int serialBegin(char* port)
{
    if (serial_port != -1) {
        printf("Error: trying to initialize an open serial port.\n");
        return -1;
    }

    serial_port = open(port, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (serial_port < 0) {
        printf("Error %i from open: %s\n", errno, strerror(errno));
        return -1;
    }

    struct termios tty;
    memset(&tty, 0, sizeof tty);
    if (tcgetattr(serial_port, &tty) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        return -1;
    }

    // 8N1
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= B9600 | CS8 | CLOCAL | CREAD;

    tcflush(serial_port, TCIFLUSH);
    tcsetattr(serial_port, TCSANOW, &tty);

    return serial_port;
};

int serialClose()
{
    // There may be no port to close
    if (serial_port == -1)
    {
        return -1;
    }

    // Else close and reset its value 
    close(serial_port);
    serial_port = -1;
    return 0;
};

static int serialSendRequest(const Request data) 
{
    write(serial_port, &data, sizeof(Request));
    write(serial_port, "\r", 1);  // Send newline to mark end of message
}

static SerialError serialReadResponse(Response *dest, const int timeout_sec)
{
    char buffer[sizeof(Response) + 1]; // struct + delimiter
    int  bytes_read = 0;
    time_t start_time = time(NULL);

    // Read chars one by one until timeout or matching delimiter
    while (difftime(time(NULL), start_time) < timeout_sec) {
        int n = read(serial_port, &buffer[bytes_read], 1);
        if (n > 0) {

            if (buffer[bytes_read] == SERIAL_DELIMITER) {
                memcpy(dest, buffer, sizeof(Request));
                return SERIAL_ERROR_OK;
            }

            bytes_read += n;
            if (bytes_read >= sizeof(Request) + 1) {
                return SERIAL_ERROR_OVERFLOW; 
            }
        }
    }

    return SERIAL_ERROR_TIMEOUT;
}

int serialRequestResponse(Request request, Response response)
{
    SerialSendRequest(request);
    SerialError ret = SerialReadUntil(response, SERIAL_TIMEOUT);

    // Check serial interchange
    switch (ret) {
        case SERIAL_ERROR_TIMEOUT:
            return serialRequestResponse(request, response)
        case SERIAL_ERROR_OVERFLOW:
            return serialRequestResponse(request, response)
        case SERIAL_ERROR_OK:
            break;
        default: 
            break;
    }

    return 0;
};
