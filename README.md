# ASD Arduino-serial-driver 

A multifunctional arduino sketch and a C library with a serial interface to your Arduino board.


### Arduino side

TODO converto to raw bytes instead of strings inside structs
```
arduino-cli compile -b arduino:avr:uno asd_ino/
```

### Compile and run C sample

TODO serialization / deserialization
TODO checksumming
 
```
# Edit usb port in source code
arduino-cli board list  # take note of usb port /dev/ttyUSB0
vim .  

# Compile and run
gcc -Iasd/include main.c asd/src/serial.c asd/src/commands.c -o main.o
./main.o
```


### Diving deeper

The creation of this repo was a learning effort. Here are some key points worth mentioning as a review.

#### Shared libraries 

To avoid code repetition, it was desired to have a `common/` library for both Arduino and C. Using external arduino code is not so simple, and the simplest solution was to ask the user to symbollically link the headers into their .ino directory.

#### Serial communications

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
