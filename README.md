# ASD Arduino-serial-driver 

A multifunctional arduino sketch and a C library with a serial interface to your Arduino board.

TODO consecutive communications cause timeout on first try of second call  


## Compilation

Build with: 

- `make asd`: builds the local library
- `make arduino`: compiles and uploads the arduino sketch
- `make clean`: cleans up the repository

See also:

- `make main`: to compile a C sample application
- `arduino-cli board list`: to figure the path of the arduino's fd
- `arduino-cli compile -b arduino:TAB`: to figure your arduino's avr 

## Usage


From C/C++:

```
#include "asd.h"

int pinValue;

int main(int argc, char* argv[])
{
    serialBegin("/dev/ttyUSB0");
    
    pinValue = digitalRead(3);
    digitalWrite(3, HIGH);
    pinValue = digitalRead(3);

    serialClose();

    return 0;
}
```

From python3:

```
import asd_host.py
from asd_host.py import HIGH, OUTPUT

asd.serialBegin("/dev/ttyUSB0")

asd.pinMode(3, OUTPUT)
asd.digitalWrite(3, HIGH)
asd.digitalRead(3)

asd.serialClose()
```

## Diving deeper

The creation of this repo was a learning effort. Here are some key points worth mentioning as a review.

### Arduino-cli memo

```
arduino-cli board list  
arduino-cli compile -b arduino:avr:nano sketch/
arduino-cli upload  -b arduino:avr:nano sketch/ -p /dev/ttyUSB0  
```

### Retcode convention

This program's return convention is as follows:

- Functions returning integer 0 mean success
- Functions returning negative integers mean failure
- Serial structs define their convention in the headers
- Declared typedefs do not follow these guidelines

### Shared libraries 

To avoid code repetition, it was desired to have a `common/` library for both Arduino and C. Using external arduino code is not so simple, and the simplest solution was to ask the user to symbollically link the headers into their .ino directory.

### Serial communications

Several cascading ideas were explored towards serial communications:

- Plain string messages: which require easy yet laborous parsing on both endpoints
- Serialized structs: which makes parsing different data types straightforward, yet raised the need for data dividers in between them.
- Serial protocol using custom structs: which coincidentally is a combination of the above.

Implementing such a thing was a big headache, namely the following issues were encountered:

- Configuring the serial termios interface: kept as simple as possible based on the POSIX Serial guide plus the tested examples of `arduino-serial-library`. TODO
- Weird behavior on struct delimiters: When Arduino writes a `\r` on serial, an Ascii-10 rather than a 13 is received. 
- Endianness: Luckily both Arduino and my system are Little-Endian.
- Packing: Structs required packing to avoid parsing issues.
- Field sizes: Arduino and GCC integer sizes differ, one can either use a C99 type or define the spanned bitfield for each ambiguous member of the structs.

### Monitoring serial ports

One can create virtual sockets with socat:

```
socat -d -d pty,raw,echo=0 pty,raw,echo=0
```

Then use the `/dev/pts/{X,Y}` it returns, one to write into and the other to listen.
