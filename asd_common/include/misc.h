#ifndef __ARDUINO_H__
#define __ARDUINO_H__

#define SERIAL_DELIMITER (10) // ascii char

// User conveniences inherited from Arduino
// Bool values
#ifndef HIGH
    #define HIGH 0x1
#endif

#ifndef LOW
    #define LOW 0x0
#endif

// Pin Modes
#ifndef INPUT
    #define INPUT 0x0
#endif

#ifndef OUTPUT
    #define OUTPUT 0x1
#endif

#ifndef INPUT_PULLUP
    #define INPUT_PULLUP 0x2
#endif

enum PinState
{
    PINSTATE_HIGH = HIGH,
    PINSTATE_LOW  = LOW,
};
typedef enum PinState PinState;

enum PinMode
{
    PINMODE_INPUT        = INPUT,
    PINMODE_OUTPUT       = OUTPUT,
    PINMODE_INPUT_PULLUP = INPUT_PULLUP
};
typedef enum PinMode PinMode;

enum Command
{
    COMMAND_DIGITAL_WRITE,
    COMMAND_DIGITAL_READ,
    COMMAND_PIN_MODE
};
typedef enum Command Command;

#endif
