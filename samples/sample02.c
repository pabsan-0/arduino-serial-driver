#include "asd.h"

#include <time.h>

/*
 * This sample makes a benchmarking on serial speeds
 */

int main(int argc, char* argv[])
{
    // Boot serial port, can be argument
    char* port = (argc == 2) ? argv[1] : "/dev/ttyUSB0";
    printf("Using port '%s'\n", port);

    if (serialBegin(port) == -1)
    {
        return 1;
    }

    {
        // Boot arduino
        time_t start_time = time(NULL);
        int    wait_sec   = 3.0;
        while (difftime(time(NULL), start_time) <= wait_sec)
        {
            int percentage = difftime(time(NULL), start_time) / wait_sec * 100.0;
            printf("\rWaking arduino... %d%%", percentage);
            usleep(10000);
        }
        printf(" Ready!\n\n");
    }

    // How many calls in X seconds?
    int pinNumber = 3;
    {
        time_t start_time = time(NULL);
        float  total_time = 3.0;
        int    times;
        for (times = 0; difftime(time(NULL), start_time) <= total_time; times++)
        {
            digitalRead(pinNumber);
        }
        float calls_per_second = times / total_time;
        printf("digitalRead: \t%f calls per second\n", calls_per_second);
        printf("             \t%d times in %f seconds\n", times, total_time);
    }

    {
        time_t start_time = time(NULL);
        float  total_time = 3.0;
        int    times;
        for (times = 0; difftime(time(NULL), start_time) <= total_time; times++)
        {
            digitalWrite(pinNumber, HIGH);
        }
        float calls_per_second = times / total_time;
        printf("digitalWrite: \t%f calls per second\n", calls_per_second);
        printf("              \t%d times in %f seconds\n", times, total_time);
    }

    {
        time_t start_time = time(NULL);
        float  total_time = 3.0;
        int    times;
        for (times = 0; difftime(time(NULL), start_time) <= total_time; times++)
        {
            pinMode(pinNumber, OUTPUT);
        }
        float calls_per_second = times / total_time;
        printf("pinMode: \t%f calls per second\n", calls_per_second);
        printf("         \t%d times in %f seconds\n", times, total_time);
    }

    serialClose();

    return 0;
}
