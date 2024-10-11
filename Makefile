main: main.c asd_host/** asd_common/**
	gcc -Wall -Wpedantic -g3 -Iasd_common/include -Iasd_host/include asd_host/src/* main.c -o main

library:
	gcc -Wall -Wpedantic -g3 -Iasd_common/include -Iasd_host/include asd_host/src/* asd_python/src/* -shared -fPIC -o libasd.so

all: arduino main

arduino:
	ln -s $PWD/asd_common asd_ino || true
	arduino-cli compile -b arduino:avr:nano asd_ino
	arduino-cli upload  -b arduino:avr:nano asd_ino/ -p /dev/ttyUSB0  

clean:
	rm main
