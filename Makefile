asd: asd_host/lib/libasd.so 

asd_host/lib/libasd.so: asd_host/** asd_common/**
	gcc -Wall -Wpedantic -g3 -Iasd_common/include -Iasd_host/include asd_host/src/*.c asd_host/py/*.c -shared -fPIC -o asd_host/lib/libasd.so

all: arduino asd sample00 sample02

arduino:
	ln -s $PWD/asd_common asd_ino || true
	arduino-cli compile -b arduino:avr:nano asd_ino
	arduino-cli upload  -b arduino:avr:nano asd_ino/ -p /dev/ttyUSB0  

sample00: samples/sample00.c asd
	gcc -Wall -Wpedantic -g3 -Iasd_common/include -Iasd_host/include samples/sample00.c -L./asd_host/lib -lasd -Wl,-rpath=./asd_host/lib -o sample00

sample02: samples/sample02.c asd
	gcc -Wall -Wpedantic -g3 -Iasd_common/include -Iasd_host/include samples/sample02.c -L./asd_host/lib -lasd -Wl,-rpath=./asd_host/lib -o sample02

clean:
	rm sample00                   || true
	rm sample02                   || true
	rm asd_host/lib/libasd.so     || true
	rm -r __pycache__             || true
	rm -r asd_host/py/__pycache__ || true
	
# main-src: main.c asd_host/** asd_common/**
# 	gcc -Wall -Wpedantic -g3 -Iasd_common/include -Iasd_host/include asd_host/src/* main.c -o main
