#include "commands.h"
#include "serial.h"

int main(int argc, char* argv[]) {
    // serialBegin("/dev/pts/2");
    serialBegin("/dev/ttyUSB0");
    printf("started\n");
    sleep(5);
    printf("for realsies\n");

    PinState pin1 = digitalRead(1);
    printf("1: pin1: %d", pin1);

    digitalWrite(1, 1);
    printf("2: pin1: %d", pin1);
    
    serialClose();

    return 0;
}
