main: main.c asd_host/lib/libasd.so
	gcc -Wall -Wpedantic -g3 -Iasd_common/include -Iasd_host/include main.c -L./asd_host/lib -lasd -Wl,-rpath=./asd_host/lib -o main

asd_host/lib/libasd.so: asd_host/** asd_common/**
	gcc -Wall -Wpedantic -g3 -Iasd_common/include -Iasd_host/include asd_host/src/*.c asd_host/py/*.c -shared -fPIC -o asd_host/lib/libasd.so

all: arduino main

main-src: main.c asd_host/** asd_common/**
	gcc -Wall -Wpedantic -g3 -Iasd_common/include -Iasd_host/include asd_host/src/* main.c -o main

arduino:
	ln -s $PWD/asd_common asd_ino || true
	arduino-cli compile -b arduino:avr:nano asd_ino
	arduino-cli upload  -b arduino:avr:nano asd_ino/ -p /dev/ttyUSB0  

clean:
	rm main                       || true
	rm asd_host/lib/libasd.so     || true
	rm -r __pycache__             || true
	rm -r asd_host/py/__pycache__ || true
