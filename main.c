#include "commands.h"
#include "serial.h"

int main(int argc, char* argv[]) {
    serialBegin();

    printf("started\n");

    int pin1 = digitalRead(1);
    printf("1: pin1: %d", pin1);

    digitalWrite(1, 1);
    printf("2: pin1: %d", pin1);

    return 0;
}
