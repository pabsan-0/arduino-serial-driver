#include "commands.h"
#include "serial.h"

int main(int argc, char* argv[]) {
    serialBegin();

    int pin1 = digitalRead(1);
    printf("pin1: %d", pin1);

    digitalWrite(1, 1);
    printf("pin1: %d", pin1);

    return 0;
}
