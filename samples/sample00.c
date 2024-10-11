#include "asd.h"

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

    int      pinNumber = 5;
    PinState pinValue;

    printf("1: Reading pinValue...\n");
    pinValue = digitalRead(pinNumber);
    printf("1: Read pinValue: %d\n", pinValue);

    printf("2: Setting pin to HIGH...\n ");
    digitalWrite(pinNumber, HIGH);
    printf("2: Set %d to HIGH\n", pinNumber);

    printf("3: Reading pinValue...\n ");
    pinValue = digitalRead(pinNumber);
    printf("3: Read pinValue: %d\n", pinValue);

    serialClose();

    return 0;
}
