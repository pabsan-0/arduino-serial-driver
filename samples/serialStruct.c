#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

// Define the same struct used in Arduino
typedef struct {
    unsigned int page:1; // (0,1)
    unsigned int cric:4; // 10 choices (4 bits)
    unsigned int crac:3; // 5 choices (3 bits)
    unsigned int croc:2; // 3 choices (2 bits)
} structTable;

int main() {
    // Open the serial port
    int serial_port = open("/dev/ttyUSB0", O_RDWR);  // Adjust the path for your serial port

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

    tty.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    tty.c_iflag = IGNPAR;
    tty.c_oflag = 0;
    tty.c_lflag = 0;

    tcflush(serial_port, TCIFLUSH);
    tcsetattr(serial_port, TCSANOW, &tty);

    // Create and populate a struct instance
    structTable dataToSend;
    dataToSend.page = 1;
    dataToSend.cric = 9;  // Example data
    dataToSend.crac = 5;  // Example data
    dataToSend.croc = 2;  // Example data

    // Send the struct over the serial port
    write(serial_port, &dataToSend, sizeof(structTable));

    // Read back the response from the Arduino
    char read_buf[22];  // Buffer to store response
    memset(&read_buf, '\0', sizeof(read_buf));

    // Wait for Arduino to process and send back the response
    usleep(1000);  // Sleep for 100 milliseconds

    int num_bytes = read(serial_port, &read_buf, sizeof(read_buf));
    if (num_bytes < 0) {
        printf("Error reading: %s\n", strerror(errno));
        return 1;
    }

    // Print the Arduino response
    printf("Arduino Response: %s\n", read_buf);

    // Close the serial port
    close(serial_port);

    return 0;
}
