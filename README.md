# ASD Arduino-serial-driver 

A multifunctional arduino sketch and a C library with a serial interface to your Arduino board.


### Arduino side

TODO converto to raw bytes instead of strings inside structs

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
