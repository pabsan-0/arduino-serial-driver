main: main.c asd_host/** asd_common/**
	gcc -Wall -Wpedantic -g3 -Iasd_common/include -Iasd_host/include asd_host/src/* main.c -o main

clean:
	rm main
