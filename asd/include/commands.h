#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include "serial.h"
#include "request.h"
#include "response.h"
#include "types.h"

int digitalWrite(int pin, PinState value);
int digitalRead(int pin);
int pinMode(int pin, PinMode mode);

#endif
