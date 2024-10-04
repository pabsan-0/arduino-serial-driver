#include "commands.h"
#include "serial.h"

#include <time.h>

int main(int argc, char* argv[])
{
    // Boot serial port, can be argument
    char* port = (argc == 2) ? argv[1] : "/dev/ttyUSB0";
    printf("Using port '%s'\n", port);

    if (serialBegin(port) == -1)
    {
        return 1;
    }

    // Wait for arduino, as it resets on serial connection
    time_t start_time = time(NULL);
    int    wait_sec   = 3.0;
    while (difftime(time(NULL), start_time) <= wait_sec)
    {
        int percentage = difftime(time(NULL), start_time) / wait_sec * 100.0;
        printf("\rWaking arduino... %d%%", percentage);
        usleep(10000);
    }
    printf(" Ready!\n");

    PinState pin1 = digitalRead(5);
    printf("1: Read pin1: %d\n", pin1);

    digitalWrite(5, HIGH);
    printf("2: Set pin1 to HIGH\n");

    pin1 = digitalRead(5);
    printf("3: Read pin1: %d\n", pin1);

    serialClose();

    return 0;
}
