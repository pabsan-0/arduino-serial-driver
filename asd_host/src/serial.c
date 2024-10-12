#include "serial.h"

static int serial_port = -1;

int serialBegin(char* port)
{
    if (serial_port != -1)
    {
        printf("Error: trying to initialize an open serial port.\n");
        return -1;
    }

    serial_port = open(port, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (serial_port < 0)
    {
        printf("Error %i from open: %s\n", errno, strerror(errno));
        return -1;
    }

    struct termios tty;
    memset(&tty, 0, sizeof tty);
    if (tcgetattr(serial_port, &tty) != 0)
    {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        return -1;
    }

    // 8N1
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= B9600 | CS8 | CLOCAL | CREAD;

    // Raw input: else issues on consecutive serial comms
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    tty.c_oflag &= ~OPOST;

    // For reference: taken from https://github.com/todbot/arduino-serial/blob/main/arduino-serial-lib.c
    //
    // tty.c_cflag &= ~CRTSCTS;
    // tty.c_cflag &= ~HUPCL;
    // tty.c_iflag &= ~(IXON | IXOFF | IXANY);         // turn off s/w flow ctrl
    // tty.c_cc[VMIN]  = 0;
    // tty.c_cc[VTIME] = 0;
    // tty.c_cc[VTIME] = 20;

    tcflush(serial_port, TCIFLUSH);
    tcsetattr(serial_port, TCSANOW, &tty);

    return serial_port;
}

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
}

static int serialSendRequest(const Request data)
{
    char delim_str[] = { SERIAL_DELIMITER }; // allow string usage for macro

    write(serial_port, (char*)&data, sizeof(Request));
    write(serial_port, (char*)delim_str, 1);

    return 0;
}

static SerialError serialReadResponse(Response* dest, const int timeout_sec)
{
    memset(dest, 0, sizeof(Response));

    char   buffer[sizeof(Response) + 1]; // struct + delimiter
    int    bytes_read = 0;
    time_t start_time = time(NULL);

    // Read chars one by one until timeout or matching delimiter
    while (difftime(time(NULL), start_time) < timeout_sec)
    {
        int n = read(serial_port, &buffer[bytes_read], 1);
        if (n > 0)
        {
            bytes_read += n;

            // Debugging: print incoming chars one by one
            // fflush(stdout);
            // printf("\t'%c':'%d'\n", buffer[bytes_read - 1], buffer[bytes_read - 1]);
            // fflush(stdout);

            // Have I read more bytes than response + delimiter?
            if (bytes_read > sizeof(Response) + 1)
            {
                // printf("Exceeded bytes!");
                printf("Expected %ld bytes plus 1 delimiter , got %d\n", sizeof(Response), bytes_read);
                return SERIAL_ERROR_OVERFLOW;
            }

            // Is this incoming byte the delimiter?
            if (buffer[bytes_read - 1] == SERIAL_DELIMITER_CHEAT)
            {
                memcpy(dest, buffer, sizeof(Response));
                return SERIAL_ERROR_NONE;
            }
        }
    }

    if (bytes_read > 0)
    {
        printf("Got %d/%ld bytes but delimiter never came! {\n", bytes_read, sizeof(Response) + 1);
        printf("\t'%c':'%d'\n", SERIAL_DELIMITER_CHEAT, SERIAL_DELIMITER_CHEAT);
        printf("}\n");
    }
    return SERIAL_ERROR_TIMEOUT;
}

int serialRequestResponse(Request request, Response* response_ptr, int recursion_lvl)
{
    if (recursion_lvl > SERIAL_RETRY_LIMIT)
    {
        printf("Reached retry limit. Failed Serial comms.");
        return -1;
    }

    serialSendRequest(request);
    SerialError ret = serialReadResponse(response_ptr, SERIAL_TIMEOUT);

    // Check serial interchange
    switch (ret)
    {
        case SERIAL_ERROR_TIMEOUT:
            printf("Serial timed out, retrying...\n");
            return serialRequestResponse(request, response_ptr, ++recursion_lvl);
        case SERIAL_ERROR_OVERFLOW:
            printf("Serial got overflowing bytes, retrying...\n");
            return serialRequestResponse(request, response_ptr, ++recursion_lvl);
        case SERIAL_ERROR_NONE:
            // printf("Serial OK\n");
            usleep(100);
            break;
        default:
            printf("Serial got unexpected outcome. Ignoring...\n");
            break;
    }

    return 0;
}
