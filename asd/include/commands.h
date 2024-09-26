#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include "serial.h"
#include "request.h"
#include "response.h"

int digitalWrite(int pin, int value);
int digitalRead(int pin);
int pinMode(int pin, int mode);

#endif
