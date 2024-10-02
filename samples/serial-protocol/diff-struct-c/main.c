#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <time.h>

#include "../diff-struct-common/reqres.h"

#define SERIAL_CHARWISE_DEBUG (1)
#define ASCII_CR (10) // \r in arduino is not received as ascii 10


int SerialBegin () {

    // Open the serial port
    int serial_port = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NONBLOCK);  // Adjust the path for your serial port

    if (serial_port < 0) {
        printf("Error %i from open: %s\n", errno, strerror(errno));
        return 1;
    }

    // Configure the serial port
    struct termios tty;
    memset(&tty, 0, sizeof tty);

    if (tcgetattr(serial_port, &tty) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        return 1;
    }

    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= B9600 | CS8 | CLOCAL | CREAD;

    // tty.c_iflag = IGNPAR;
    // tty.c_oflag = 0;
    // tty.c_lflag = 0;

    tcflush(serial_port, TCIFLUSH);
    tcsetattr(serial_port, TCSANOW, &tty);

    return serial_port;
}

int SerialWrite(const int serial_port, const Request data) {
    write(serial_port, &data, sizeof(Request));
    write(serial_port, "\r", 1);  // Send newline to mark end of message
}


int SerialReadUntil(const int serial_port, const char cc, const int timeout_sec, Response *dest) {

    char buffer[sizeof(Response) + 1]; // struct + CR
    int bytes_read = 0;
    time_t start_time = time(NULL);

    while (1) {
        int n = read(serial_port, &buffer[bytes_read], 1);

        if (n > 0) {

#if SERIAL_CHARWISE_DEBUG
            printf("\t'%c':'%d'\n", buffer[bytes_read], buffer[bytes_read]);
            fflush(stdout);
#endif

            if (buffer[bytes_read] == cc) {
                break;
            }

            bytes_read += n;
            if (bytes_read >= sizeof(Request) + 1) {
                return 0; 
            }
        }
        if (difftime(time(NULL), start_time) > 5) {  // Timeout after 5 seconds
            return 0;
        }
    }

    // Copy the received bytes into the Response struct
    memcpy(dest, buffer, sizeof(Request));
    return 1;
}


int main() {

    time_t start_time;
    int allowed_sec;

    const int serial_port = SerialBegin();

    // Create and populate a struct instance
    Request req;
    req.a = 1;
    strncpy(req.b, "hey", 4);
    
    // Send the struct over the serial port
    const char delimiter[2] = "\r";
    
    // Init
    printf("Allowing some time for the arduino to reset...\n");
    fflush(stdout);
    sleep(3);

    printf("1. Expected Success ---- writing data and read-parsing into struct\n\n");
    
    Response res;
    SerialWrite(serial_port, req);
    SerialReadUntil(serial_port, ASCII_CR, 4, &res);
    printf("a: %d, b: %s\n",
        res.a,
        res.b
    );
}
