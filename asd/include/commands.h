#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include "serial.h"
#include "misc.h"
#include "request.h"
#include "response.h"

void     digitalWrite(int pin, PinState value);
PinState digitalRead(int pin);
void     pinMode(int pin, PinMode mode);

#endif
