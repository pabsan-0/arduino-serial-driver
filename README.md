# ASD Arduino-serial-driver 

Generic Arduino Sketch and C library to perform Arduino I/O operations from programs running in your PC via serial. 

Speed limits at 9600 baud:

```
digitalRead:    30.666666 calls per second
                92 times in 3.000000 seconds
digitalWrite:   30.333334 calls per second
                91 times in 3.000000 seconds
pinMode:        30.333334 calls per second
                91 times in 3.000000 seconds
```

<table>
<tr><th> C/C++ </th><th> Python </th></tr> <tr><td>

```
#include "asd.h"

int main(int argc, char* argv[])
{
    serialBegin("/dev/ttyUSB0");

    pinMode(3, OUTPUT);
    digitalWrite(3, HIGH);
    int pinValue = digitalRead(3);

    serialClose();
}
```

</td><td>
    
```
import asd_host.py
from asd_host.py import HIGH, OUTPUT

asd.serialBegin("/dev/ttyUSB0")

asd.pinMode(3, OUTPUT)
asd.digitalWrite(3, HIGH)
pin_value = asd.digitalRead(3)

asd.serialClose()
```

</td></tr></table>


To be implemented:
- Baudrate switching
- Benchmarker on arduino side
- Samples: Integral example with its own makefile of a two-side app
- Samples: Basic tests, error showcases


## Compilation

### Host

Build the host side library with:

```
make asd
```

### Arduino 

You can either build and upload using the Arduino Editor, or via the CLI. First of all:

- Plug the arduino and take note of its port: `arduino-cli board list`
- Figure your board's avr based on its model: `arduino-cli compile -b arduino:[TAB][TAB]`

Then, adjust and run these commands to compile and upload.

```
ln -s $PWD/asd_common asd_ino   ## Run from repo root
arduino-cli compile -b arduino:avr:nano asd_ino/
arduino-cli upload  -b arduino:avr:nano asd_ino/ -p /dev/ttyUSB0  
```

The above values are hardcoded in the `Makefile`, if they match yours, simply do `make arduino`.

## Usage

This library is meant to be used in client code. Once the library is in place, this is the basic workflow you should follow.

- Plug the Arduino into your PC
- Figure it's connection port path
- Optionally, check its model's avr and re-upload `asd_ino`
- Compile and run your application:
    - Start the serial at the right port
    - Allow some time for the arduino to reset
    - ...
    - Close the serial connection 

For client code examples, refer to `samples/`.

