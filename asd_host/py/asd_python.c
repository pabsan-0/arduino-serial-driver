#include "commands.h"
#include "serial.h"
#include "misc.h"

/*
 * Boilerplate for python interface
 */

int py_serialBegin(char* port)
{
    return serialBegin(port);
}

int py_serialClose()
{
    return serialClose();
}

void py_digitalWrite(int pin, PinState value)
{
    digitalWrite(pin, value);
}

PinState py_digitalRead(int pin)
{
    return digitalRead(pin);
}

void py_pinMode(int pin, PinMode mode)
{
    pinMode(pin, mode);
}
