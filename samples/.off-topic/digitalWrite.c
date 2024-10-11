#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

typedef struct 
{
    char header[255];
    char command[255];
    char arg0[255];
    char arg1[255];
    char arg2[255];

    char checksum[255];
} Request;

int serial_setup () {
    
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
    
    return serial_port;
}

int main() {

    int serial_port = serial_setup();

    // Create and populate a struct instance
    Request req;
    strcpy(req.header, "pc->arduino");
    strcpy(req.command, "digitalWrite");
    strcpy(req.arg0, "2");
    strcpy(req.arg1, "HIGH");
    strcpy(req.arg2, "");
    strcpy(req.checksum, "1");

    // Send the struct over the serial port
    write(serial_port, &req, sizeof(req));

    // Read back the response from the Arduino
    char read_buf[8 * 256];  // Buffer to store response
    memset(&read_buf, '\0', sizeof(read_buf));

    // Wait for Arduino to process and send back the response
    usleep(1000); 

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
