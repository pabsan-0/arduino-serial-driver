#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <time.h>


#define SERIAL_CHARWISE_DEBUG (1)
#define ASCII_CR (10) // \r in arduino is not received as ascii 10


// Define the same struct used in Arduino
// int and long sizes mismatch between arduino and pc, hence we specify either bit fields or compiler-independent types
// https://en.cppreference.com/w/c/types/integer
struct structTable {
    int16_t  a:4;
    long b:4;
    char c[4];
    char d;
} __attribute__((__packed__));

typedef struct structTable structTable;


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

int SerialWrite(const int serial_port, const structTable data) {
    write(serial_port, &data, sizeof(structTable));
    write(serial_port, "\r", 1);  // Send newline to mark end of message
}


int SerialReadUntil(const int serial_port, const char cc, const int timeout_sec, structTable *dest) {

    char buffer[sizeof(structTable) + 1]; // struct + CR
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
            if (bytes_read >= sizeof(structTable) + 1) {
                return 0; 
            }
        }
        if (difftime(time(NULL), start_time) > 5) {  // Timeout after 5 seconds
            return 0;
        }
    }

    // Copy the received bytes into the Response struct
    memcpy(dest, buffer, sizeof(structTable));
    return 1;
}


int main() {

    time_t start_time;
    int allowed_sec;

    const int serial_port = SerialBegin();

    // Create and populate a struct instance
    structTable dataToSend;
    dataToSend.a = 0;
    dataToSend.b = 1;
    strncpy(dataToSend.c, "hey", 4);
    dataToSend.d = 'a';

    // Send the struct over the serial port
    const char delimiter[2] = "\r";
    
   
    // Init
    printf("Allowing some time for the arduino to reset...\n");
    fflush(stdout);
    sleep(3);

    printf("1. Expected Success ---- writing data and read-parsing into struct\n\n");
    
    structTable rec;
    SerialWrite(serial_port, dataToSend);
    SerialReadUntil(serial_port, ASCII_CR, 4, &rec);
    printf("a: %d, b: %d, c: %s, d: %c\n",
        rec.a,
        rec.b,
        rec.c,
        rec.d
    );
}
